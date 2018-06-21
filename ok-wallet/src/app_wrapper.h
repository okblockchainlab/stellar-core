#pragma once

#include "ledger/AccountFrame.h"
#include "main/ApplicationImpl.h"

class AppWrapper {
public:
  AppWrapper(const std::string& net_type);
  ~AppWrapper();

  bool update_ledger();

  stellar::AccountFrame::pointer loadAccount(const stellar::PublicKey pkey);

private:
  stellar::Config mCfg;
  stellar::VirtualClock mClock;
  stellar::Application::pointer mApp = nullptr;
};
