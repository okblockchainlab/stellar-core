#!/usr/bin/env bash

if [ -z "$JAVA_HOME" ]; then
	printf "No JAVA_HOME detected! "
	printf "Setup JAVA_HOME before build: export JAVA_HOME=/path/to/java\\n"
	exit 1
fi

if [ -z "$COIN_DEPS" ]; then
  printf "NO COIN_DEPS detected!"
	printf "Setup COIN_DEPS before build: export COIN_DEPS=path/to/depslib\\n"
	exit 1
fi

EXT=so
TARGET_OS=`uname -s`
case "$TARGET_OS" in
    Darwin)
        EXT=dylib
        ;;
    Linux)
        EXT=so
        ;;
    *)
        echo "Unknown platform!" >&2
        exit 1
esac

git submodule init && git submodule update

./autogen.sh
./configure --enable-shared=yes --disable-pie CXXFLAGS="-I${COIN_DEPS}/libpq/include -fPIC"  CFLAGS="-fPIC" libpq_LIBS="-L${COIN_DEPS}/libpq/lib -lpq"
make -j

PROJECT_NAME=stellar-core
mkdir ok-build
cd ok-build
cmake -DCMAKE_BUILD_TYPE=Release -DOKLIBRARY_NAME=${PROJECT_NAME} ../ok-wallet
make -j
cp ./lib${PROJECT_NAME}.${EXT} ../

nm lib${PROJECT_NAME}.${EXT} |grep _Java_com_okcoin
