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

#ifndef GOOGLEHOMEREQUEST_H_
#define GOOGLEHOMEREQUEST_H_

#include "google/googlehomeclient.h"
#include "http/httprequest.h"

class supla_google_home_request : public supla_http_request {
 private:
  void *lck;
  supla_google_home_client *client;

 protected:
  bool duplicateExists;

 public:
  supla_google_home_request(supla_user *user, int ClassID, int DeviceId,
                            int ChannelId, event_type EventType,
                            const supla_caller &Caller);
  virtual ~supla_google_home_request();

  supla_google_home_client *getClient(void);
  virtual bool queueUp(void);
  virtual bool isCancelled(void *sthread);
  virtual void terminate(void *sthread);
  virtual bool isCallerAccepted(const supla_caller &caller,
                                         bool verification);
  virtual bool isDeviceIdEqual(int DeviceId);
  virtual bool isChannelIdEqual(int ChannelId);
};

#endif /* GOOGLEHOMEREQUEST_H_ */
