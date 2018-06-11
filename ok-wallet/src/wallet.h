#pragma once

#include <string>


namespace stellar {
  class Hot
  {
  public:
    Hot(const char* seed);

    std::string address();

  private:
    std::string seed_;
  };
}/*namespace stellar*/
