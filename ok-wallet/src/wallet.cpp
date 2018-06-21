#include "wallet.h"
#include "crypto/SecretKey.h"
#include "crypto/KeyUtils.h"
#include "xdrpp/marshal.h"
#include "app_wrapper.h"
#include "test/TxTests.h"
#include "crypto/SHA.h"

void loadConfig(const std::string& net_type, stellar::Config& cfg);

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


extern "C"
bool
produceUnsignedTx(const std::string& from, const std::string& to, const std::string& amount, const std::string& net_type, std::vector<uint8_t>& utx)
{
  utx.clear();

  AppWrapper aw(net_type);
  if (!aw.update_ledger()) {
    return false;
  }

  stellar::PublicKey from_pkey = stellar::KeyUtils::fromStrKey<stellar::PublicKey>(from);
  stellar::PublicKey to_pkey = stellar::KeyUtils::fromStrKey<stellar::PublicKey>(to);

  auto from_acc = aw.loadAccount(from_pkey);
  if (!from_acc) {
      return false;
  }

  std::vector<stellar::Operation> ops = {stellar::txtest::payment(to_pkey, std::atoll(amount.c_str()))};

  auto e = stellar::TransactionEnvelope{};
  e.tx.sourceAccount = from_pkey;
  //TODO: does here need to config basefee?
  //I use value of app.getLedgerManager().getTxFee() for simplify the code,
  //which is 100 for now.
  e.tx.fee = static_cast<uint32_t>(
      (ops.size() * 100) & UINT32_MAX);
  e.tx.seqNum = /*from_acc.nextSequenceNumber();*/from_acc->getSeqNum();
  std::copy(std::begin(ops), std::end(ops), std::back_inserter(e.tx.operations));

  utx = xdr::xdr_to_opaque(e);
  return true;
}

extern "C"
bool
signTransaction(const uint8_t* unsigned_tx, const size_t size, const std::string& seed, const std::string& net_type, std::vector<uint8_t>& stx)
{
  stellar::Config cfg;
  loadConfig(net_type, cfg);
  const auto network_id = stellar::sha256(cfg.NETWORK_PASSPHRASE); //app.getNetworkID

  auto from_skey = stellar::SecretKey::fromStrKeySeed(seed);

  stellar::TransactionEnvelope envelope;
  std::vector<uint8_t> binBlob(unsigned_tx, unsigned_tx + size);
  xdr::xdr_from_opaque(binBlob, envelope);
  auto t = stellar::TransactionFrame::makeTransactionFromWire(network_id, envelope);
  t->addSignature(from_skey);

  stx = xdr::xdr_to_opaque(t->getEnvelope());
  return true;
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
