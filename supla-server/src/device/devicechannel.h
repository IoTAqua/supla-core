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

#ifndef DEVICECHANNEL_H_
#define DEVICECHANNEL_H_

#include <channel_temphum_value.h>

#include <functional>
#include <list>
#include <map>
#include <vector>

#include "analyzer/voltage_analyzers.h"
#include "caller.h"
#include "channel_address.h"
#include "channel_electricity_measurement.h"
#include "channel_ic_measurement.h"
#include "channel_thermostat_measurement.h"
#include "channel_value.h"
#include "commontypes.h"
#include "proto.h"

class supla_user;
class supla_device;
class channel_json_config;

enum rsAction {
  rsActionStop,
  rsActionDown,
  rsActionUp,
  rsActionDownOrStop,
  rsActionUpOrStop,
  rsActionStepByStep,
  rsActionShut,
  rsActionReveal
};

typedef struct {
  union {
    char value[SUPLA_CHANNELVALUE_SIZE];
    TSuplaChannelExtendedValue *extendedValue;
  };
} _logger_purpose_t;

class supla_device_channel {
 private:
  void *lck;
  supla_device *device;
  const int id;
  const unsigned char number;
  const int type;
  int func;
  int param1;
  const int param2;
  const int param3;
  const int param4;
  char *text_param1;
  char *text_param2;
  char *text_param3;
  const bool hidden;
  bool offline;
  unsigned int flags;
  TDSC_ChannelState *state;
  char value[SUPLA_CHANNELVALUE_SIZE];
  struct timeval value_valid_to;  // during offline
  TSuplaChannelExtendedValue *extended_value;
  channel_json_config *json_config;
  _logger_purpose_t *logger_data;
  supla_voltage_analyzers voltage_analyzers;

  void db_set_properties(channel_json_config *config);
  void db_set_params(int param1, int param2, int param3, int param4);
  void update_timer_state(void);
  void update_extended_electricity_meter_value(void);
  supla_channel_value *_get_channel_value(void);

 public:
  supla_device_channel(supla_device *device, int id, int number, int type,
                       int func, int param1, int param2, int param3, int param4,
                       const char *text_param1, const char *text_param2,
                       const char *text_param3, bool hidden, unsigned int flags,
                       const char value[SUPLA_CHANNELVALUE_SIZE],
                       unsigned _supla_int_t validity_time_sec,
                       const char *user_config, const char *properties);
  virtual ~supla_device_channel();

  static void get_defaults(int type, int func, int *param1, int *param2);
  static int func_list_filter(int func_list, int type);

  void lock(void);
  void unlock(void);
  int get_id(void);
  int get_number(void);
  int get_user_id(void);
  supla_user *get_user();
  supla_device *get_device();
  int get_func(void);
  void set_func(int func);
  int get_type(void);
  int get_param1(void);
  int get_param2(void);
  int get_param3(void);
  int get_param4(void);
  const char *get_text_param1(void);
  const char *get_text_param2(void);
  const char *get_text_param3(void);
  bool is_hidden(void);
  unsigned int get_flags();
  void add_flags(unsigned int flags);
  bool is_offline(void);
  bool set_offline(bool Offline);
  bool is_value_writable(void);
  bool is_char_value_writable(void);
  bool is_rgbw_value_writable(void);
  unsigned int get_value_duration(void);
  unsigned _supla_int_t get_value_validity_time_sec(void);
  void get_value(char value[SUPLA_CHANNELVALUE_SIZE]);
  bool set_value(const char value[SUPLA_CHANNELVALUE_SIZE],
                 const unsigned _supla_int_t *validity_time_sec,
                 bool *significantChange, unsigned char proto_version);
  bool get_extended_value(TSuplaChannelExtendedValue *ev, bool em_update);
  void set_extended_value(TSuplaChannelExtendedValue *ev);
  void assign_rgbw_value(char value[SUPLA_CHANNELVALUE_SIZE], int color,
                         char color_brightness, char brightness, char on_off);
  void get_double(double *value);
  void get_char(char *value);
  bool get_config(TSD_ChannelConfig *config, unsigned char config_type,
                  unsigned _supla_int_t flags);
  void set_action_trigger_config(unsigned int capabilities,
                                 int related_channel_id,
                                 unsigned int disables_local_operation);

  std::list<int> master_channel(void);
  std::list<int> related_channel(void);
  supla_channel_electricity_measurement *get_electricity_measurement(
      bool for_data_logger_purposes);
  supla_channel_ic_measurement *get_impulse_counter_measurement(
      bool for_data_logger_purposes);
  supla_channel_thermostat_measurement *get_thermostat_measurement(void);
  channel_json_config *get_json_config(void);
  bool conver_value_to_extended(void);
  unsigned int get_value_validity_time_left_msec(void);
  void set_state(TDSC_ChannelState *state);
  bool get_state(TDSC_ChannelState *state);
  bool get_voltage_analyzers_with_any_sample_over_threshold(
      supla_voltage_analyzers *voltage_analyzers, bool reset);

  template <typename T>
  T *get_channel_value(void);
};

template <typename T>
T *supla_device_channel::get_channel_value(void) {
  supla_channel_value *value = _get_channel_value();
  if (value) {
    T *expected = dynamic_cast<T *>(value);
    if (!expected) {
      delete value;
    }
    return expected;
  }
  return nullptr;
}

#endif /* DEVICECHANNEL_H_ */
