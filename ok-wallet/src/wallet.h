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

__attribute__ ((visibility("default")))
std::vector<uint8_t>
signTransaction(const uint8_t* unsigned_tx, const size_t size);

//hot
extern "C"
__attribute__ ((visibility("default")))
std::vector<uint8_t>
produceUnsignedTx(const std::string& from, const std::string& to, const std::string& amount);
