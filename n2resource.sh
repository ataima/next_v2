#!/bin/bash
#Colors
WHITE=" -e \E[1;37m"
GRAY_LIGHT=" -e \E[0;37m"
GRAY_DARK=" -e \E[1;30m"
BLUE=" -e \E[0;34m"
BLUE_LIGHT=" -e \E[1;34m"
GREEN=" -e \E[0;32m"
GREEN_LIGHT=" -e \E[1;32m"
CYAN=" -e \E[0;36m"
CYAN_LIGHT=" -e \E[1;36m"
RED=" -e \E[0;31m"
RED_LIGHT=" -e \E[1;31m"
PURPLE=" -e \E[0;35m"
PURPLE_LIGHT=" -e \E[1;35m"
BROWN=" -e \E[0;33m"
YELLOW=" -e \E[1;33m"
BLACK=" -e \E[0;30m"
REPLACE=" -e \E[0m"




#PUBLIC FUNCTION EXPORTED TO ALL GENERATED SCRIPTS 
#$1 color
#$2   string
function print_c(){ #EX
echo -e $(echo $1) "$2" $(echo $3) "$4"$(echo $5) "$6" $(echo $REPLACE)
} #EX


# $1..3 argv
function print_s_ita(){ #EX
local A1=$(printf "%-20s" "$1")
local A2=$(printf "%-20s" "$2")
local A3=$(printf "%-20s" "$3")
echo  $(echo $GREEN_LIGHT) "$A1" $(echo $WHITE) "$A2" $(echo $RED_LIGHT) "$A3" $(echo $REPLACE)
} #EX




#none
function print_del_ita(){ #EX
print_s_ita "--------------------" "--------------------" "--------------------"
} #EX

# $1..3 argv
function print_ita(){ #EX
print_del_ita
print_s_ita "$1" "$2" "$3"
} #EX

#$1   string
#$2   string
#$3   string
function error_c(){ #EX
print_del_ita
print_c "$RED_LIGHT" "ERROR : " "$BLUE_LIGHT" " - $1" 
if [ "$2" ]; then
	print_c "$WHITE" "	> " "$YELLOW" "- $2" 
	if [ "$3" ]; then
		print_c "$WHITE" "	> " "$YELLOW" "- $3" 
		if [ "$4" ]; then
			print_c "$WHITE" "	> " "$YELLOW" "- $4" 
			if [ "$5" ]; then
				print_c "$WHITE" "	> " "$YELLOW" "- $5" 
				if [ "$6" ]; then
					print_c "$WHITE" "	> " "$YELLOW" "- $6" 
				fi
			fi	
		fi
	fi
fi
print_c "$GRAY_DARK" "PATH  :" "$GRAY_LIGHT" $(pwd)
print_c "$GRAY_DARK" "FILE  :" "$GRAY_LIGHT" "$0"
print_c "$GRAY_DARK" "LINE  :" "$GRAY_LIGHT" "${BASH_LINENO[0]}"
print_del_ita
exit 1
} #EX



#$1   string
#$2   string
#$3   string
function warning_c(){ #EX
print_del_ita
print_c "$GREEN_LIGHT" "WARNING : " "$BLUE_LIGHT" "- $1" 
if [ "$2" ]; then
	print_c "$WHITE" "	 >"  "$YELLOW" " - $2" 
	if [ "$3" ]; then
		print_c "$WHITE" "	 >"  "$YELLOW" " - $3" 
		if [ "$4" ]; then
			print_c "$WHITE" "	 >"  "$YELLOW" " - $4" 
			if [ "$5" ]; then
				print_c "$WHITE" "	 >"  "$YELLOW" " - $5" 
				if [ "$6" ]; then
					print_c "$WHITE" "	 >"  "$YELLOW" " - $6" 
				fi
			fi
		fi
	fi
fi
print_del_ita
} #EX



#$1 full filename
function check_exist(){ #EX
if  [ -e "$1" ] && [ -f "$1" ] ; then 
	return 0
fi
return 1
} #EX

#$1 path
function recurs_dir(){
	local DIR=$(ls "$1")
	local VV=""
	for VV in $DIR; do
		[ "$1/$VV" == "."   ] && continue
		[ "$1/$VV" == ".." ] && continue
		[ -d "$1/$VV" ] && recurs_dir "$1/$VV"
		[ -f "$1/$VV"  ] && echo -n " $1/$VV"
	done
}


# $1 array of fullpath files
# $2 output file..
function create_c_function(){
	local VV=""
	local NAME=""
        echo "//file autogenerate from script file linkresource.sh" >$2
        echo "//do not change. Any changes will be lost at next regeneration" >>$2
        echo "#include \"n2resource.h\"" >>$2
	echo "#include <map>" >>$2
	echo "" >>$2
	echo "" >>$2
	echo "" >>$2
	echo "// map filename to array file" >>$2
	echo "typedef struct resource_tag{" >>$2
	echo "    const unsigned char *resource;">>$2
	echo "    size_t size;" >>$2
        echo "    void set(const unsigned char *_resource,size_t _size){" >>$2
        echo "    		resource=_resource;size=_size;}" >>$2
        echo "} memresource,*ptrmemresource;" >>$2
	echo "" >>$2
	echo "" >>$2
	echo "" >>$2
        echo "typedef std::map<std::string,memresource >  ResourceFile;" >>$2
	echo "" >>$2
	echo "" >>$2
	echo "" >>$2
        echo "static ResourceFile appResource;" >>$2
        echo "" >>$2
        echo "" >>$2
        echo "" >>$2
        for VV in $1; do
                NAME=$(echo resource_$VV | sed -e 's#/#_#g' | sed -e 's/\./_/g')
                echo "//declare image of file $VV to array $NAME">>$2
                echo "//size =  $(wc $VV)">>$2
                echo "const unsigned char $NAME[] __attribute__ ((aligned (32)))={">>$2
                cat $VV | xxd -i  >> $2
                echo "};">>$2
                echo "" >>$2
                echo "" >>$2
                echo "" >>$2
        done
        echo "int nnResource::Get(const char *file,const unsigned char ** ptr,size_t *size){" >>$2
        echo "int res=-1;" >>$2
        echo "std::string s=file;" >>$2
        echo "std::map<std::string,memresource >::iterator it=appResource.find(s);"  >>$2
        echo "if( it!=appResource.end()) {"  >>$2
        echo "	*ptr=it->second.resource;"  >>$2
        echo "	*size=it->second.size;"  >>$2
        echo "	res=0;"  >>$2
        echo "	}"  >>$2
        echo "return res;"  >>$2
        echo "}" >>$2
        echo "" >>$2
        echo "" >>$2
        echo "" >>$2
        echo "void nnResource::Init(void){" >>$2
	echo "memresource res;" >>$2
        echo "std::string name;" >>$2
	for VV in $1; do
		NAME=$(echo resource_$VV | sed -e 's#/#_#g' | sed -e 's/\./_/g')
                echo "name=\"$VV\";">>$2
                echo "res.set($NAME,sizeof($NAME));">>$2
                echo "appResource[name]=res;">>$2
	done
	echo "}" >>$2
        echo "" >>$2
        echo "" >>$2
        echo "" >>$2
}




# input resource path
function main(){
local RES=0
local ALL_FILES
	if [ $# -ne 2 ]; then 
		error_c "usage : " "./n2resource <resource path> outfile.c" 
	fi
	if [ ! -e "$1" ]; then 
		error_c "input $1" "not exist" 
	fi
	if [ ! -d "$1" ]; then 
		error_c "input $1" "not is a directory" 
	fi
	if [ "$2" ] && [ -e "$2" ]; then 
		mv "$2" "$2_$(date +%s)"
		rm -f  "$2"
	fi	
	ALL_FILES=$(recurs_dir "$1") 
	create_c_function "$ALL_FILES" "$2"
	RES=$?	
        chmod -w "$2"
	exit $RES	
}



if [ $# -eq 0 ]; then
	main resource n2draw/source/n2resource.cpp
else
	main $@
fi
