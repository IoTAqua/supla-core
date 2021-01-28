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
#include "ActionTest.h"
#include <list>
#include "WorkerMock.h"
#include "action.h"  // NOLINT
#include "action_rgb.h"
#include "action_shutreveal.h"
#include "gtest/gtest.h"

#define MIN_RETRY_TIME 5
#define MIN_CHECK_TIME 1

namespace {

class ActionTest : public ::testing::Test {
 protected:
};

TEST_F(ActionTest, time) {
  WorkerMock *worker = new WorkerMock(NULL);
  ASSERT_FALSE(worker == NULL);

  for (std::list<AbstractActionFactory *>::iterator it =
           AbstractActionFactory::factories.begin();
       it != AbstractActionFactory::factories.end(); it++) {
    s_worker_action *action = (*it)->create(worker);
    ASSERT_FALSE(action == NULL);
    ASSERT_GE(action->waiting_time_to_retry(), MIN_RETRY_TIME);
    ASSERT_GE(action->waiting_time_to_check(), MIN_CHECK_TIME);

    int diff =
        action->waiting_time_to_retry() - action->waiting_time_to_check();
    ASSERT_GT(diff, 0);

    ASSERT_LT(action->get_max_time(), 280);  // Max time 4 min 40 sec.

    delete action;
  }

  delete worker;
}

TEST_F(ActionTest, parsePercentage) {
  WorkerMock *worker = new WorkerMock(NULL);
  ASSERT_FALSE(worker == NULL);

  s_worker_action_shut *action = new s_worker_action_shut(worker);
  EXPECT_FALSE(action == NULL);

  if (action) {
    char p = 0;

    worker->set_action_param("{\"percentage\":45}");
    EXPECT_TRUE(action->parse_percentage(&p));
    EXPECT_EQ(p, 45);

    p = 0;
    worker->set_action_param("{\"perCentaGe\":80}");
    EXPECT_TRUE(action->parse_percentage(&p));
    EXPECT_EQ(p, 80);

    worker->set_action_param("{\"percentage\":110}");
    EXPECT_FALSE(action->parse_percentage(&p));

    worker->set_action_param("{\"percentage\":-1}");
    EXPECT_FALSE(action->parse_percentage(&p));

    worker->set_action_param(NULL);
    EXPECT_FALSE(action->parse_percentage(&p));

    worker->set_action_param("");
    EXPECT_FALSE(action->parse_percentage(&p));

    delete action;
  }

  delete worker;
}

TEST_F(ActionTest, parseRgb) {
  WorkerMock *worker = new WorkerMock(NULL);
  ASSERT_FALSE(worker == NULL);

  s_worker_action_rgb *action = new s_worker_action_rgb(worker);
  EXPECT_FALSE(action == NULL);

  worker->set_action_param(
      "{\"brightness\":33,\"hue\":0,\"color_brightness\":28}");
  int color = -1;
  char color_brightness = -1;
  char brightness = -1;
  bool random = false;

  EXPECT_TRUE(action->parse_rgbw_params(&color, &color_brightness, &brightness,
                                        &random));

  EXPECT_EQ(color, 0xFF0000);
  EXPECT_EQ(color_brightness, 28);
  EXPECT_EQ(brightness, 33);
  EXPECT_FALSE(random);

  worker->set_action_param(
      "{\"brightness\":15,\"hue\":244,\"color_brightness\":48}");

  color = -1;
  color_brightness = -1;
  brightness = -1;
  random = false;

  EXPECT_TRUE(action->parse_rgbw_params(&color, &color_brightness, &brightness,
                                        &random));

  EXPECT_EQ(color, 0x1000FF);
  EXPECT_EQ(color_brightness, 48);
  EXPECT_EQ(brightness, 15);
  EXPECT_FALSE(random);

  worker->set_action_param(
      "{\"brightness\":31,\"hue\":\"random\",\"color_brightness\":21}");

  color = -1;
  color_brightness = -1;
  brightness = -1;
  random = false;

  EXPECT_TRUE(action->parse_rgbw_params(&color, &color_brightness, &brightness,
                                        &random));

  EXPECT_EQ(color_brightness, 21);
  EXPECT_EQ(brightness, 31);
  EXPECT_TRUE(random);

  worker->set_action_param(
      "{\"brightness\":32,\"hue\":\"white\",\"color_brightness\":22}");

  color = -1;
  color_brightness = -1;
  brightness = -1;
  random = false;

  EXPECT_TRUE(action->parse_rgbw_params(&color, &color_brightness, &brightness,
                                        &random));

  EXPECT_EQ(color, 0xFFFFFF);
  EXPECT_EQ(color_brightness, 22);
  EXPECT_EQ(brightness, 32);
  EXPECT_FALSE(random);

  worker->set_action_param("{\"brightness\":32}");

  color = -1;
  color_brightness = -1;
  brightness = -1;
  random = false;

  EXPECT_TRUE(action->parse_rgbw_params(&color, &color_brightness, &brightness,
                                        &random));

  EXPECT_EQ(color, 0);
  EXPECT_EQ(color_brightness, 0);
  EXPECT_EQ(brightness, 32);
  EXPECT_FALSE(random);

  worker->set_action_param("{\"color_brightness\":32}");

  color = -1;
  color_brightness = -1;
  brightness = -1;
  random = false;

  EXPECT_TRUE(action->parse_rgbw_params(&color, &color_brightness, &brightness,
                                        &random));

  EXPECT_EQ(color, 0);
  EXPECT_EQ(color_brightness, 32);
  EXPECT_EQ(brightness, 0);
  EXPECT_FALSE(random);

  worker->set_action_param("{\"hue\":\"white\"}");

  color = -1;
  color_brightness = -1;
  brightness = -1;
  random = false;

  EXPECT_TRUE(action->parse_rgbw_params(&color, &color_brightness, &brightness,
                                        &random));

  EXPECT_EQ(color, 0xFFFFFF);
  EXPECT_EQ(color_brightness, 0);
  EXPECT_EQ(brightness, 0);
  EXPECT_FALSE(random);

  if (action) {
    delete action;
  }

  delete worker;
}

}  // namespace
