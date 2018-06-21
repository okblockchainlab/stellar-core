#pragma once

#include <string>


namespace stellar {
  class Cold
  {
  public:
    Cold(const char* seed);

    std::string address();

  private:
    std::string seed_;
  };
}/*namespace stellar*/

extern "C"
__attribute__ ((visibility("default")))
bool GetAddressFromPrivateKey(const std::string& seed, std::string& address);

extern "C"
__attribute__ ((visibility("default")))
bool
signTransaction(const uint8_t* unsigned_tx, const size_t size, const std::string& seed, const std::string& net_type, std::vector<uint8_t>& stx);

//hot
extern "C"
__attribute__ ((visibility("default")))
bool
produceUnsignedTx(const std::string& from, const std::string& to, const std::string& amount, const std::string& net_type, std::vector<uint8_t>& utx);
