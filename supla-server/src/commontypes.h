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

#ifndef COMMONTYPES_H_
#define COMMONTYPES_H_

#include "proto.h"

enum event_source_type {
  EST_UNKNOWN,
  EST_DEVICE,
  EST_CLIENT,
  EST_IPC,
  EST_AMAZON_ALEXA,
  EST_GOOGLE_HOME,
};

enum event_type {
  ET_CHANNEL_VALUE_CHANGED,
  ET_DEVICE_DELETED,
  ET_CHANNELS_ADDED,
  ET_USER_RECONNECT,
  ET_GOOGLE_HOME_SYNC_NEEDED,
};

typedef struct {
  bool online;
  bool hidden_channel;
  int function;
  int channel_type;
  int channel_flags;
  bool hi;
  char brightness;
  char color_brightness;
  int color;
  char on_off;
  double temperature;
  double humidity;
  double wind;
  double pressure;
  double rain;
  double weight;
  double distance;
  double depth;
  char shut;
  bool partially_closed;
  bool overcurrent_relay_off;
  TValve_Value valve_value;
} channel_complex_value;

typedef struct {
  int deviceId;
  int channelId;
  bool channel_is_hidden;
  int channel_type;
  int function;
} channel_function_t;

#endif /* COMMONTYPES_H_ */
