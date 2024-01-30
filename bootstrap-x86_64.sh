#!/bin/bash

rm -rf build
rm -rf install_dir

PWD=`pwd`

STAGING_DIR=$PWD/install_dir

cmake   -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON        \
        -DCMAKE_INSTALL_PREFIX=$STAGING_DIR     \
        -DCMAKE_BUILD_TYPE=Debug \
        -S . -B build


make -C build all install 
