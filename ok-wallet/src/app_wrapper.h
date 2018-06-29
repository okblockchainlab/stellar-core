#pragma once

#include "ledger/AccountFrame.h"
#include "main/ApplicationImpl.h"

class AppWrapper {
public:
  AppWrapper(const std::string& net_type, const char* data_dir, bool start = false);
  ~AppWrapper();

  stellar::Application::pointer app();
  stellar::VirtualClock& clock() {return mClock;}

  bool update_ledger();

  stellar::AccountFrame::pointer loadAccount(const stellar::PublicKey pkey);

private:
  stellar::Config mCfg;
  stellar::VirtualClock mClock;
  stellar::Application::pointer mApp = nullptr;
};
