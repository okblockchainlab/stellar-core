#pragma once

#include <string>
#include <vector>


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
produceUnsignedTx(const std::string& from, const std::string& to, const std::string& amount, const std::string& fee, const std::string& seqNum, std::vector<uint8_t>& utx);

extern "C"
__attribute__ ((visibility("default")))
bool
signTransaction(const std::vector<uint8_t>& utx, const std::string& seed, const std::string& net_type, std::vector<uint8_t>& stx);

extern "C"
__attribute__ ((visibility("default")))
bool commitTransaction(const std::vector<uint8_t>& stx, const std::string& net_type, const char* data_dir, std::string& result_str);
