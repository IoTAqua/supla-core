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

#include "http/httprequestqueue.h"
#include <unistd.h>  // NOLINT
#include <cstddef>   // NOLINT
#include <list>      // NOLINT
#include "database.h"
#include "http/httprequest.h"
#include "lck.h"
#include "log.h"
#include "safearray.h"
#include "sthread.h"
#include "svrcfg.h"
#include "tools.h"
#include "user/user.h"

typedef struct {
  supla_http_request *request;
  void *sthread;
  void *arr_thread;
} _request_thread_ptr_t;

supla_http_request_queue *supla_http_request_queue::instance = NULL;

char supla_http_request_queue_arr_userspace_arr_queue_clean(void *_request) {
  supla_http_request *request = static_cast<supla_http_request *>(_request);
  if (request) {
    delete request;
    return 1;
  }

  return 0;
}

char supla_http_request_queue_arr_userspace_clean(void *_user_space) {
  _heq_user_space_t *user_space = static_cast<_heq_user_space_t *>(_user_space);
  if (user_space) {
    if (user_space->arr_queue) {
      safe_array_clean(user_space->arr_queue,
                       supla_http_request_queue_arr_userspace_arr_queue_clean);
      safe_array_free(user_space->arr_queue);
      delete user_space;
      return 1;
    }
  }

  return 0;
}

void supla_http_request_thread_execute(void *ptr, void *sthread) {
  database::thread_init();
  static_cast<_request_thread_ptr_t *>(ptr)->request->execute(sthread);
}

void supla_http_request_thread_finish(void *_ptr, void *sthread) {
  _request_thread_ptr_t *ptr = static_cast<_request_thread_ptr_t *>(_ptr);
  void *arr_thread = ptr->arr_thread;

  safe_array_lock(arr_thread);
  delete ptr->request;
  delete ptr;
  safe_array_remove(arr_thread, ptr);
  safe_array_unlock(arr_thread);

  supla_http_request_queue::getInstance()->raiseEvent();
  database::thread_end();
}

// static
void supla_http_request_queue::init(void) {
  supla_http_request_queue::instance = new supla_http_request_queue();
}

// static
void supla_http_request_queue::queueFree(void) {
  if (supla_http_request_queue::instance) {
    delete supla_http_request_queue::instance;
    supla_http_request_queue::instance = NULL;
  }
}

// static
supla_http_request_queue *supla_http_request_queue::getInstance(void) {
  return supla_http_request_queue::instance;
}

supla_http_request_queue::supla_http_request_queue() {
  this->main_eh = eh_init();
  this->arr_user_space = safe_array_init();
  this->arr_thread = safe_array_init();
  this->thread_count_limit = scfg_int(CFG_HTTP_THREAD_COUNT_LIMIT);
  this->last_user_offset = -1;
}

void supla_http_request_queue::terminateAllThreads(void) {
  safe_array_lock(arr_thread);
  for (int a = 0; a < safe_array_count(arr_thread); a++) {
    _request_thread_ptr_t *ptr =
        static_cast<_request_thread_ptr_t *>(safe_array_get(arr_thread, a));
    if (ptr && ptr->request) {
      ptr->request->terminate(ptr->sthread);
    }
  }
  safe_array_unlock(arr_thread);
}

supla_http_request_queue::~supla_http_request_queue() {
  safe_array_free(arr_thread);

  safe_array_clean(arr_user_space,
                   supla_http_request_queue_arr_userspace_clean);
  safe_array_free(arr_user_space);

  eh_free(main_eh);
}

void supla_http_request_queue::runThread(supla_http_request *request) {
  if (!request) {
    return;
  }

  safe_array_lock(arr_thread);
  _request_thread_ptr_t *ptr = new _request_thread_ptr_t();
  safe_array_add(arr_thread, ptr);

  Tsthread_params stp;

  stp.execute = supla_http_request_thread_execute;
  stp.finish = supla_http_request_thread_finish;
  stp.user_data = ptr;
  stp.free_on_finish = true;
  stp.initialize = NULL;

  ptr->arr_thread = arr_thread;
  ptr->request = request;

  ptr->sthread = sthread_run(&stp);

  safe_array_unlock(arr_thread);
}

int supla_http_request_queue::queueSize(void) {
  int size = 0;

  safe_array_lock(arr_user_space);
  for (int a = 0; a < safe_array_count(arr_user_space); a++) {
    _heq_user_space_t *user_space =
        static_cast<_heq_user_space_t *>(safe_array_get(arr_user_space, a));
    size += safe_array_count(user_space->arr_queue);
  }
  safe_array_unlock(arr_user_space);

  return size;
}

int supla_http_request_queue::userCount(void) {
  return safe_array_count(arr_user_space);
}

supla_http_request *supla_http_request_queue::queuePop(void *q_sthread) {
  supla_http_request *result = NULL;

  struct timeval now;
  gettimeofday(&now, NULL);

  safe_array_lock(arr_user_space);
  last_user_offset++;

  if (last_user_offset >= safe_array_count(arr_user_space)) {
    last_user_offset = 0;
  }
  for (int a = last_user_offset; a < safe_array_count(arr_user_space); a++) {
    _heq_user_space_t *user_space =
        static_cast<_heq_user_space_t *>(safe_array_get(arr_user_space, a));

    safe_array_unlock(arr_user_space);
    safe_array_lock(user_space->arr_queue);
    for (int b = 0; b < safe_array_count(user_space->arr_queue); b++) {
      supla_http_request *request = static_cast<supla_http_request *>(
          safe_array_get(user_space->arr_queue, b));

      if (request && !request->isWaiting(&now)) {
        if (request->isCancelled(q_sthread)) {
          delete request;
          request = NULL;
        } else if (request->timeout(NULL)) {
          supla_log(LOG_WARNING,
                    "HTTP request execution timeout! UserID: %i, IODevice: %i "
                    "Channel: %i QS: %i, UC: %i, TC: %i,"
                    "EventSourceType: %i (%lu/%lu/%lu/%lu/%lu)",
                    request->getUserID(), request->getDeviceId(),
                    request->getChannelId(), queueSize(), userCount(),
                    threadCount(), request->getEventSourceType(),
                    request->getTimeout(), request->getStartTime(), now.tv_sec,
                    request->getTouchTimeSec(), request->getTouchCount());

          delete request;
          request = NULL;
        } else {
          result = request;
        }

        safe_array_delete(user_space->arr_queue, b);
        break;
      }

      if (request) {
        request->touch(&now);
      }
    }
    safe_array_unlock(user_space->arr_queue);
    safe_array_lock(arr_user_space);

    if (result) {
      break;
    }
  }
  safe_array_unlock(arr_user_space);

  return result;
}

long supla_http_request_queue::getNextTimeOfDelayedExecution(long time) {
  struct timeval now;
  gettimeofday(&now, NULL);

  safe_array_lock(arr_user_space);

  for (int a = 0; a < safe_array_count(arr_user_space) && time > 1000; a++) {
    _heq_user_space_t *user_space =
        static_cast<_heq_user_space_t *>(safe_array_get(arr_user_space, a));

    safe_array_unlock(arr_user_space);
    safe_array_lock(user_space->arr_queue);

    for (int b = 0; b < safe_array_count(user_space->arr_queue) && time > 1000;
         b++) {
      supla_http_request *request = static_cast<supla_http_request *>(
          safe_array_get(user_space->arr_queue, b));

      if (request) {
        long timeLeft = request->timeLeft(&now);
        if (timeLeft < time) {
          time = timeLeft;
        }
      }
    }
    safe_array_unlock(user_space->arr_queue);
    safe_array_lock(arr_user_space);
  }
  safe_array_unlock(arr_user_space);

  return time;
}

int supla_http_request_queue::threadCount(void) {
  return safe_array_count(arr_thread);
}

int supla_http_request_queue::threadCountLimit(void) {
  return thread_count_limit;
}

void supla_http_request_queue::raiseEvent(void) { eh_raise_event(main_eh); }

void supla_http_request_queue::iterate(void *q_sthread) {
  bool warn_msg = false;
  while (sthread_isterminated(q_sthread) == 0) {
    long wait_time = 10000000;
    if (queueSize() > 0) {
      supla_http_request *request = NULL;

      if (threadCount() < threadCountLimit()) {
        warn_msg = false;
        request = queuePop(q_sthread);
        if (request) {
          runThread(request);
          wait_time = 0;
        } else {
          wait_time = getNextTimeOfDelayedExecution(wait_time);
        }
      } else if (!warn_msg) {
        supla_log(LOG_WARNING,
                  "Http request - thread count limit exceeded (%i)",
                  threadCountLimit());
        warn_msg = true;
      }
    }

    wait_time += 1000;

    if (wait_time < 1000) {
      wait_time = 1000;
    } else if (wait_time > 10000000) {
      wait_time = 10000000;
    }

    if (wait_time > 1000000 && queueSize() > 0) {
      wait_time = 1000000;
    }

    eh_wait(main_eh, wait_time);
  }

  terminateAllThreads();

  int n_wait = 1;
  while (threadCount()) {
    if (n_wait == 20) {
      supla_log(LOG_INFO, "Waiting for http threads...");
    }
    usleep(100000);
    n_wait++;
  }
}

_heq_user_space_t *supla_http_request_queue::getUserSpace(supla_user *user) {
  _heq_user_space_t *result = NULL;

  if (user == NULL) {
    return NULL;
  }

  safe_array_lock(arr_user_space);
  for (int a = 0; a < safe_array_count(arr_user_space); a++) {
    _heq_user_space_t *user_space =
        static_cast<_heq_user_space_t *>(safe_array_get(arr_user_space, a));
    if (user_space && user_space->user == user) {
      result = user_space;
      break;
    }
  }

  if (result == NULL) {
    result = new _heq_user_space_t();
    result->user = user;
    result->arr_queue = safe_array_init();
    if (result->arr_queue == NULL) {
      delete result;
    } else {
      safe_array_add(arr_user_space, result);
    }
  }

  safe_array_unlock(arr_user_space);

  return result;
}

void supla_http_request_queue::addRequest(_heq_user_space_t *user_space,
                                          supla_http_request *request) {
  safe_array_add(user_space->arr_queue, request);
}

void supla_http_request_queue::addRequest(supla_http_request *request) {
  supla_user *user = NULL;
  _heq_user_space_t *user_space = NULL;
  if (request && (user = request->getUser()) != NULL &&
      (user_space = getUserSpace(user)) != NULL) {
    addRequest(user_space, request);
  }
}

void supla_http_request_queue::createByChannelEventSourceType(
    supla_user *user, int deviceId, int channelId, event_type eventType,
    event_source_type eventSourceType, const char correlationToken[],
    const char googleRequestId[]) {
  if (st_app_terminate != 0) {
    return;
  }

  _heq_user_space_t *user_space = getUserSpace(user);
  if (user_space == NULL) {
    return;
  }

  std::list<supla_http_request *> requests =
      AbstractHttpRequestFactory::createByChannelEventSourceType(
          user, deviceId, channelId, eventType, eventSourceType);

  for (std::list<supla_http_request *>::iterator it = requests.begin();
       it != requests.end(); it++) {
    supla_http_request *request = *it;

    safe_array_lock(user_space->arr_queue);

    int ClassID = request->getClassID();

    for (int a = 0; a < safe_array_count(user_space->arr_queue); a++) {
      supla_http_request *existing = static_cast<supla_http_request *>(
          safe_array_get(user_space->arr_queue, a));
      if (existing && existing->getClassID() == ClassID &&
          existing->isDeviceIdEqual(deviceId) &&
          existing->isChannelIdEqual(channelId) &&
          !request->verifyExisting(existing)) {
        break;
      }
    }

    safe_array_unlock(user_space->arr_queue);

    if (request) {
      if (!request->isEventSourceTypeAccepted(eventSourceType, false) ||
          !request->queueUp()) {
        delete request;
        request = NULL;
      }

      if (request) {
        request->setCorrelationToken(correlationToken);
        request->setGoogleRequestId(googleRequestId);
        request->requestWillBeAdded();
        addRequest(user_space, request);
      }
    }
  }

  if (requests.size()) {
    raiseEvent();
  }
}

void supla_http_request_queue::onChannelValueChangeEvent(
    supla_user *user, int deviceId, int channelId,
    event_source_type eventSourceType, const char correlationToken[],
    const char googleRequestId[]) {
  createByChannelEventSourceType(user, deviceId, channelId,
                                 ET_CHANNEL_VALUE_CHANGED, eventSourceType,
                                 correlationToken, googleRequestId);
}

void supla_http_request_queue::onDeviceAddedEvent(
    supla_user *user, int deviceId, event_source_type eventSourceType,
    const char correlationToken[], const char googleRequestId[]) {
  createByChannelEventSourceType(user, deviceId, 0, ET_DEVICE_ADDED,
                                 eventSourceType, correlationToken,
                                 googleRequestId);
}

void supla_http_request_queue::onDeviceDeletedEvent(
    supla_user *user, int deviceId, event_source_type eventSourceType,
    const char correlationToken[], const char googleRequestId[]) {
  createByChannelEventSourceType(user, deviceId, 0, ET_DEVICE_DELETED,
                                 eventSourceType, correlationToken,
                                 googleRequestId);
}

void supla_http_request_queue::onUserReconnectEvent(
    supla_user *user, event_source_type eventSourceType) {
  createByChannelEventSourceType(user, 0, 0, ET_USER_RECONNECT, eventSourceType,
                                 NULL, NULL);
}

void supla_http_request_queue::onGoogleHomeSyncNeededEvent(
    supla_user *user, event_source_type eventSourceType) {
  createByChannelEventSourceType(user, 0, 0, ET_GOOGLE_HOME_SYNC_NEEDED,
                                 eventSourceType, NULL, NULL);
}

void http_request_queue_loop(void *ssd, void *q_sthread) {
  supla_http_request_queue::getInstance()->iterate(q_sthread);
}
