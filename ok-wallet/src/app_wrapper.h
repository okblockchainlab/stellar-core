#pragma once

#include "ledger/AccountFrame.h"
#include "main/ApplicationImpl.h"

class AppWrapper {
public:
  AppWrapper();
  ~AppWrapper();

  bool update_ledger();

  stellar::AccountFrame::pointer loadAccount(const stellar::PublicKey pkey);

private:
  stellar::Config mCfg;
  stellar::VirtualClock mClock;
  stellar::Application::pointer mApp = nullptr;
};
