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

pita "TE" "L" "SY"

if [ "$1" == "" ] || [ $# -ne 3 ]
then
	p_red "usage" "./$0 <exec file> <ip-pillow> <ip-host>"
	error   1 "Missing program to debug"
fi

TARGET=$2
HOST=$3

info "pillow at ip   : $TARGET"
info "pc     at ip   : $HOST"
PWD=`pwd`
ssh root@${TARGET} "ls $PWD" > /dev/null
RES=$?
if [ $RES -ne 0 ]
then	
ssh-keygen -f "/home/angelo/.ssh/known_hosts" -R ${TARGET}
fi
ssh root@${TARGET} "ls $PWD" > /dev/null

ssh root@${TARGET} "rm -rf $PWD"
RES=$?
error $RES "on run ssh remote shell command"

rem_dir=`echo $PWD | sed 's@/@  @g'`
pp=""
rdir="/"
for pp in $rem_dir 
do
	rdir="$rdir$pp"
	ssh root@${TARGET} "mkdir -p $rdir"
	rdir="$rdir/"
done

ssh root@$TARGET "killall  gdbserver "

info "copy current source to remote pillow@$TARGET"
rsync -azP --exclude 'build'  * root@${TARGET}:$PWD > /dev/null 2>&1 
RES=$?
error $RES "on run  rsync shell command"


info "start remote gdbserver"

ssh root@$TARGET "gdbserver  ${HOST}:3333  $PWD/install_dir/$1 "
RES=$?
error $RES "on run ssh remote shell command"

ssh root@$TARGET "killall  gdbserver "


