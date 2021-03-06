/*
 Copyright (C) AC SOFTWARE SP. Z O.O.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

#include "accept_loop.h"
#include "database.h"
#include "datalogger.h"
#include "ipcsocket.h"
#include "log.h"
#include "proto.h"
#include "srpc.h"
#include "sthread.h"
#include "supla-socket.h"
#include "svrcfg.h"
#include "tools.h"
#include "user.h"

int main(int argc, char *argv[]) {
  void *ssd_ssl = NULL;
  void *ssd_tcp = NULL;
  void *ipc = NULL;
  void *tcp_accept_loop_t = NULL;
  void *ssl_accept_loop_t = NULL;
  void *ipc_accept_loop_t = NULL;
  void *datalogger_loop_t = NULL;

  // INIT BLOCK
  if (svrcfg_init(argc, argv) == 0) return EXIT_FAILURE;

#if defined(__DEBUG) && __SSOCKET_WRITE_TO_FILE == 1
  unlink("ssocket_read.raw");
  unlink("ssocket_write.raw");
#endif /* defined(__DEBUG) && __SSOCKET_WRITE_TO_FILE == 1 */

  {
    char dt[64];
    int iid[2];

    supla_log(LOG_INFO, "Server version %s [Protocol v%i], %i", SERVER_VERSION,
              SUPLA_PROTO_VERSION, sizeof(iid) * sizeof(int));

    supla_log(LOG_INFO, "Started at %s", st_get_datetime_str(dt));
  }

  if (run_as_daemon && 0 == st_try_fork()) {
    goto exit_fail;
  }

  {
    struct rlimit limit;

    limit.rlim_cur = 10240;
    limit.rlim_max = 10240;
    setrlimit(RLIMIT_NOFILE, &limit);
  }

  if (database::mainthread_init() == false) {
    goto exit_fail;
  }

  {
    database *db = new database();
    if (!db->check_db_version("20180507095139")) {
      delete db;
      database::mainthread_end();
      goto exit_fail;
    } else {
      delete db;
    }
  }

#ifndef NOSSL
  if (scfg_bool(CFG_SSL_ENABLED) == 1) {
    if (0 == (ssd_ssl = ssocket_server_init(scfg_string(CFG_SSL_CERT),
                                            scfg_string(CFG_SSL_KEY),
                                            scfg_int(CFG_SSL_PORT), 1)) ||
        0 == ssocket_openlistener(ssd_ssl)) {
      goto exit_fail;
    }
  }
#endif

  if (scfg_bool(CFG_TCP_ENABLED) == 1) {
    if (0 == (ssd_tcp =
                  ssocket_server_init("", "", scfg_int(CFG_TCP_PORT), 0)) ||
        0 == ssocket_openlistener(ssd_tcp)) {
      goto exit_fail;
    }
  }

  if (0 == st_set_ug_id(scfg_getuid(CFG_UID), scfg_getgid(CFG_GID))) {
    goto exit_fail;
  }

  supla_user::init();

  st_setpidfile(pidfile_path);
  st_mainloop_init();
  st_hook_signals();

  ipc = ipcsocket_init(scfg_string(CFG_IPC_SOCKET_PATH));

  // INI ACCEPT LOOP

  if (ssd_ssl != NULL)
    ssl_accept_loop_t = sthread_simple_run(accept_loop, ssd_ssl, 0);

  if (ssd_tcp != NULL)
    tcp_accept_loop_t = sthread_simple_run(accept_loop, ssd_tcp, 0);

  if (ipc) ipc_accept_loop_t = sthread_simple_run(ipc_accept_loop, ipc, 0);

  // DATA LOGGER
  datalogger_loop_t = sthread_simple_run(datalogger_loop, NULL, 0);

  // MAIN LOOP

  while (st_app_terminate == 0) {
    st_mainloop_wait(1000000);
  }

  supla_log(LOG_INFO, "Shutting down...");

  // RELEASE BLOCK

  if (ipc != NULL) {
    ipcsocket_close(ipc);
    sthread_twf(ipc_accept_loop_t);  // ! after ipcsocket_close and before
                                     // ipcsocket_free !
    ipcsocket_free(ipc);
  }

  if (ssd_ssl != NULL) {
    ssocket_close(ssd_ssl);
    sthread_twf(
        ssl_accept_loop_t);  // ! after ssocket_close and before ssocket_free !
    ssocket_free(ssd_ssl);
  }

  if (ssd_tcp != NULL) {
    ssocket_close(ssd_tcp);
    sthread_twf(
        tcp_accept_loop_t);  // ! after ssocket_close and before ssocket_free !
    ssocket_free(ssd_tcp);
  }

  sthread_twf(datalogger_loop_t);

  st_mainloop_free();
  st_delpidfile(pidfile_path);

  supla_user::free();
  database::mainthread_end();

  scfg_free();

  {
    char dt[64];
    supla_log(LOG_INFO, "Stopped at %s", st_get_datetime_str(dt));
  }

  return EXIT_SUCCESS;

exit_fail:

  ssocket_free(ssd_ssl);
  ssocket_free(ssd_tcp);
  scfg_free();
  exit(EXIT_FAILURE);
}
