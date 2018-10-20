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
typedef bool (*produce_unsigned_tx_t)(const std::string& from, const std::string& to, const std::string& amount, const std::string& fee, const std::string& seqNum, std::vector<uint8_t>& utx);
typedef bool (*sign_transaction_t)(const std::vector<uint8_t>& utx, const std::string& seed, const std::string& net_type, std::vector<uint8_t>& stx);
typedef bool (*commit_transaction_t)(const std::vector<uint8_t>& stx, const std::string& net_type, const char* data_dir, std::string& result_str);

class OKWalletTest : public ::testing::Test {
public:
  static void SetUpTestCase() {
    #ifdef __APPLE__
    const char* dll_path = "./libstellar-core.dylib";
    #elif __linux__
    const char* dll_path = "./libstellar-core.so";
    #endif
    wlt_mod = dlopen(dll_path, RTLD_LAZY);
    getAddress = (get_address_t)dlsym(wlt_mod, "GetAddressFromPrivateKey");
    produceUnsignedTx = (produce_unsigned_tx_t)dlsym(wlt_mod, "produceUnsignedTx");
    signTransaction = (sign_transaction_t)dlsym(wlt_mod, "signTransaction");
    commitTransaction = (commit_transaction_t)dlsym(wlt_mod, "commitTransaction");
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
  static produce_unsigned_tx_t produceUnsignedTx;
  static sign_transaction_t signTransaction;
  static commit_transaction_t commitTransaction;
};
void* OKWalletTest::wlt_mod = NULL;
get_address_t OKWalletTest::getAddress = nullptr;
produce_unsigned_tx_t OKWalletTest::produceUnsignedTx = nullptr;
sign_transaction_t OKWalletTest::signTransaction = nullptr;
commit_transaction_t OKWalletTest::commitTransaction = nullptr;

TEST_F(OKWalletTest, getAddress) {
  std::string address;
  ASSERT_TRUE(getAddress(test_pair1.seed, address));

  ASSERT_STREQ(test_pair1.address, address.c_str());
}

TEST_F(OKWalletTest, produceUnsignedTx) {
  std::vector<uint8_t> expect_tx = {1, 2, 3};
  std::vector<uint8_t> utx;

  ASSERT_TRUE(produceUnsignedTx(test_pair1.address, test_pair2.address, "100", "1", "212", utx));
  ASSERT_TRUE(utx.size() > 0);
}

TEST_F(OKWalletTest, signTransaction) {
  std::vector<uint8_t> utx;

  ASSERT_TRUE(produceUnsignedTx(test_pair1.address, test_pair2.address, "100", "", "121", utx));
  ASSERT_TRUE(utx.size() > 0);

  std::vector<uint8_t> stx;
  ASSERT_TRUE(signTransaction(utx, test_pair1.seed, "testnet", stx));
  ASSERT_TRUE(stx.size() > utx.size());
}

TEST_F(OKWalletTest, commitTransaction) {
  std::vector<uint8_t> utx;

  ASSERT_TRUE(produceUnsignedTx(test_pair1.address, test_pair2.address, "100", "", "10", utx));
  ASSERT_TRUE(utx.size() > 0);

  std::vector<uint8_t> stx;
  ASSERT_TRUE(signTransaction(utx, test_pair1.seed, "testnet", stx));
  ASSERT_TRUE(stx.size() > utx.size());

  std::string resstr;
  ASSERT_TRUE(commitTransaction(stx, "testnet", ".", resstr));
}
