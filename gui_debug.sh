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
local MSG="${1}"
printf "${LRED}Error${LWHITE}      :  ${MSG} ${NC}\n"
exit 1
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

pita "TE" "L" "SY"
echo "-->>$#"

if [ "$1" == "" ] || [ $# -lt 2 ]
then
	p_red "usage" "./$0 <exec file> <ip-pillow>  <args>"
	error  "Missing args"
fi

PWD=`pwd`

TOOLSCHAIN=`dirname $PWD`/openwrt-toolchain


TARGET=$2

info "TARGET ON $TARGET"
info "GDB = aarch64-openwrt-linux-musl-gdb"

rm -rf gdbconf

echo "target extended-remote $TARGET:3333" > gdbconf
echo "set remote exec-file $PWD/install_dir/$1" >> gdbconf
if [ "$3$4$5$6" != "" ]
then	
echo "set args $3 $4 $5 $6" >> gdbconf
fi
echo "load" >> gdbconf
#echo "set br main" >> gdbconf
#echo "run" >> gdbconf

GUI=`which gdbgui`
if [ "$GUI" == "" ]
then
	error 1 "non gdbgui installed : do pip --install gdbgui <require python> "
fi	


gdbgui -g "$TOOLSCHAIN/bin/aarch64-openwrt-linux-musl-gdb -x gdbconf" $PWD/install_dir/$1

