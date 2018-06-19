#include "wallet.h"
#include "crypto/SecretKey.h"
#include "main/Config.h"
#include "crypto/KeyUtils.h"
#include "test/TxTests.h"
#include "util/Timer.h"
#include "main/ApplicationImpl.h"
#include "database/Database.h"
#include "ledger/AccountFrame.h"
#include "overlay/StellarXDR.h"
#include "xdrpp/marshal.h"
#include "util/Logging.h"
#include "test/TestAccount.h"
#include <cstdlib>

namespace stellar {
  Cold::Cold(const char* seed): seed_(seed) {
  }

  std::string Cold::address() {
    auto sec_key = SecretKey::fromStrKeySeed(seed_);
    return sec_key.getStrKeyPublic();
  }

}/*namespace stellar*/


extern "C"
bool GetAddressFromPrivateKey(const std::string& seed, std::string& address)
{
  stellar::Cold cold(seed.c_str());
  address = cold.address();
  return true;
}

int main(int argc, char* const* argv);

#include <thread>

namespace stellar {
Application::pointer myApp = nullptr;
}

extern "C"
std::vector<uint8_t>
produceUnsignedTx(const std::string& from, const std::string& to, const std::string& amount)
{
  char* argv[] = {
    "stellar-core",
    //"--newdb",
    //"--ll", "trace",
    //"--catchup-complete",
  };

  std::thread main_thread(main, 2, argv);
  //main(1, argv);
  //if (0 != main(2, argv))
   // return {};
     std::this_thread::sleep_for(std::chrono::seconds(10));

  //stellar::Config cfg;
  //FIXME: use a better config path.
  //FIXME: load config when initializing?
  //cfg.load("./stellar-core.cfg");
  //if (cfg.LOG_FILE_PATH.size())
      //stellar::Logging::setLoggingToFile(cfg.LOG_FILE_PATH);
  //stellar::Logging::setLogLevel(el::Level::Info, nullptr);

  //if (cfg.LOG_FILE_PATH.size()) {
    //printf("log file: %s\n", cfg.LOG_FILE_PATH.c_str());
    //stellar::Logging::setLoggingToFile(cfg.LOG_FILE_PATH);
  //}
  //stellar::Logging::setLogLevel(el::Level::Debug, nullptr);

  //uint8_t ver;
  //std::vector<uint8_t> from_pkey;
  stellar::PublicKey from_pkey = stellar::KeyUtils::fromStrKey<stellar::PublicKey>(from);
  stellar::PublicKey to_pkey = stellar::KeyUtils::fromStrKey<stellar::PublicKey>(to);

  std::vector<stellar::Operation> ops = {stellar::txtest::payment(to_pkey, std::atoll(amount.c_str()))};

  auto from_acc = stellar::txtest::loadAccount(from_pkey, *stellar::myApp, false);
  if (!from_acc) {
      return {};
  }

  auto e = stellar::TransactionEnvelope{};
  e.tx.sourceAccount = from_pkey;
  //TODO: is there need to config basefee?
  //I use value of app.getLedgerManager().getTxFee() for simplify the code,
  //which is 100 for now.
  e.tx.fee = static_cast<uint32_t>(
      (ops.size() * 100) & UINT32_MAX);
  e.tx.seqNum = /*from_acc.nextSequenceNumber();*/from_acc->getSeqNum();
  std::copy(std::begin(ops), std::end(ops), std::back_inserter(e.tx.operations));

  auto res = xdr::xdr_to_opaque(e);

  stellar::myApp->gracefulStop();
  while (stellar::myApp->getClock().crank(true))
      ;
  stellar::myApp.reset();
  main_thread.join();

 return res;
}

  /* keypoint:

  Config cfg;
  cfg.load(cfgFile)

  createPaymentTx //签了名的

  //签名
  std::vector<uint8_t> binBlob;
  xdr::xdr_from_opaque(binBlob, envelope);
  auto res = TransactionFrame::makeTransactionFromWire(app.getNetworkID(), e);
  res->addSignature(from);

  xdr::xdr_to_opaque (e) //TransactionEnvelope to bytes
  xdr_argpack_archive

  getAccount
  loadAccount ->AccountFrame::pointer
  acc->getSeqNum
  Application::create

      app->gracefulStop();
      while (clock.crank(true))
          ;

  startApp

  broadcastMessage
  */
