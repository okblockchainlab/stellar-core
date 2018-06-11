#include "wallet.h"
#include "crypto/SecretKey.h"

namespace stellar {
  Hot::Hot(const char* seed): seed_(seed) {
  }

  std::string Hot::address() {
    const ByteSlice bs(seed_);
    auto sec_key = SecretKey::fromSeed(bs);
    return sec_key.getStrKeyPublic();
  }

}/*namespace stellar*/
