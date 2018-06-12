#include "wallet.h"
#include "crypto/SecretKey.h"

namespace stellar {
  Cold::Cold(const char* seed): seed_(seed) {
  }

  std::string Cold::address() {
    auto sec_key = SecretKey::fromStrKeySeed(seed_);
    return sec_key.getStrKeyPublic();
  }

}/*namespace stellar*/


bool GetAddressFromPrivateKey(const std::string& seed, std::string& address)
{
  stellar::Cold cold(seed.c_str());
  address = cold.address();
  return true;
}

__attribute__ ((visibility("default")))
std::vector<uint8_t>
produceUnsignedTx(const std::string& target_address, const std::string& amount)
{
  xxx
  /* keypoint:

  Config cfg;
  cfg.load(cfgFile)

  createPaymentTx //签了名的
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
}
