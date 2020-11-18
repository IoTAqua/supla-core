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

#ifndef MQTT_CLIENT_DBDATASOURCE_H_
#define MQTT_CLIENT_DBDATASOURCE_H_

#include "database.h"
#include "mqtt_client_datasource.h"

class supla_mqtt_client_db_datasource : public supla_mqtt_client_datasource {
 private:
  database *db;

 protected:

  bool db_connect(void);
  void db_disconnect(void);
  database *get_db(void);

 public:
  supla_mqtt_client_db_datasource(void);
  virtual ~supla_mqtt_client_db_datasource(void);
  void thread_init(void);
  void thread_cleanup(void);
};

#endif /*MQTT_CLIENT_DBDATASOURCE_H_*/
