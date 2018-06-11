#include "gtest/gtest.h"
#include "wallet.h"

struct pair_t {
  const char* seed;
  const char* address;
};

static const pair_t test_pair1 = {
  "SBCRVCYITA3WRDNODQOPUKNMSAMFF7YZZMITJABUHWWIGDOBFZKVNEHO",
  "GBHVJDEOZU2CZY7QA6AUE5GOZF3TE4BXFTJVMPUS52X6TGKTIZ75H4US",
};

static const pair_t test_pair2 = {
  "SD2WEMOWNWP4WIQKHP2KVZH6ZJX2NODMCXTFASZLK24Y5Z33FS7NYCW4",
  "GA4SNMFYYT77VX7NPUXTDDVNX6FIAF2IIZEJEOFKBZ2U5GPX2OU6O7BW",
};


TEST(Hot, getAddress) {
  stellar::Hot hot(test_pair1.seed);

  ASSERT_STREQ(test_pair1.address, hot.address().c_str());
}
