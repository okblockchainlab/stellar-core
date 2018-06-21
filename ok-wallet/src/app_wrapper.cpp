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

void loadConfig(const std::string& net_type, stellar::Config& cfg);

namespace stellar {
int catchupComplete(stellar::Application::pointer app, Json::Value& catchupInfo);
bool checkInitialized(stellar::Application::pointer app);
}


AppWrapper::AppWrapper(const std::string& net_type) : mClock(stellar::VirtualClock::REAL_TIME) {
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

  loadConfig(net_type, mCfg);

  if (mCfg.LOG_FILE_PATH.size()) {
    stellar::Logging::setLoggingToFile(mCfg.LOG_FILE_PATH);
  }

  auto app = stellar::Application::create(mClock, mCfg, false);
  //if (!stellar::checkInitialized(app)) {
    //mApp = stellar::Application::create(clock, cfg, true);
  //}

  if (!app->getHistoryArchiveManager().checkSensibleConfig()) {
    return ;
  }

  //mApp->applyCfgCommands();

  stellar::ExternalQueue ps(*app);
  ps.setInitialCursors(mCfg.KNOWN_CURSORS);
  app->getMaintainer().start();
  //mApp->start();


     //std::this_thread::sleep_for(std::chrono::seconds(10));

  //mApp->gracefulStop();
  //while (mApp->getClock().crank(true)) ;
  //mApp.reset();

  //printf("fuck: app: %x\n", app.get());
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
