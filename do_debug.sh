
if [ "$1" == "" ]
then 
	echo "usage ./do_debug 32 or 64 ...."
	exit 1
fi

if [ "$2" == "" ]
then
        echo "usage ./do_debug 32 or 64 Linux or Windows ...."
        exit 1
fi

#$3 prefix ex arm-none-eabi-


mkdir Debug$2$1
cd Debug$2$1
cmake -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_TOOLCHAIN_FILE=../$1_$2_i686.toolchain.cmake   ..
make -j8

