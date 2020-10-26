#!/bin/sh

function usage(){
echo "./prepare.sh width< 32-64> \nos <linux windows imx6 imx8> \nmode < Debug release> "
exit 1
}


if [ "$1" == "" ]
then 
	usage
fi


if [ $1 -ne 32 ] && [ $1 -ne 64 ] 
then 
	usage
fi 



if [ "$2" == "" ]
then
	usage
fi

ENVIRON=$(echo $2 | tr '[:upper:]' '[:lower:]' )

if [ "$ENVIRON" != "linux" ] && 
	[ "$ENVIRON" != "windows" ] &&
	[ "$ENVIRON" != "imx6" ]    &&
	[ "$ENVIRON" != "imx8" ] 
then 
	usage
fi


if [ "$3" == "" ]
then
	usage
fi  

if [ "$3" != "Debug" ] && [ "$3" != "Release" ]
then 
	usage
fi


MODE=$3

MACHINE=""

if [ "$ENVIRON" == "linux" ] || 
	[ "$ENVIRON" == "window" ]
then
	MACHINE="i686"
fi

if [ "$ENVIRON" == "imx6" ] 
then
        MACHINE="imx6"
	ENVIRON="linux"
fi


if [ "$ENVIRON" == "imx8" ] 
then
        MACHINE="imx8"
        ENVIRON="linux"
fi

BUILDPATH=$MODE-$ENVIRON-$1-$MACHINE
DISTPATH=$(pwd)/dist/$BUILDPATH

mkdir -p $BUILDPATH
mkdir $DISTPATH

cd $BUILDPATH
TOOLSCHAIN="../template.cmake/"$1"_"$ENVIRON"_"$MACHINE".toolchain.cmake"

cmake -DCMAKE_INSTALL_PREFIX=$DISTPATH -DCMAKE_BUILD_TYPE=$MODE  -DCMAKE_TOOLCHAIN_FILE=$TOOLSCHAIN  ..
make -j8
make install
