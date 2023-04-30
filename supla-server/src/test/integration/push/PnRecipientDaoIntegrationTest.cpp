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

#include "integration/push/PnRecipientDaoIntegrationTest.h"

#include <string>

using std::string;

namespace testing {

PnRecipientDaoIntegrationTest::PnRecipientDaoIntegrationTest()
    : IntegrationTest(), Test() {
  dba = nullptr;
  dao = nullptr;
}

PnRecipientDaoIntegrationTest::~PnRecipientDaoIntegrationTest() {}

void PnRecipientDaoIntegrationTest::SetUp() {
  Test::SetUp();

  dba = new supla_db_access_provider();
  ASSERT_TRUE(dba != nullptr);
  dao = new supla_pn_recipient_dao(dba);
  ASSERT_TRUE(dao != nullptr);

  initTestDatabase();
  Test::SetUp();
}

void PnRecipientDaoIntegrationTest::TearDown() {
  Test::TearDown();

  if (dao) {
    delete dao;
    dao = nullptr;
  }

  if (dba) {
    delete dba;
    dba = nullptr;
  }

  Test::TearDown();
}

TEST_F(PnRecipientDaoIntegrationTest, remove) {
  runSqlScript("SetPushNotificationToken.sql");

  string result;
  sqlQuery("SELECT push_token FROM supla_client WHERE id = 24", &result);

  EXPECT_EQ(result, "push_token\nabcd\n");

  supla_pn_recipient r1(3, 0, false, "abcd");
  dao->remove(2, &r1);

  result = "";

  sqlQuery("SELECT push_token FROM supla_client WHERE id = 24", &result);

  EXPECT_EQ(result, "push_token\nabcd\n");

  supla_pn_recipient r2(24, 0, false, "abcd");
  dao->remove(2, &r2);

  result = "";

  sqlQuery("SELECT push_token FROM supla_client WHERE id = 24", &result);

  EXPECT_EQ(result, "push_token\nNULL\n");
}

TEST_F(PnRecipientDaoIntegrationTest, getRecipients) {
  runSqlScript("AddPushNotification.sql");
  supla_pn_recipients recipients;
  dao->get_recipients(2, 6, &recipients);
  EXPECT_EQ(recipients.total_count(), 0);

  dao->get_recipients(1, 5, &recipients);
  EXPECT_EQ(recipients.total_count(), 0);

  dao->get_recipients(2, 5, &recipients);
  EXPECT_EQ(recipients.total_count(), 3);

  supla_pn_recipient *recipient = recipients.get(platform_android, 0);

  EXPECT_EQ(recipient->get_token(), "Token 1");
  EXPECT_EQ(recipient->get_client_id(), 4);
  EXPECT_EQ(recipient->get_app_id(), 0);
  EXPECT_TRUE(recipient->is_exists());
  EXPECT_FALSE(recipient->is_development_env());

  recipient = recipients.get(platform_ios, 0);

  EXPECT_EQ(recipient->get_token(), "Token 2");
  EXPECT_EQ(recipient->get_client_id(), 24);
  EXPECT_EQ(recipient->get_app_id(), 200);
  EXPECT_TRUE(recipient->is_exists());
  EXPECT_FALSE(recipient->is_development_env());

  recipient = recipients.get(platform_ios, 1);

  EXPECT_EQ(recipient->get_token(), "Token 3");
  EXPECT_EQ(recipient->get_client_id(), 31);
  EXPECT_EQ(recipient->get_app_id(), 0);
  EXPECT_TRUE(recipient->is_exists());
  EXPECT_TRUE(recipient->is_development_env());
}

} /* namespace testing */
