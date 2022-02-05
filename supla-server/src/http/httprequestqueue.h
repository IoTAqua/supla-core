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

#ifndef HTTP_HTTPREQUESTQUEUE_H_
#define HTTP_HTTPREQUESTQUEUE_H_

#include "commontypes.h"
#include "eh.h"
#include "http/httprequestextraparams.h"
#include "string.h"

class supla_http_request;
class supla_user;

class supla_http_request_queue {
 private:
  static supla_http_request_queue *instance;
  TEventHandler *main_eh;
  void *lck;
  void *arr_queue;
  void *arr_thread;
  int thread_count_limit;
  int queue_offset;
  int last_user_id;
  unsigned long long last_iterate_time_sec;
  unsigned long long time_of_the_next_iteration_usec;

  unsigned long long last_metric_log_time_sec;
  unsigned long long request_total_count;

  void terminateAllThreads(void);
  void runThread(supla_http_request *request);
  supla_http_request *queuePop(void *q_sthread, struct timeval *now);
  int queueSize(void);
  int threadCount(void);
  int threadCountLimit(void);
  unsigned long long requestTotalCount(void);
  void createByChannelEventSourceType(
      supla_user *user, int deviceId, int channelId, event_type eventType,
      event_source_type eventSourceType,
      supla_http_request_extra_params *extraParams);
  void recalculateTime(struct timeval *now);

 public:
  static void init();
  static void queueFree();
  static supla_http_request_queue *getInstance(void);
  supla_http_request_queue();
  virtual ~supla_http_request_queue();

  void recalculateTime(void);
  void raiseEvent(void);
  void logStuckWarning(void);
  void logMetrics(unsigned int min_interval_sec);

  void iterate(void *q_sthread);
  void addRequest(supla_http_request *request);
  void onChannelValueChangeEvent(supla_user *user, int deviceId, int channelId,
                                 event_source_type eventSourceType,
                                 const char correlationToken[] = NULL,
                                 const char googleRequestId[] = NULL);

  void onChannelsAddedEvent(supla_user *user, int deviceId,
                            event_source_type eventSourceType,
                            const char correlationToken[] = NULL,
                            const char googleRequestId[] = NULL);

  void onDeviceDeletedEvent(supla_user *user, int deviceId,
                            event_source_type eventSourceType,
                            const char correlationToken[] = NULL,
                            const char googleRequestId[] = NULL);

  void onUserReconnectEvent(supla_user *user,
                            event_source_type eventSourceType);

  void onGoogleHomeSyncNeededEvent(supla_user *user,
                                   event_source_type eventSourceType);

  void onActionsTriggered(supla_user *user, int deviceId, int channelId,
                          unsigned int actions);
};

void http_request_queue_loop(void *ssd, void *q_sthread);

#endif /* HTTP_HTTPREQUESTQUEUE_H_ */
