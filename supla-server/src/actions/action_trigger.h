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

#ifndef ACTIONTRIGGER_H_
#define ACTIONTRIGGER_H_

#include <actions/abstract_channel_action_executor.h>

#include "action_trigger_config.h"
#include "device/abstractdevicefinder.h"

class supla_action_trigger {
 private:
  action_trigger_config *config;
  supla_abstract_channel_action_executor *ca_exec;
  supla_abstract_device_finder *dev_finder;

 public:
  supla_action_trigger(supla_abstract_channel_action_executor *ca_exec,
                       action_trigger_config *config,
                       supla_abstract_device_finder *dev_finder);
  ~supla_action_trigger(void);

  void execute_actions(int user_id, unsigned int caps);
};

#endif /* ACTIONTRIGGER_H_ */
