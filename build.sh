#! /bin/sh
PWD=`pwd`
LIBDOWNLOAD=$PWD/libdownload

if [ -z "$COIN_DEPS" ]; then
	printf "No COIN_DEPS detected!\\n"
	printf "Setup COIN_DEPS before build: export COIN_DEPS=`pwd`/depslib \\n"
	exit 1
fi

if [ ! -d "$LIBDOWNLOAD" ];then
	mkdir $LIBDOWNLOAD
fi

if [ ! -d "$COIN_DEPS" ];then
	mkdir $COIN_DEPS
fi

cd $COIN_DEPS

if [ ! -d ./libpq ];then
	mkdir libpq
fi

###install libpq
cd $LIBDOWNLOAD

if [ ! -f ./postgresql-10.4.tar.gz ];then
	wget https://ftp.postgresql.org/pub/source/v10.4/postgresql-10.4.tar.gz
	if [ ! -f ./postgresql-10.4.tar.gz ];then
		echo "Error cannot download postgresql-10.4.tar.gz" >&2
		exit 1
	fi
fi

if [ ! -d ./postgresql-10.4 ];then
	tar xf postgresql-10.4.tar.gz
	if [ ! -d postgresql-10.4 ];then
		echo "Error cannot tar xf postgresql-10.4.tar.gz"
		exit 1
	fi
fi

cd postgresql-10.4
./configure --prefix=$COIN_DEPS/libpq CFLAGS="-g -O2 -fPIC" CPPFLAGS="-g -O2 -fPIC"
make && make install
