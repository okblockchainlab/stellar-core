#include "gtest/gtest.h"
#include "wallet.h"
#include <dlfcn.h>

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

typedef bool (*get_address_t)(const std::string& seed, std::string& address);

class AccountTest : public ::testing::Test {
public:
  static void SetUpTestCase() {
    //FIXME: use a better path
    wlt_mod = dlopen("./libstellar-wlt.so", RTLD_LAZY);
    getAddress = (get_address_t)dlsym(wlt_mod, "GetAddressFromPrivateKey");
  }
  static void TearDownTestCase() {
    if (NULL != wlt_mod) {
      dlclose(wlt_mod);
      wlt_mod = NULL;
    }
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

  static void* wlt_mod;
  static get_address_t getAddress;
};
void* AccountTest::wlt_mod = NULL;
get_address_t AccountTest::getAddress = nullptr;

TEST_F(AccountTest, getAddress) {
  std::string address;
  ASSERT_TRUE(getAddress(test_pair1.seed, address));

  ASSERT_STREQ(test_pair1.address, address.c_str());
}

TEST_F(AccountTest, xxx) {
  ASSERT_TRUE(1 < 2);
}
