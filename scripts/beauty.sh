#!/bin/sh

PATH=$(pwd);

if [ "$1" != "" ]
then 
	PATH=$1
fi

/usr/bin/astyle -n --quiet --style=ansi $PATH/src/*.cpp
/usr/bin/astyle -n --quiet --style=ansi $PATH/inc/*.h
