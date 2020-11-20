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

#include "MqttDataSourceMock.h"
#include <stdio.h>
#include <string.h>

MqttDataSourceMock::MqttDataSourceMock(supla_mqtt_client_settings *settings)
    : supla_mqtt_client_datasource(settings) {
  this->open_count = 0;
  this->close_count = 0;
  this->idx = -1;
}
MqttDataSourceMock::~MqttDataSourceMock(void) {}

bool MqttDataSourceMock::context_open(const _mqtt_ds_context_t *context) {
  open_count++;
  idx = 0;
  return true;
}

bool MqttDataSourceMock::_fetch(const _mqtt_ds_context_t *context,
                                char **topic_name, void **message,
                                size_t *message_size) {
  if (idx < 0 || idx > 2) {
    return false;
  }

  if (topic_name) {
    *topic_name = (char *)malloc(100);
    snprintf(*topic_name, sizeof(*topic_name),
             "/user/%i/device/%i/channel/%i/%i", context->user_id,
             context->device_id, context->channel_id, idx);
  }

  if (message && message_size) {
    char scope[30];

    switch (context->scope) {
      case MQTTDS_SCOPE_NONE:
        snprintf(scope, sizeof(scope), "%s%i", "SCOPE_NONE", idx);
        break;
      case MQTTDS_SCOPE_FULL:
        snprintf(scope, sizeof(scope), "%s%i", "SCOPE_FULL", idx);
        break;
      case MQTTDS_SCOPE_USER:
        snprintf(scope, sizeof(scope), "%s%i", "SCOPE_USER", idx);
        break;
      case MQTTDS_SCOPE_DEVICE:
        snprintf(scope, sizeof(scope), "%s%i", "SCOPE_DEVICE", idx);
        break;
      case MQTTDS_SCOPE_CHANNEL_VALUE:
        snprintf(scope, sizeof(scope), "%s%i", "CHANNEL_VALUE", idx);
        break;
    }

    *message_size = strlen(scope) + 1;
    *message = malloc(*message_size);
    memcpy(*message, scope, *message_size);
  }

  idx++;
  return true;
}

void MqttDataSourceMock::context_close(const _mqtt_ds_context_t *context) {
  close_count++;
}

int MqttDataSourceMock::openCount(void) { return open_count; }

int MqttDataSourceMock::closeCount(void) { return close_count; }
