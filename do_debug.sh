
if [ "$1" == "" ]
then 
	echo "usage ./do_debug 32 or 64 ...."
	exit 1
fi


if [ $1 -ne 32 ] && [ $1 -ne 64 ] 
then 
	 echo "usage ./do_debug 32 or 64 ...."
        exit 1
fi 



if [ "$2" == "" ]
then
        echo "usage ./do_debug 32 or 64 Linux or Windows ...."
        exit 1
fi

ENVIRON=$(echo $2 | tr '[:upper:]' '[:lower:]' )
echo "---> $ENVIRON"
#$3 prefix ex arm-none-eabi-

MODE=Debug


mkdir -p $MODE$ENVIRON$1

cd $MODE$ENVIRON$1

BUILDID="../"$1"_"$ENVIRON"_i686.toolchain.cmake"
echo "--> $BUILDID"

cmake -DCMAKE_BUILD_TYPE=$MODE  -DCMAKE_TOOLCHAIN_FILE=$BUILDID  ..
make -j8
make install
