#include "app_wrapper.h"
#include "history/HistoryArchiveManager.h"
#include "test/TxTests.h"
#include "util/Logging.h"
#include "main/Config.h"
#include "util/Timer.h"
#include "database/Database.h"
#include "overlay/StellarXDR.h"
#include "main/ExternalQueue.h"
#include "main/Maintainer.h"

void loadConfig(const std::string& net_type, const char* data_dir, bool listen, stellar::Config& cfg);

namespace stellar {
int catchupComplete(stellar::Application::pointer app, Json::Value& catchupInfo);
bool checkInitialized(stellar::Application::pointer app);
}


AppWrapper::AppWrapper(const std::string& net_type, const char* data_dir, bool start) : mClock(stellar::VirtualClock::REAL_TIME) {
  stellar::Logging::init();
  // yes you really have to do this 3 times
  stellar::Logging::setLogLevel(el::Level::Error, nullptr);
  stellar::Logging::setLogLevel(el::Level::Error, nullptr);
  stellar::Logging::setLogLevel(el::Level::Error, nullptr);

  if (sodium_init() < 0)
  {
    LOG(FATAL) << "Could not initialize crypto";
    return ;
  }

  loadConfig(net_type, data_dir, start ? true : false, mCfg);

  if (mCfg.LOG_FILE_PATH.size()) {
    stellar::Logging::setLoggingToFile(mCfg.LOG_FILE_PATH);
  }

  bool newdb = false;
  const auto& init_ffile = data_dir + std::string("/.") + net_type + "_db_initialized";
  if (access(init_ffile.c_str(), R_OK) < 0) {
    newdb = true;
  }

  auto app = stellar::Application::create(mClock, mCfg, newdb);
  assert(true == stellar::checkInitialized(app));

  if (!app->getHistoryArchiveManager().checkSensibleConfig()) {
    return ;
  }

  if (start) {
    app->applyCfgCommands();
    app->start();
  }
  else {
    stellar::ExternalQueue ps(*app);
    ps.setInitialCursors(mCfg.KNOWN_CURSORS);
    app->getMaintainer().start();
  }

  if (newdb) {
    fopen(init_ffile.c_str(), "w");
  }

  mApp = std::move(app);
}

AppWrapper::~AppWrapper() {
  if (nullptr == mApp) {
    return;
  }
  mApp->gracefulStop();
  while (mApp->getClock().crank(true))
      ;
  mApp.reset();
}

stellar::Application::pointer AppWrapper::app()
{
  return mApp;
}

bool AppWrapper::update_ledger() {
  if (nullptr == mApp) {
    return false;
  }

  Json::Value catchupInfo;
  return 0 == stellar::catchupComplete(mApp, catchupInfo) ? true : false;
}

stellar::AccountFrame::pointer
AppWrapper::loadAccount(const stellar::PublicKey pkey) {
  if (nullptr == mApp) {
    return nullptr;
  }
  return stellar::txtest::loadAccount(pkey, *mApp, false);
}
