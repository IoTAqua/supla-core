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

#ifndef HTTP_ASYNCTASK_HTTP_REQUEST_H_
#define HTTP_ASYNCTASK_HTTP_REQUEST_H_

#include <functional>
#include <memory>

#include "asynctask/abstract_asynctask.h"
#include "caller.h"
#include "commontypes.h"
#include "device/abstract_channel_property_getter.h"
#include "http/abstract_curl_adapter.h"

class supla_asynctask_http_request : public supla_abstract_asynctask {
 private:
  supla_caller caller;
  int user_id;
  int device_id;
  int channel_id;
  event_type et;
  supla_abstract_channel_property_getter *property_getter;

 protected:
  const supla_caller &get_caller(void);
  int get_user_id(void);
  int get_device_id(void);
  int get_channel_id(void);
  event_type get_event_type(void);
  supla_abstract_channel_property_getter *get_property_getter(void);
  supla_channel_value *get_channel_value(int *func, bool *online);

  virtual bool _execute(bool *execute_again,
                        supla_asynctask_thread_bucket *bucket);
  virtual bool make_request(supla_abstract_curl_adapter *curl_adapter) = 0;

 public:
  supla_asynctask_http_request(
      const supla_caller &caller, int user_id, int device_id, int channel_id,
      event_type et, supla_asynctask_queue *queue,
      supla_abstract_asynctask_thread_pool *pool,
      supla_abstract_channel_property_getter *property_getter);

  virtual ~supla_asynctask_http_request(void);
};

#endif /* HTTP_ASYNCTASK_HTTP_REQUEST_H_ */
