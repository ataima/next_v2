#!/bin/sh

PATH=$(pwd);


if [ "$1" != "" ]
then 
	PATH=$(/usr/bin/dirname $1)
fi

/usr/bin/astyle -n --quiet --style=ansi $PATH/inc/*.h
/usr/bin/astyle -n --quiet --style=ansi $PATH/src/*.c
/usr/bin/astyle -n --quiet --style=ansi $PATH/src/*.cpp
