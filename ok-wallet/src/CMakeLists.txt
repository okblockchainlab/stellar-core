ExternalProject_Add (proj_stellar_core
	PREFIX ${PROJECT_SOURCE_DIR}/..
	SOURCE_DIR ${PROJECT_SOURCE_DIR}/..
	INSTALL_DIR ${PROJECT_BINARY_DIR}/deps-install
	TMP_DIR ${PROJECT_BINARY_DIR}/deps-tmp/stellar-core
	STAMP_DIR ${PROJECT_BINARY_DIR}/deps-stamp/stellar-core
	CONFIGURE_COMMAND COMMAND <SOURCE_DIR>/autogen.sh COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
	INSTALL_COMMAND ""
	BUILD_IN_SOURCE 1
)
ExternalProject_Get_Property(proj_stellar_core source_dir)
add_library(stellar_core STATIC IMPORTED)
set_target_properties(stellar_core PROPERTIES
	IMPORTED_LOCATION ${source_dir}/src/stellar-core
)
add_dependencies(stellar_core proj_stellar_core)

ExternalProject_Add (proj_xdrpp
	PREFIX ${PROJECT_SOURCE_DIR}/../lib/xdrpp
	SOURCE_DIR ${PROJECT_SOURCE_DIR}/../lib/xdrpp
	INSTALL_DIR ${PROJECT_BINARY_DIR}/deps-install
	TMP_DIR ${PROJECT_BINARY_DIR}/deps-tmp/xdrpp
	STAMP_DIR ${PROJECT_BINARY_DIR}/deps-stamp/xdrpp
	CONFIGURE_COMMAND COMMAND <SOURCE_DIR>/autogen.sh COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
	BUILD_IN_SOURCE 1
)
ExternalProject_Get_Property(proj_xdrpp install_dir)
add_library(xdrpp STATIC IMPORTED)
set_target_properties(xdrpp PROPERTIES
	IMPORTED_LOCATION ${install_dir}/lib/libxdrpp.a
)
add_dependencies(xdrpp proj_xdrpp)

ExternalProject_Add (proj_libsodium
	PREFIX ${PROJECT_SOURCE_DIR}/../lib/libsodium
	SOURCE_DIR ${PROJECT_SOURCE_DIR}/../lib/libsodium
	INSTALL_DIR ${PROJECT_BINARY_DIR}/deps-install
	TMP_DIR ${PROJECT_BINARY_DIR}/deps-tmp/libsodium
	STAMP_DIR ${PROJECT_BINARY_DIR}/deps-stamp/libsodium
	CONFIGURE_COMMAND COMMAND <SOURCE_DIR>/autogen.sh COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
	BUILD_IN_SOURCE 1
)
ExternalProject_Get_Property(proj_libsodium install_dir)
add_library(libsodium STATIC IMPORTED)
set_target_properties(libsodium PROPERTIES
	IMPORTED_LOCATION ${install_dir}/lib/libsodium.a
)
add_dependencies(libsodium proj_libsodium)

################################################################################

set(STELLAR_CORE_SOURCE
	${PROJECT_SOURCE_DIR}/../src/main/StellarCoreVersion.cpp
	${PROJECT_SOURCE_DIR}/../src/bucket/Bucket.cpp
	${PROJECT_SOURCE_DIR}/../src/bucket/BucketApplicator.cpp
	${PROJECT_SOURCE_DIR}/../src/bucket/BucketInputIterator.cpp
	${PROJECT_SOURCE_DIR}/../src/bucket/BucketList.cpp
	${PROJECT_SOURCE_DIR}/../src/bucket/BucketManagerImpl.cpp
	${PROJECT_SOURCE_DIR}/../src/bucket/BucketOutputIterator.cpp
	${PROJECT_SOURCE_DIR}/../src/bucket/BucketTests.cpp
	${PROJECT_SOURCE_DIR}/../src/bucket/FutureBucket.cpp
	${PROJECT_SOURCE_DIR}/../src/bucket/PublishQueueBuckets.cpp
	${PROJECT_SOURCE_DIR}/../src/catchup/ApplyBucketsWork.cpp
	${PROJECT_SOURCE_DIR}/../src/catchup/ApplyLedgerChainWork.cpp
	${PROJECT_SOURCE_DIR}/../src/catchup/CatchupConfiguration.cpp
	${PROJECT_SOURCE_DIR}/../src/catchup/CatchupManagerImpl.cpp
	${PROJECT_SOURCE_DIR}/../src/catchup/CatchupWork.cpp
	${PROJECT_SOURCE_DIR}/../src/catchup/CatchupWorkTests.cpp
	${PROJECT_SOURCE_DIR}/../src/catchup/CatchupWorkTests.cpp
	${PROJECT_SOURCE_DIR}/../src/catchup/DownloadBucketsWork.cpp
	${PROJECT_SOURCE_DIR}/../src/catchup/VerifyLedgerChainWork.cpp
	${PROJECT_SOURCE_DIR}/../src/crypto/CryptoTests.cpp
	${PROJECT_SOURCE_DIR}/../src/crypto/CryptoTests.cpp
	${PROJECT_SOURCE_DIR}/../src/crypto/ECDH.cpp
	${PROJECT_SOURCE_DIR}/../src/crypto/Hex.cpp
	${PROJECT_SOURCE_DIR}/../src/crypto/KeyUtils.cpp
	${PROJECT_SOURCE_DIR}/../src/crypto/Random.cpp
	${PROJECT_SOURCE_DIR}/../src/crypto/SHA.cpp
	${PROJECT_SOURCE_DIR}/../src/crypto/SecretKey.cpp
	${PROJECT_SOURCE_DIR}/../src/crypto/SignerKey.cpp
	${PROJECT_SOURCE_DIR}/../src/crypto/SignerKeyUtils.cpp
	${PROJECT_SOURCE_DIR}/../src/crypto/StrKey.cpp
	${PROJECT_SOURCE_DIR}/../src/database/Database.cpp
	${PROJECT_SOURCE_DIR}/../src/database/DatabaseConnectionString.cpp
	${PROJECT_SOURCE_DIR}/../src/database/DatabaseConnectionStringTest.cpp
	${PROJECT_SOURCE_DIR}/../src/database/DatabaseTests.cpp
	${PROJECT_SOURCE_DIR}/../src/database/DatabaseUtils.cpp
	${PROJECT_SOURCE_DIR}/../src/herder/Herder.cpp
	${PROJECT_SOURCE_DIR}/../src/herder/HerderImpl.cpp
	${PROJECT_SOURCE_DIR}/../src/herder/HerderPersistenceImpl.cpp
	${PROJECT_SOURCE_DIR}/../src/herder/HerderSCPDriver.cpp
	${PROJECT_SOURCE_DIR}/../src/herder/HerderTests.cpp
	${PROJECT_SOURCE_DIR}/../src/herder/HerderUtils.cpp
	${PROJECT_SOURCE_DIR}/../src/herder/LedgerCloseData.cpp
	${PROJECT_SOURCE_DIR}/../src/herder/PendingEnvelopes.cpp
	${PROJECT_SOURCE_DIR}/../src/herder/PendingEnvelopesTests.cpp
	${PROJECT_SOURCE_DIR}/../src/herder/TxSetFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/herder/Upgrades.cpp
	${PROJECT_SOURCE_DIR}/../src/herder/UpgradesTests.cpp
	${PROJECT_SOURCE_DIR}/../src/history/FileTransferInfo.cpp
	${PROJECT_SOURCE_DIR}/../src/history/HistoryArchive.cpp
	${PROJECT_SOURCE_DIR}/../src/history/HistoryArchiveManager.cpp
	${PROJECT_SOURCE_DIR}/../src/history/HistoryManagerImpl.cpp
	${PROJECT_SOURCE_DIR}/../src/history/HistoryTests.cpp
	${PROJECT_SOURCE_DIR}/../src/history/HistoryTestsUtils.cpp
	${PROJECT_SOURCE_DIR}/../src/history/InferredQuorum.cpp
	${PROJECT_SOURCE_DIR}/../src/history/InferredQuorumTests.cpp
	${PROJECT_SOURCE_DIR}/../src/history/SerializeTests.cpp
	${PROJECT_SOURCE_DIR}/../src/history/StateSnapshot.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/BatchDownloadWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/BucketDownloadWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/FetchRecentQsetsWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/GetAndUnzipRemoteFileWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/GetHistoryArchiveStateWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/GetRemoteFileWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/GunzipFileWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/GzipFileWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/MakeRemoteDirWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/Progress.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/PublishWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/PutHistoryArchiveStateWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/PutRemoteFileWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/PutSnapshotFilesWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/RepairMissingBucketsWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/ResolveSnapshotWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/RunCommandWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/VerifyBucketWork.cpp
	${PROJECT_SOURCE_DIR}/../src/historywork/WriteSnapshotWork.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/AccountSubEntriesCountIsValid.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/AccountSubEntriesCountIsValidTests.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/BucketListIsConsistentWithDatabase.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/BucketListIsConsistentWithDatabaseTests.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/CacheIsConsistentWithDatabase.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/CacheIsConsistentWithDatabaseTests.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/ConservationOfLumens.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/ConservationOfLumensTests.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/InvariantDoesNotHold.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/InvariantManagerImpl.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/InvariantTestUtils.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/InvariantTests.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/LedgerEntryIsValid.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/MinimumAccountBalance.cpp
	${PROJECT_SOURCE_DIR}/../src/invariant/MinimumAccountBalanceTests.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/AccountFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/CheckpointRange.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/DataFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/EntryFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/LedgerDelta.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/LedgerDeltaTests.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/LedgerEntryTests.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/LedgerHeaderFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/LedgerHeaderTests.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/LedgerManagerImpl.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/LedgerRange.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/LedgerTestUtils.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/LedgerTests.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/OfferFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/SyncingLedgerChain.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/SyncingLedgerChainTests.cpp
	${PROJECT_SOURCE_DIR}/../src/ledger/TrustFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/main/Application.cpp
	${PROJECT_SOURCE_DIR}/../src/main/ApplicationImpl.cpp
	${PROJECT_SOURCE_DIR}/../src/main/ApplicationTests.cpp
	${PROJECT_SOURCE_DIR}/../src/main/CommandHandler.cpp
	${PROJECT_SOURCE_DIR}/../src/main/Config.cpp
	${PROJECT_SOURCE_DIR}/../src/main/ConfigTests.cpp
	${PROJECT_SOURCE_DIR}/../src/main/ExternalQueue.cpp
	${PROJECT_SOURCE_DIR}/../src/main/ExternalQueueTests.cpp
	${PROJECT_SOURCE_DIR}/../src/main/LruCacheTests.cpp
	${PROJECT_SOURCE_DIR}/../src/main/Maintainer.cpp
	${PROJECT_SOURCE_DIR}/../src/main/NtpSynchronizationChecker.cpp
	${PROJECT_SOURCE_DIR}/../src/main/PersistentState.cpp
	${PROJECT_SOURCE_DIR}/../src/main/dumpxdr.cpp
	${PROJECT_SOURCE_DIR}/../src/main/fuzz.cpp
	${PROJECT_SOURCE_DIR}/../src/main/main.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/BanManagerImpl.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/FloodTests.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/Floodgate.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/ItemFetcher.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/ItemFetcherTests.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/LoadManager.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/LoadManagerTests.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/LoopbackPeer.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/OverlayManagerImpl.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/OverlayManagerTests.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/OverlayTests.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/Peer.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/PeerAuth.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/PeerBareAddress.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/PeerDoor.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/PeerRecord.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/PeerRecordTests.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/PeerSharedKeyId.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/TCPPeer.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/TCPPeerTests.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/Tracker.cpp
	${PROJECT_SOURCE_DIR}/../src/overlay/TrackerTests.cpp
	${PROJECT_SOURCE_DIR}/../src/process/PosixSpawnFileActions.cpp
	${PROJECT_SOURCE_DIR}/../src/process/ProcessManagerImpl.cpp
	${PROJECT_SOURCE_DIR}/../src/process/ProcessTests.cpp
	${PROJECT_SOURCE_DIR}/../src/scp/BallotProtocol.cpp
	${PROJECT_SOURCE_DIR}/../src/scp/LocalNode.cpp
	${PROJECT_SOURCE_DIR}/../src/scp/NominationProtocol.cpp
	${PROJECT_SOURCE_DIR}/../src/scp/QuorumSetTests.cpp
	${PROJECT_SOURCE_DIR}/../src/scp/QuorumSetUtils.cpp
	${PROJECT_SOURCE_DIR}/../src/scp/SCP.cpp
	${PROJECT_SOURCE_DIR}/../src/scp/SCPDriver.cpp
	${PROJECT_SOURCE_DIR}/../src/scp/SCPTests.cpp
	${PROJECT_SOURCE_DIR}/../src/scp/SCPUnitTests.cpp
	${PROJECT_SOURCE_DIR}/../src/scp/Slot.cpp
	${PROJECT_SOURCE_DIR}/../src/simulation/CoreTests.cpp
	${PROJECT_SOURCE_DIR}/../src/simulation/LoadGenerator.cpp
	${PROJECT_SOURCE_DIR}/../src/simulation/Simulation.cpp
	${PROJECT_SOURCE_DIR}/../src/simulation/Topologies.cpp
	${PROJECT_SOURCE_DIR}/../src/test/TestAccount.cpp
	${PROJECT_SOURCE_DIR}/../src/test/TestExceptions.cpp
	${PROJECT_SOURCE_DIR}/../src/test/TestMarket.cpp
	${PROJECT_SOURCE_DIR}/../src/test/TestPrinter.cpp
	${PROJECT_SOURCE_DIR}/../src/test/TestUtils.cpp
	${PROJECT_SOURCE_DIR}/../src/test/TxTests.cpp
	${PROJECT_SOURCE_DIR}/../src/test/test.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/AllowTrustOpFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/AllowTrustTests.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/BumpSequenceOpFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/BumpSequenceTests.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/ChangeTrustOpFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/ChangeTrustTests.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/CreateAccountOpFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/CreatePassiveOfferOpFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/ExchangeTests.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/InflationOpFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/InflationTests.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/ManageDataOpFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/ManageDataTests.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/ManageOfferOpFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/MergeOpFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/MergeTests.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/OfferExchange.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/OfferTests.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/OperationFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/PathPaymentOpFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/PathPaymentTests.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/PaymentOpFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/PaymentTests.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/SetOptionsOpFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/SetOptionsTests.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/SignatureChecker.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/SignatureUtils.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/SignatureUtilsTest.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/TransactionFrame.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/TxEnvelopeTests.cpp
	${PROJECT_SOURCE_DIR}/../src/transactions/TxResultsTests.cpp
	${PROJECT_SOURCE_DIR}/../src/util/BalanceTests.cpp
	${PROJECT_SOURCE_DIR}/../src/util/BigDivideTests.cpp
	${PROJECT_SOURCE_DIR}/../src/util/BitsetEnumerator.cpp
	${PROJECT_SOURCE_DIR}/../src/util/BitsetEnumeratorTests.cpp
	${PROJECT_SOURCE_DIR}/../src/util/DecoderTests.cpp
	${PROJECT_SOURCE_DIR}/../src/util/Fs.cpp
	${PROJECT_SOURCE_DIR}/../src/util/FsTests.cpp
	${PROJECT_SOURCE_DIR}/../src/util/GlobalChecks.cpp
	${PROJECT_SOURCE_DIR}/../src/util/HashOfHash.cpp
	${PROJECT_SOURCE_DIR}/../src/util/Logging.cpp
	${PROJECT_SOURCE_DIR}/../src/util/Math.cpp
	${PROJECT_SOURCE_DIR}/../src/util/MetricResetter.cpp
	${PROJECT_SOURCE_DIR}/../src/util/NtpClient.cpp
	${PROJECT_SOURCE_DIR}/../src/util/NtpWork.cpp
	${PROJECT_SOURCE_DIR}/../src/util/SecretValue.cpp
	${PROJECT_SOURCE_DIR}/../src/util/StatusManager.cpp
	${PROJECT_SOURCE_DIR}/../src/util/StatusManagerTest.cpp
	${PROJECT_SOURCE_DIR}/../src/util/Timer.cpp
	${PROJECT_SOURCE_DIR}/../src/util/TimerTests.cpp
	${PROJECT_SOURCE_DIR}/../src/util/TmpDir.cpp
	${PROJECT_SOURCE_DIR}/../src/util/Uint128Tests.cpp
	${PROJECT_SOURCE_DIR}/../src/util/types.cpp
	${PROJECT_SOURCE_DIR}/../src/work/Work.cpp
	${PROJECT_SOURCE_DIR}/../src/work/WorkManagerImpl.cpp
	${PROJECT_SOURCE_DIR}/../src/work/WorkParent.cpp
	${PROJECT_SOURCE_DIR}/../src/work/WorkTests.cpp
	${PROJECT_SOURCE_DIR}/../lib/sqlite/sqlite3.c
)

set(WALLET_LIB_SOURCE
  wallet.cpp
  app_wrapper.cpp
	config.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/..)
include_directories(${PROJECT_SOURCE_DIR}/../lib)
include_directories(${PROJECT_SOURCE_DIR}/../lib/xdrpp)
include_directories(${PROJECT_SOURCE_DIR}/../src)
include_directories(${PROJECT_SOURCE_DIR}/../lib/asio/asio/include)
include_directories(${PROJECT_SOURCE_DIR}/../lib/cereal/include)
include_directories(${PROJECT_SOURCE_DIR}/../lib/libmedida/src)
include_directories(${PROJECT_SOURCE_DIR}/../lib/soci/src/core)
include_directories(${PROJECT_SOURCE_DIR}/../lib/autocheck/include)
include_directories($ENV{JAVA_HOME}/include)
include_directories($ENV{JAVA_HOME}/include/darwin)

link_directories(../../lib)

add_library(stellar-core SHARED ${STELLAR_CORE_SOURCE} ${WALLET_LIB_SOURCE})
target_link_libraries(stellar-core xdrpp libsodium soci medida 3rdparty pq)
add_dependencies(stellar-core xdrpp libsodium stellar_core)

################################################################################
ExternalProject_Add (proj_gtest
	PREFIX ${PROJECT_SOURCE_DIR}/test/deps/googletest
	SOURCE_DIR ${PROJECT_SOURCE_DIR}/test/deps/googletest
	INSTALL_DIR ${PROJECT_BINARY_DIR}/deps-install
	TMP_DIR ${PROJECT_BINARY_DIR}/deps-tmp/googletest
	STAMP_DIR ${PROJECT_BINARY_DIR}/deps-stamp/googletest
	BINARY_DIR ${PROJECT_BINARY_DIR}/deps-binary/googletest
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)
ExternalProject_Get_Property(proj_gtest source_dir)
ExternalProject_Get_Property(proj_gtest install_dir)
add_library(gtest STATIC IMPORTED)
set_target_properties(gtest PROPERTIES
	IMPORTED_LOCATION ${install_dir}/lib/libgmock_main.a
)
add_dependencies(gtest proj_gtest)

include_directories(${install_dir}/include)
include_directories(${install_dir}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

enable_testing()
include(add_test.cmake)
cxx_test(test_okwallet "" gtest)
