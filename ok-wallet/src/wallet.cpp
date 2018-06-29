#include "wallet.h"
#include "crypto/SecretKey.h"
#include "crypto/KeyUtils.h"
#include "xdrpp/marshal.h"
#include "app_wrapper.h"
#include "test/TxTests.h"
#include "crypto/SHA.h"
#include "herder/Herder.h"
#include "overlay/OverlayManager.h"
#include "com_okcoin_vault_jni_stellar_Stellarj.h"
#include <regex>

void loadConfig(const std::string& net_type, const char* data_dir, bool listen, stellar::Config& cfg);

std::string bytes2string(const std::vector<uint8_t>& byte_vec)
{
  std::string result;
  result.reserve(byte_vec.size() * 2);

  for (const auto& b : byte_vec) {
    const uint8_t h = (b >> 4) & 0xF;
    const uint8_t l = b & 0xF;

    assert(h >= 0 && h <= 0xF);
    assert(l >= 0 && l <= 0xF);

    if (h < 0xA) {
      result.push_back(h + 0x30);
    } else {
      result.push_back(h - 0xA + 'A');
    }
    if (l < 0xA) {
      result.push_back(l + 0x30);
    } else {
      result.push_back(l - 0xA + 'A');
    }
  }

  return result;
}

std::vector<uint8_t> string2bytes(const std::string& str)
{
  assert(str.length() % 2 == 0);

  std::vector<uint8_t> result;
  result.reserve(str.length() / 2);

  for(size_t i = 0; i < str.length(); i += 2) {
    const char h = tolower(str[i]);
    const char l = tolower(str[i + 1]);

    assert((0x30 <= h && h <= 0x39) || ( 'a' <= h && h <= 'f'));
    assert((0x30 <= l && l <= 0x39) || ( 'a' <= l && l <= 'f'));

    uint8_t b;
    if (0x30 <= h && h <= 0x39) {
      b = h - 0x30;
    } else {
      b = h - 'a' + 0xA;
    }
    b = (b << 4) & 0xF0;

    if (0x30 <= l && l <= 0x39) {
      b += l - 0x30;
    } else {
      b += l - 'a' + 0xA;
    }

    result.push_back(b);
  }

  return result;
}

jstring char2jstring(JNIEnv* env, const char* pat)
{
  jclass strClass = env->FindClass("Ljava/lang/String;");
  jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");

  jbyteArray bytes = env->NewByteArray(strlen(pat));
  env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
  jstring encoding = env->NewStringUTF("utf-8");
  return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

std::string jstring2stdstring(JNIEnv* env, const jstring& jstr)
{
  jclass clsstring = env->FindClass("java/lang/String");
  jstring strencode = env->NewStringUTF("utf-8");
  jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
  jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
  jsize alen = env->GetArrayLength(barr);
  jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);

  std::string res((const char*)ba, alen);
  env->ReleaseByteArrayElements(barr, ba, 0);
  return res;
}

jobjectArray string2jobjectArray(JNIEnv* env, const std::string& s)
{
  jobjectArray result = env->NewObjectArray(1, env->FindClass("java/lang/String"), 0);

  env->SetObjectArrayElement(result, 0, char2jstring(env, s.c_str()));
  return result;
}

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
produceUnsignedTx(const std::string& from, const std::string& to, const std::string& amount, const std::string& net_type, const char* data_dir, std::vector<uint8_t>& utx)
{
  utx.clear();

  AppWrapper aw(net_type, data_dir);
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
  e.tx.seqNum = /*from_acc.nextSequenceNumber();*/from_acc->getSeqNum() + 1;
  std::copy(std::begin(ops), std::end(ops), std::back_inserter(e.tx.operations));

  utx = xdr::xdr_to_opaque(e);
  return true;
}

extern "C"
bool
signTransaction(const std::vector<uint8_t>& utx, const std::string& seed, const std::string& net_type, const char* data_dir, std::vector<uint8_t>& stx)
{
  stellar::Config cfg;
  loadConfig(net_type, data_dir, false, cfg);
  const auto network_id = stellar::sha256(cfg.NETWORK_PASSPHRASE); //app.getNetworkID

  auto from_skey = stellar::SecretKey::fromStrKeySeed(seed);

  stellar::TransactionEnvelope envelope;
  xdr::xdr_from_opaque(utx, envelope);
  auto t = stellar::TransactionFrame::makeTransactionFromWire(network_id, envelope);
  t->addSignature(from_skey);

  stx = xdr::xdr_to_opaque(t->getEnvelope());
  return true;
}

bool commitTransaction(const std::vector<uint8_t>& stx, const std::string& net_type, const char* data_dir, std::string& result_str)
{
  AppWrapper aw(net_type, data_dir, true);

  stellar::TransactionEnvelope envelope;
  xdr::xdr_from_opaque(stx, envelope);
  auto tx_frame = stellar::TransactionFrame::makeTransactionFromWire(aw.app()->getNetworkID(), envelope);
  if (!tx_frame) {
    return false;
  }

  bool success = true;

  // add it to our current set
  // and make sure it is valid
  auto status = aw.app()->getHerder().recvTransaction(tx_frame);
  if (status == stellar::Herder::TX_STATUS_PENDING) {
    stellar::StellarMessage msg;
    msg.type(stellar::TRANSACTION);
    msg.transaction() = envelope;
    aw.app()->getOverlayManager().broadcastMessage(msg);
  }

  std::ostringstream output;
  output << "{"
         << "\"status\": "
         << "\"" << stellar::Herder::TX_STATUS_STRING[status] << "\"";

  if (status == stellar::Herder::TX_STATUS_ERROR) {
    const std::string& result = xdr::xdr_to_string(tx_frame->getResult().result.code());
    output << " , \"error\": \"" << result << "\"";
    success = false;
  }

  output << "}";
  result_str = output.str();

  if (success) {
    printf("app is running. You can press ctrl+C to stop it.\n");
    auto& io = aw.clock().getIOService();
    asio::io_service::work mainWork(io);
    while (!io.stopped()) {
        aw.clock().crank();
    }
  }

  return success;
}

static
std::vector<std::string> split_by_regex(const std::string& s, const char* pattern)
{
  std::regex pat(pattern);
  auto s_begin = std::sregex_iterator(s.begin(), s.end(), pat);
  auto s_end = std::sregex_iterator();

  std::vector<std::string> result;
  result.reserve(std::distance(s_begin, s_end));

  for (auto i = s_begin; i != s_end; i++) {
    std::smatch match = *i;
    result.emplace_back(match.str());
  }

  return result;
}

JNIEXPORT jobjectArray JNICALL
Java_com_okcoin_vault_jni_stellar_Stellarj_execute(JNIEnv *env, jclass, jstring networkType, jstring _command)
{
  static struct {
    const char* cmd_name;
    std::function<jobjectArray(const std::vector<std::string>& args)> handler;
  } command_handlers[] = {
    {
      "getaddressbyprivatekey", [env](const std::vector<std::string>& args)->jobjectArray{
        assert(1 == args.size());
        std::string address;
        if (true != GetAddressFromPrivateKey(args[0], address)) {
          address.clear();
        }

        return string2jobjectArray(env, address);
      }
    },
    {
      "createrawtransaction", [env, &networkType](const std::vector<std::string>& args)->jobjectArray {
        assert(4 == args.size());

        std::vector<uint8_t> utx;
        const std::string& net_type = jstring2stdstring(env, networkType);
        if (true != produceUnsignedTx(args[0], args[1], args[2], net_type, args[3].c_str(), utx)) {
          utx.clear();
        }

        return string2jobjectArray(env, bytes2string(utx));
      }
    },
    {
      "signrawtransaction", [env, &networkType](const std::vector<std::string>& args)->jobjectArray {
        assert(3 == args.size());

        std::vector<uint8_t> stx;
        const auto& utx = string2bytes(args[0]);
        const std::string& net_type = jstring2stdstring(env, networkType);
        if (true != signTransaction(utx, args[1], net_type, args[2].c_str(), stx)) {
          stx.clear();
        }

        return string2jobjectArray(env, bytes2string(stx));
      }
    }
  };

  const auto& command = jstring2stdstring(env, _command);
  const auto& cmd_vec = split_by_regex(command, "(\\S+)");
  if (cmd_vec.empty()) {
    return jobjectArray();
  }

  for (auto& h : command_handlers) {
    if (h.cmd_name != cmd_vec[0]) {
      continue;
    }

    return h.handler(std::vector<std::string>(cmd_vec.begin() + 1, cmd_vec.end()));
  }

  return jobjectArray();
}
