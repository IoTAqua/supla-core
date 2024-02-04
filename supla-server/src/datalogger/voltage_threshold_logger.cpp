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

#include "datalogger/voltage_threshold_logger.h"

#include <memory>

#include "datalogger/voltage_threshold_logger_dao.h"
#include "device/devicechannel.h"
#include "user/user.h"

using std::shared_ptr;
using std::vector;
using std::weak_ptr;

supla_voltage_threshold_logger::supla_voltage_threshold_logger()
    : supla_abstract_cyclictask() {}

supla_voltage_threshold_logger::~supla_voltage_threshold_logger() {}

unsigned int supla_voltage_threshold_logger::task_interval_sec(void) {
  return 600;
}

void supla_voltage_threshold_logger::run(
    const vector<supla_user *> *users, supla_abstract_db_access_provider *dba) {
  std::vector<supla_electricity_analyzer *> vas;

  for (auto uit = users->cbegin(); uit != users->cend(); ++uit) {
    (*uit)->get_devices()->for_each([&vas](shared_ptr<supla_device> device,
                                           bool *will_continue) -> void {
      device->get_channels()->for_each([&vas](supla_device_channel *channel,
                                              bool *will_continue) -> void {
        supla_electricity_analyzer *analyzers =
            channel->get_data_analyzer<supla_electricity_analyzer>(true, true);

        if (analyzers) {
          vas.push_back(analyzers);
        }
      });
    });
  }

  supla_voltage_threshold_logger_dao dao(dba);

  for (auto it = vas.begin(); it != vas.end(); ++it) {
    dao.add(*it);
    delete *it;
  }
}
