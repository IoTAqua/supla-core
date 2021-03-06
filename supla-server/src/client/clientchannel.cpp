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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "log.h"
#include "proto.h"
#include "safearray.h"
#include "srpc.h"
#include "user.h"
#include "client.h"
#include "clientchannel.h"

supla_client_channel::supla_client_channel(supla_client_channels *Container,
                                           int Id, int DeviceId, int LocationID,
                                           int Func, int Param1, int Param2,
                                           const char *Caption, int AltIcon,
                                           unsigned char ProtocolVersion)
    : supla_client_objcontainer_item(Container, Id, Caption) {
  this->DeviceId = DeviceId;
  this->LocationId = LocationID;
  this->Func = Func;
  this->Param1 = Param1;
  this->Param2 = Param2;
  this->AltIcon = AltIcon;
  this->ProtocolVersion = ProtocolVersion;
  this->Flags = 0;
}

int supla_client_channel::getDeviceId() { return DeviceId; }

int supla_client_channel::getExtraId() { return DeviceId; }

bool supla_client_channel::remote_update_is_possible(void) {
  switch (Func) {
    case SUPLA_CHANNELFNC_CONTROLLINGTHEDOORLOCK:
    case SUPLA_CHANNELFNC_CONTROLLINGTHEGATEWAYLOCK:
    case SUPLA_CHANNELFNC_CONTROLLINGTHEGATE:
    case SUPLA_CHANNELFNC_CONTROLLINGTHEGARAGEDOOR:
    case SUPLA_CHANNELFNC_THERMOMETER:
    case SUPLA_CHANNELFNC_HUMIDITY:
    case SUPLA_CHANNELFNC_HUMIDITYANDTEMPERATURE:
    case SUPLA_CHANNELFNC_NOLIQUIDSENSOR:
    case SUPLA_CHANNELFNC_CONTROLLINGTHEROLLERSHUTTER:
    case SUPLA_CHANNELFNC_POWERSWITCH:
    case SUPLA_CHANNELFNC_LIGHTSWITCH:
    case SUPLA_CHANNELFNC_DIMMER:
    case SUPLA_CHANNELFNC_RGBLIGHTING:
    case SUPLA_CHANNELFNC_DIMMERANDRGBLIGHTING:
    case SUPLA_CHANNELFNC_DEPTHSENSOR:
    case SUPLA_CHANNELFNC_DISTANCESENSOR:
    case SUPLA_CHANNELFNC_MAILSENSOR:
    case SUPLA_CHANNELFNC_WINDSENSOR:
    case SUPLA_CHANNELFNC_PRESSURESENSOR:
    case SUPLA_CHANNELFNC_RAINSENSOR:
    case SUPLA_CHANNELFNC_WEIGHTSENSOR:
    case SUPLA_CHANNELFNC_WEATHER_STATION:
    case SUPLA_CHANNELFNC_STAIRCASETIMER:

      return true;

    case SUPLA_CHANNELFNC_OPENINGSENSOR_GATEWAY:
    case SUPLA_CHANNELFNC_OPENINGSENSOR_GATE:
    case SUPLA_CHANNELFNC_OPENINGSENSOR_GARAGEDOOR:
    case SUPLA_CHANNELFNC_OPENINGSENSOR_DOOR:
    case SUPLA_CHANNELFNC_OPENINGSENSOR_ROLLERSHUTTER:
    case SUPLA_CHANNELFNC_OPENINGSENSOR_WINDOW:

      if (Param1 == 0 && Param2 == 0) {
        return true;
      }

      break;
  }

  return false;
}

void supla_client_channel::proto_get(TSC_SuplaChannel *channel,
                                     supla_client *client) {
  memset(channel, 0, sizeof(TSC_SuplaChannel));

  channel->Id = getId();
  channel->Func = Func;
  channel->LocationID = this->LocationId;

  if (client && client->getUser()) {
    client->getUser()->get_channel_value(DeviceId, getId(), &channel->value,
                                         &channel->online);
  }

  proto_get_caption(channel->Caption, &channel->CaptionSize,
                    SUPLA_CHANNEL_CAPTION_MAXSIZE);
}

void supla_client_channel::proto_get(TSC_SuplaChannel_B *channel,
                                     supla_client *client) {
  memset(channel, 0, sizeof(TSC_SuplaChannel_B));

  channel->Id = getId();
  channel->Func = Func;
  channel->LocationID = this->LocationId;
  channel->AltIcon = this->AltIcon;
  channel->ProtocolVersion = this->ProtocolVersion;
  channel->Flags = this->Flags;

  if (client && client->getUser()) {
    client->getUser()->get_channel_value(DeviceId, getId(), &channel->value,
                                         &channel->online);
  }

  proto_get_caption(channel->Caption, &channel->CaptionSize,
                    SUPLA_CHANNEL_CAPTION_MAXSIZE);
}

void supla_client_channel::proto_get(TSC_SuplaChannelValue *channel_value,
                                     supla_client *client) {
  memset(channel_value, 0, sizeof(TSC_SuplaChannelValue));
  channel_value->Id = getId();

  if (client && client->getUser()) {
    client->getUser()->get_channel_value(
        DeviceId, getId(), &channel_value->value, &channel_value->online);
  }
}
