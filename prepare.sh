#!/bin/sh

function usage(){
echo "./prepare.sh width< 32 -64> os <Linux windows> mode < Debug release> "
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



if [ "$3" == "" ]
then
	usage
fi  


if [ "$3" != "Debug" ] && [ "$3" != "Release" ]
then 
	usage
fi

ENVIRON=$(echo $2 | tr '[:upper:]' '[:lower:]' )
echo "---> $ENVIRON"
#$3 prefix ex arm-none-eabi-

MODE=$3


mkdir -p $MODE$ENVIRON$1

cd $MODE$ENVIRON$1

BUILDID="../"$1"_"$ENVIRON"_i686.toolchain.cmake"
echo "--> $BUILDID"

cmake -DCMAKE_INSTALL_PREFIX=$(pwd)/../dist/$MODE -DCMAKE_BUILD_TYPE=$MODE  -DCMAKE_TOOLCHAIN_FILE=$BUILDID  ..
make -j8
make install
