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

include(add_test.cmake)
cxx_test(test_account "" gtest)
