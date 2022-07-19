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

#include "action.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

s_worker_action::s_worker_action(s_abstract_worker *worker) {
  this->worker = worker;
}

s_worker_action::~s_worker_action() {}

bool s_worker_action::_is_action_allowed(void) {
  if (!is_action_allowed()) {
    worker->get_db()->set_result(worker->get_id(),
                                 ACTION_EXECUTION_RESULT_CANCELLED);
    return false;
  };

  return true;
}

bool s_worker_action::check_before_start(void) { return false; }

bool s_worker_action::retry_when_fail(void) {
  return worker->retry_when_fail();
}

void s_worker_action::execute(void) {
  if (worker == NULL || worker->get_db() == NULL || !_is_action_allowed())
    return;

  if (!worker->channel_group() && check_before_start() &&
      worker->get_retry_count() == 0 && result_success(NULL)) {
    worker->get_db()->set_result(worker->get_id(),
                                 ACTION_EXECUTION_RESULT_SUCCESS);
    return;
  }

  bool channel_group = worker->channel_group();

  if (worker->get_retry_count() % 2 == 0) {  // SET
    bool action_result = do_action();

    if (channel_group) {
      if (action_result) {
        worker->get_db()->set_result(worker->get_id(),
                                     ACTION_EXECUTED_WITHOUT_CONFIRMATION);
      } else {
        if (worker->ipcc_is_connected() == IPC_RESULT_SERVER_UNREACHABLE) {
          worker->get_db()->set_result(
              worker->get_id(), ACTION_EXECUTION_RESULT_SERVER_UNREACHABLE);
        } else {
          worker->get_db()->set_result(worker->get_id(),
                                       ACTION_EXECUTION_RESULT_FAILURE);
        }
      }

    } else {
      if (worker->get_retry_count() == 0 && try_limit() <= 1) {
        worker->get_db()->set_result(worker->get_id(),
                                     action_result
                                         ? ACTION_EXECUTED_WITHOUT_CONFIRMATION
                                         : ACTION_EXECUTION_RESULT_FAILURE);
      } else {
        worker->get_db()->set_retry(worker->get_id(), waiting_time_to_check());
      }
    }

    return;
  }

  int fail_result_code = 0;

  // CHECK
  if (result_success(&fail_result_code)) {
    worker->get_db()->set_result(worker->get_id(),
                                 ACTION_EXECUTION_RESULT_SUCCESS);
  } else if (retry_when_fail() &&
             worker->get_retry_count() + 1 < (try_limit() * 2)) {
    worker->get_db()->set_retry(
        worker->get_id(), waiting_time_to_retry() - waiting_time_to_check());

  } else if (fail_result_code > 0) {
    worker->get_db()->set_result(worker->get_id(), fail_result_code);

  } else {
    switch (worker->ipcc_is_connected()) {
      case IPC_RESULT_CONNECTED:
        worker->get_db()->set_result(worker->get_id(),
                                     ACTION_EXECUTION_RESULT_FAILURE);
        break;
      case IPC_RESULT_DISCONNECTED:
        worker->get_db()->set_result(
            worker->get_id(), ACTION_EXECUTION_RESULT_DEVICE_UNREACHABLE);
        break;
      case IPC_RESULT_SERVER_UNREACHABLE:
        worker->get_db()->set_result(
            worker->get_id(), ACTION_EXECUTION_RESULT_SERVER_UNREACHABLE);
        break;
    }
  }
}

int s_worker_action::get_max_time(void) {
  return try_limit() * waiting_time_to_retry();
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

std::list<AbstractActionFactory *> AbstractActionFactory::factories;

AbstractActionFactory::~AbstractActionFactory(void) {}

AbstractActionFactory::AbstractActionFactory(int action_type,
                                             std::string classname) {
  assert(AbstractActionFactory::factoryByActionType(action_type) == NULL);

  this->action_type = action_type;
  this->classname = classname;
  factories.push_back(this);
}

int AbstractActionFactory::getActionType(void) { return action_type; }

std::string AbstractActionFactory::getActionClassName(void) {
  return classname;
}

AbstractActionFactory *AbstractActionFactory::factoryByActionType(
    int action_type) {
  for (std::list<AbstractActionFactory *>::iterator it =
           AbstractActionFactory::factories.begin();
       it != AbstractActionFactory::factories.end(); it++) {
    if ((*it)->getActionType() == action_type) return *it;
  }

  return NULL;
}

s_worker_action *AbstractActionFactory::createByActionType(
    int action_type, s_abstract_worker *worker) {
  AbstractActionFactory *factory = NULL;

  if (NULL !=
      (factory = AbstractActionFactory::factoryByActionType(action_type))) {
    return factory->create(worker);
  }

  return NULL;
}
