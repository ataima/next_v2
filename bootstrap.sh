#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;33m'
WHITE='\033[0;37m'
LRED='\033[1;31m'
LGREEN='\033[1;32m'
LBLUE='\033[1;33m'
LWHITE='\033[1;37m'
LYELLOW='\033[1;34m'


NC='\033[0m'

RES=0

SIZE=30M


#$1 check value
#$2 msg
function error(){
local RES="${1}"
local MSG="${2}"
if [ ${RES} -ne 0 ]
then
        printf "${LRED}Error${LWHITE}:${RES} ${MSG} ${NC}\n"
        exit 1
fi
} 


function info(){
        printf "${LYELLOW}Info${LWHITE}      : ${GREEN} $1 ${NC}\n"
}


function p_yellow(){
        printf "${LBLUE}$1${LWHITE}      : ${GREEN} $2 ${NC}\n"
}


function p_red(){
        printf "${LRED}$1${LWHITE}      : ${GREEN} $2 ${NC}\n"
}


function pita(){
	printf "${LGREEN}$1${LWHITE}$2${LRED}$3${NC}\n"
}


function p_g_w(){
	printf "\t${LGREEN}$1${LWHITE}$2${NC}\n"

}


function prepare(){

PWD=`pwd`

STAGING_DIR=$PWD/install_dir

BUILDDIR=$PWD/build

rm -rf $STAGING_DIR

rm -rf $BUILDDIR

mkdir $STAGING_DIR

mkdir $BUILDDIR

export STAGING_DIR

}


PWD=`pwd`
STAGING_DIR=""
BUILDDIR=""
BMODE=""
VMODE=""
TOOL=`dirname $PWD`"/openwrt-toolchain"
OPENWRT_TOOL="$TOOL/cmake-toolschain"
OPENWRT_SYSROOT="$TOOL"

TOOLSCHAIN=""
SYSROOT=""
INSTALLDIR=""
TOOLCMD=""
MORE=""
function main(){
	
	prepare

	pita "TE" "L" "SY"
	p_yellow "Select build mode :"
	p_g_w    "1)"   "DEBUG           : -> -g O0 -DDEBUG"
	p_g_w    "2)"   "RELEASE         : -> O3 -DNDEBUG"
	p_g_w    "3)"   "MINSIZEREL      : -> Os -DNDEBUG"
	p_g_w    "4)"   "RELWITHDEBINFO  : -> -g 03 -DNDEBUG"
	p_g_w    "*)"   "exit"
	read
	case ${REPLY} in
		1) BMODE="-DCMAKE_BUILD_TYPE=Debug"
			;;
		2) BMODE="-DCMAKE_BUILD_TYPE=Release"
			;;
		3) BMODE="-DCMAKE_BUILD_TYPE=MinSizeRel"
			;;
		4) BMODE="-DCMAKE_BUILD_TYPE=RelWithDebInfo"
			;;
		*) exit	0
	esac	
	p_yellow "Select verbose mode :"
	p_g_w "1)" "ON   : -> verbose mode"
	p_g_w "2)" "OFF  : -> silent mode"
	p_g_w "*)" "exit"
	read
        case ${REPLY} in
                1) VMODE="-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON"
                        ;;
                2) VMODE="-DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF"
                        ;;
                *) exit 0
        esac
	p_yellow "Select toolchain :"
	p_g_w "1)" "x86_64 gcc installed on PC"
	p_g_w "2)" "openwrt toolschain"
	p_g_w "*)" "exit"
	read
	case ${REPLY} in
                1) TOOLSCHAIN=""
		   SYSROOT="" 	
		   p_red "Toolchain" "none" 
			;;
		2) TOOLSCHAIN="-DCMAKE_TOOLCHAIN_FILE=$OPENWRT_TOOL "
		   SYSROOT="-DCMAKE_SYSROOT=$OPENWRT_SYSROOT "	 
		   p_red "Toolchain" "$OPENWRT_TOOL "
			;;
		*) exit 0
	esac
	CC="-DCMAKE_C_COMPILER=$TOOL/bin/aarch64-openwrt-linux-musl-gcc "
	CPP="-DCMAKE_CXX_COMPILER=$TOOL/bin/aarch64-openwrt-linux-musl-g++ "
	INSTALLDIR="-DCMAKE_INSTALL_PREFIX=$STAGING_DIR"
	MORE="-S $PWD  -B $BUILDDIR"
	TOOLCMD="$TOOLSCHAIN $CC $CPP $SYSROOT $INSTALLDIR $BMODE $VMODE $MORE"
	cmake $TOOLCMD
	make -C $BUILDDIR all install
}



main $@
