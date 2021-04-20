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

#include <list>
#include "commontypes.h"
#include "proto.h"

class supla_user;

class channel_address {
 private:
  int DeviceId;
  int ChannelId;

 public:
  channel_address(int DeviceId, int ChannelId);
  int getDeviceId(void);
  int getChannelId(void);
};

class supla_channel_temphum {
 private:
  int ChannelId;
  bool TempAndHumidity;
  double Temperature;
  double Humidity;

 public:
  supla_channel_temphum(bool TempAndHumidity, int ChannelId, double Temperature,
                        double Humidity);
  supla_channel_temphum(bool TempAndHumidity, int ChannelId,
                        const char value[SUPLA_CHANNELVALUE_SIZE]);
  supla_channel_temphum(int ChannelId, int Func,
                        const char value[SUPLA_CHANNELVALUE_SIZE]);

  int getChannelId(void);
  bool isTempAndHumidity(void);
  double getTemperature(void);
  double getHumidity(void);
  void setTemperature(double Temperature);
  void setHumidity(double Humidity);
  void toValue(char value[SUPLA_CHANNELVALUE_SIZE]);
  void assignValue(const char value[SUPLA_CHANNELVALUE_SIZE],
                   bool TempAndHumidity);

  static void free(void *tarr);
};

class supla_channel_electricity_measurement {
 private:
  TElectricityMeter_ExtendedValue_V2 em_ev;
  int ChannelId;
  void assign(int Param2, const char *TextParam1,
              TElectricityMeter_ExtendedValue_V2 *em_ev);

  static void set_costs(int Param2, const char *TextParam1,
                        TElectricityMeter_ExtendedValue *em_ev);
  static void set_costs(int Param2, const char *TextParam1,
                        TElectricityMeter_ExtendedValue_V2 *em_ev);

 public:
  supla_channel_electricity_measurement(int ChannelId,
                                        TElectricityMeter_ExtendedValue *em_ev,
                                        int Param2, const char *TextParam1);
  supla_channel_electricity_measurement(
      int ChannelId, TElectricityMeter_ExtendedValue_V2 *em_ev, int Param2,
      const char *TextParam1);

  int getChannelId(void);
  void getMeasurement(TElectricityMeter_ExtendedValue *em_ev);
  void getMeasurement(TElectricityMeter_ExtendedValue_V2 *em_ev);
  void getCurrency(char currency[4]);

  static bool update_cev(TSC_SuplaChannelExtendedValue *cev, int Param2,
                         const char *TextParam1, bool convert_to_v1);
  static void free(void *emarr);
};

class supla_channel_ic_measurement {
 private:
  int ChannelId;

  _supla_int_t totalCost;
  _supla_int_t pricePerUnit;
  char currency[4];
  char customUnit[9];
  _supla_int_t impulsesPerUnit;
  unsigned _supla_int64_t counter;
  _supla_int64_t calculatedValue;

 public:
  supla_channel_ic_measurement(int ChannelId, int Func,
                               TDS_ImpulseCounter_Value *ic_val,
                               const char *TextParam1, const char *TextParam2,
                               int Param2, int Param3);

  int getChannelId(void);
  _supla_int_t getTotalCost(void);
  _supla_int_t getPricePerUnit(void);
  const char *getCurrency(void);
  const char *getCustomUnit(void);
  _supla_int_t getImpulsesPerUnit(void);
  unsigned _supla_int64_t getCounter(void);
  unsigned _supla_int64_t getCalculatedValue(void);

  static void set_default_unit(int Func, char unit[9]);
  static bool update_cev(TSC_SuplaChannelExtendedValue *cev, int Func,
                         int Param2, int Param3, const char *TextParam1,
                         const char *TextParam2);

  static double get_calculated_d(_supla_int_t impulses_per_unit,
                                 unsigned _supla_int64_t counter);
  static _supla_int64_t get_calculated_i(_supla_int_t impulses_per_unit,
                                         unsigned _supla_int64_t counter);
  static void get_cost_and_currency(const char *TextParam1, int Param2,
                                    char currency[3], _supla_int_t *total_cost,
                                    _supla_int_t *price_per_unit, double count);
  static void free(void *icarr);
};

class supla_channel_thermostat_measurement {
 private:
  int ChannelId;
  bool on;
  double MeasuredTemperature;
  double PresetTemperature;

 public:
  supla_channel_thermostat_measurement(int ChannelId, bool on,
                                       double MeasuredTemperature,
                                       double PresetTemperature);

  int getChannelId(void);
  double getMeasuredTemperature(void);
  double getPresetTemperature(void);
  bool getOn(void);

  static void free(void *icarr);
};

class supla_device_channel {
 private:
  int Id;
  unsigned char Number;
  int UserID;
  int Type;
  int Func;
  int Param1;
  int Param2;
  int Param3;
  int Param4;
  char *TextParam1;
  char *TextParam2;
  char *TextParam3;
  bool Hidden;
  bool Offline;
  unsigned int Flags;

  char value[SUPLA_CHANNELVALUE_SIZE];
  struct timeval value_valid_to;  // during offline
  TSuplaChannelExtendedValue *extendedValue;

 public:
  supla_device_channel(int Id, int Number, int UserID, int Type, int Func,
                       int Param1, int Param2, int Param3, int Param4,
                       const char *TextParam1, const char *TextParam2,
                       const char *TextParam3, bool Hidden, unsigned int Flags,
                       const char value[SUPLA_CHANNELVALUE_SIZE],
                       unsigned _supla_int_t validity_time_sec);
  virtual ~supla_device_channel();

  static void getDefaults(int Type, int Func, int *Param1, int *Param2);
  int getId(void);
  int getNumber(void);
  int getUserID(void);
  int getFunc(void);
  void setFunc(int Func);
  int getType(void);
  int getParam1(void);
  int getParam2(void);
  int getParam3(void);
  int getParam4(void);
  const char *getTextParam1(void);
  const char *getTextParam2(void);
  const char *getTextParam3(void);
  bool getHidden(void);
  unsigned int getFlags();
  bool isOffline(void);
  bool setOffline(bool Offline);
  bool isValueWritable(void);
  bool isCharValueWritable(void);
  bool isRgbwValueWritable(void);
  unsigned int getValueDuration(void);
  unsigned _supla_int_t getValueValidityTimeSec(void);
  void getValue(char value[SUPLA_CHANNELVALUE_SIZE]);
  bool setValue(const char value[SUPLA_CHANNELVALUE_SIZE],
                const unsigned _supla_int_t *validity_time_sec,
                bool *significantChange);
  bool getExtendedValue(TSuplaChannelExtendedValue *ev);
  void setExtendedValue(TSuplaChannelExtendedValue *ev);
  void assignRgbwValue(char value[SUPLA_CHANNELVALUE_SIZE], int color,
                       char color_brightness, char brightness, char on_off);
  void assignCharValue(char value[SUPLA_CHANNELVALUE_SIZE], char cvalue);
  void getDouble(double *Value);
  void getChar(char *Value);
  bool getRGBW(int *color, char *color_brightness, char *brightness,
               char *on_off);
  bool getValveValue(TValve_Value *Value);

  std::list<int> master_channel(void);
  std::list<int> related_channel(void);
  supla_channel_temphum *getTempHum(void);
  supla_channel_electricity_measurement *getElectricityMeasurement(void);
  supla_channel_ic_measurement *getImpulseCounterMeasurement(void);
  supla_channel_thermostat_measurement *getThermostatMeasurement(void);
  bool converValueToExtended(void);
};

class supla_device;
class supla_device_channels {
 private:
  void *arr;
  supla_device *device;

  static char arr_findcmp(void *ptr, void *id);
  static char arr_findncmp(void *ptr, void *n);
  static char arr_delcnd(void *ptr);
  void arr_clean(void);

  void *get_srpc(void);

  supla_device_channel *find_channel(int Id);
  supla_device_channel *find_channel_by_number(int Number);

  std::list<int> mr_channel(int ChannelID, bool Master);

  void async_set_channel_value(supla_device_channel *channel, int SenderID,
                               int GroupID, unsigned char EOL,
                               const char value[SUPLA_CHANNELVALUE_SIZE],
                               bool cancelTasks = true);
  bool set_device_channel_char_value(int SenderID,
                                     supla_device_channel *channel, int GroupID,
                                     unsigned char EOL, const char value,
                                     bool cancelTasks = true);
  bool set_on(int SenderID, int ChannelID, int GroupID, unsigned char EOL,
              bool on, bool toggle);
  bool action_shut_reveal(int SenderID, int ChannelID, int GroupID,
                          unsigned char EOL, bool shut,
                          const char *closingPercentage, bool stop);
  bool action_open_close(int SenderID, int ChannelID, int GroupID,
                         unsigned char EOL, bool unknown, bool open,
                         bool cancelTasks = true);

 public:
  explicit supla_device_channels(supla_device *device);
  virtual ~supla_device_channels();
  void add_channel(int Id, int Number, int UserID, int Type, int Func,
                   int Param1, int Param2, int Param3, int Param4,
                   const char *TextParam1, const char *TextParam2,
                   const char *TextParam3, bool Hidden, unsigned int Flags,
                   const char value[SUPLA_CHANNELVALUE_SIZE],
                   unsigned _supla_int_t validity_time_sec);
  bool get_channel_value(int ChannelID, char value[SUPLA_CHANNELVALUE_SIZE],
                         char *online,
                         unsigned _supla_int_t *validity_time_sec);
  bool get_channel_extendedvalue(int ChannelID,
                                 TSuplaChannelExtendedValue *value);
  bool get_channel_extendedvalue(int ChannelID,
                                 TSC_SuplaChannelExtendedValue *cev,
                                 bool convert_to_v1);
  bool get_channel_double_value(int ChannelID, double *Value);
  supla_channel_temphum *get_channel_temp_and_humidity_value(int ChannelID);
  bool get_channel_temperature_value(int ChannelID, double *Value);
  bool get_channel_humidity_value(int ChannelID, double *Value);
  bool get_channel_char_value(int ChannelID, char *Value);
  bool get_channel_rgbw_value(int ChannelID, int *color, char *color_brightness,
                              char *brightness, char *on_off);

  unsigned int get_channel_value_duration(int ChannelID);
  int get_channel_func(int ChannelID);
  int get_channel_type(int ChannelID);
  bool set_channel_value(int ChannelID, char value[SUPLA_CHANNELVALUE_SIZE],
                         bool *converted2extended,
                         const unsigned _supla_int_t *validity_time_sec,
                         bool *significantChange);
  bool set_channel_offline(int ChannelID, bool Offline);
  void set_channel_extendedvalue(int ChannelID, TSuplaChannelExtendedValue *ev);
  void set_channels_value(TDS_SuplaDeviceChannel_B *schannel_b,
                          TDS_SuplaDeviceChannel_C *schannel_c, int count);

  void on_device_registered(supla_user *user, int DeviceId,
                            TDS_SuplaDeviceChannel_B *schannel_b,
                            TDS_SuplaDeviceChannel_C *schannel_c, int count);

  void set_device_channel_value(int SenderID, int ChannelID, int GroupID,
                                unsigned char EOL,
                                const char value[SUPLA_CHANNELVALUE_SIZE]);
  bool set_device_channel_char_value(int SenderID, int ChannelID, int GroupID,
                                     unsigned char EOL, const char value);
  bool set_device_channel_rgbw_value(int SenderID, int ChannelID, int GroupID,
                                     unsigned char EOL, int color,
                                     char color_brightness, char brightness,
                                     char on_off);
  bool get_channel_valve_value(int ChannelID, TValve_Value *Value);
  bool get_dgf_transparency(int ChannelID, unsigned short *mask);

  std::list<int> master_channel(int ChannelID);
  std::list<int> related_channel(int ChannelID);
  std::list<int> get_channel_ids(void);
  int get_channel_id(unsigned char ChannelNumber);
  bool channel_exists(int ChannelID);
  bool is_channel_online(int ChannelID);
  void load(int UserID, int DeviceID);

  void get_temp_and_humidity(void *tarr);
  void get_electricity_measurements(void *emarr);
  supla_channel_electricity_measurement *get_electricity_measurement(
      int ChannelID);
  void get_ic_measurements(void *icarr);
  supla_channel_ic_measurement *get_ic_measurement(int ChannelID);
  void get_thermostat_measurements(void *tharr);

  bool calcfg_request(int SenderID, int ChannelID, bool SuperUserAuthorized,
                      TCS_DeviceCalCfgRequest_B *request);

  bool get_channel_state(int SenderID, TCSD_ChannelStateRequest *request);

  bool get_channel_complex_value(channel_complex_value *value, int ChannelID);
  void set_channel_function(int ChannelId, int Func);
  void get_functions_request(void);
  void get_int_params_request(TDS_GetChannelIntParamsRequest *request);

  bool set_on(int SenderID, int ChannelID, int GroupID, unsigned char EOL,
              bool on);
  bool set_rgbw(int SenderID, int ChannelID, int GroupID, unsigned char EOL,
                unsigned int *color, char *color_brightness, char *brightness,
                char *on_off);
  bool set_color(int SenderID, int ChannelID, int GroupID, unsigned char EOL,
                 unsigned int color);
  bool set_color_brightness(int SenderID, int ChannelID, int GroupID,
                            unsigned char EOL, char brightness);
  bool set_brightness(int SenderID, int ChannelID, int GroupID,
                      unsigned char EOL, char brightness);
  bool set_dgf_transparency(int SenderID, int ChannelID,
                            unsigned short activeBits, unsigned short mask);
  bool action_toggle(int SenderID, int ChannelID, int GroupID,
                     unsigned char EOL);
  bool action_shut(int SenderID, int ChannelID, int GroupID, unsigned char EOL,
                   const char *closingPercentage);
  bool action_reveal(int SenderID, int ChannelID, int GroupID,
                     unsigned char EOL);
  bool action_stop(int SenderID, int ChannelID, int GroupID, unsigned char EOL);
  bool action_open(int SenderID, int ChannelID, int GroupID, unsigned char EOL);
  bool action_close(int ChannelID);
  bool action_open_close(int SenderID, int ChannelID, int GroupID,
                         unsigned char EOL);
  bool action_open_close_without_canceling_tasks(int SenderID, int ChannelID,
                                                 int GroupID,
                                                 unsigned char EOL);
};

#endif /* DEVICECHANNEL_H_ */
