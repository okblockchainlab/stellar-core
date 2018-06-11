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
${PROJECT_SOURCE_DIR}/../src/crypto/SecretKey.cpp
${PROJECT_SOURCE_DIR}/../src/crypto/Hex.cpp
${PROJECT_SOURCE_DIR}/../src/crypto/SHA.cpp
${PROJECT_SOURCE_DIR}/../src/crypto/StrKey.cpp
${PROJECT_SOURCE_DIR}/../lib/util/crc16.cpp
${PROJECT_SOURCE_DIR}/../src/crypto/KeyUtils.cpp
${PROJECT_SOURCE_DIR}/../src/util/HashOfHash.cpp
)

set(WALLET_LIB_SOURCE
  wallet.cpp
  implents.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/..)
include_directories(${PROJECT_SOURCE_DIR}/../lib)
include_directories(${PROJECT_SOURCE_DIR}/../src)
include_directories(${PROJECT_SOURCE_DIR}/../lib/asio/asio/include)
include_directories(${PROJECT_SOURCE_DIR}/../lib/cereal/include)


add_library(stellar-wlt MODULE ${STELLAR_CORE_SOURCE} ${WALLET_LIB_SOURCE})
target_link_libraries(stellar-wlt xdrpp libsodium)
add_dependencies(stellar-wlt xdrpp libsodium stellar_core)
