#!/bin/bash

rm -rf build

rm -rf install_dir


PWD=`pwd`

STAGING_DIR=$PWD/install_dir

export STAGING_DIR

tools=`dirname $PWD`"/openwrt-toolchain"

cmake   -DCMAKE_TOOLCHAIN_FILE=$tools/cmake-toolschain \
	-DCMAKE_C_COMPILER=$tools/bin/aarch64-openwrt-linux-musl-gcc	\
	-DCMAKE_CXX_COMPILER=$tools/bin/aarch64-openwrt-linux-musl-g++	\
	-DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF  	\
	-DCMAKE_INSTALL_PREFIX=$STAGING_DIR	\
        -DCMAKE_SYSROOT=$tools \
	-DCMAKE_BUILD_TYPE=Release   \
      	-S . -B build



make -C build all install

