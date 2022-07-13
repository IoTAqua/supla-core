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

#ifndef ALEXA_CRED_CHANGED_COMMMAND_MOCK_H_
#define ALEXA_CRED_CHANGED_COMMMAND_MOCK_H_

#include <gmock/gmock.h>

#include "ipc/abstract_alexa_cred_changed_command.h"

namespace testing {

class AlexaCredChangedCommandMock
    : public supla_abstract_alexa_cred_changed_command {
 public:
  explicit AlexaCredChangedCommandMock(
      supla_abstract_ipc_socket_adapter *socket_adapter);

  MOCK_METHOD1(on_credentials_changed, void(int user_id));
};

} /* namespace testing */

#endif /* ALEXA_CRED_CHANGED_COMMMAND_MOCK_H_ */
