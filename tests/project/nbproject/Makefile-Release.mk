#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/812168374/mainApp.o \
	${OBJECTDIR}/_ext/812168374/testn2connection.o \
	${OBJECTDIR}/_ext/812168374/testn2draw.o \
	${OBJECTDIR}/_ext/812168374/testn2objmanager.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM} -ln2draw

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tests.exe

../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tests.exe: ${OBJECTFILES}
	${MKDIR} -p ../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tests ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/812168374/mainApp.o: ../source/mainApp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -I../../n2draw/header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/mainApp.o ../source/mainApp.cpp

${OBJECTDIR}/_ext/812168374/testn2connection.o: ../source/testn2connection.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -I../../n2draw/header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/testn2connection.o ../source/testn2connection.cpp

${OBJECTDIR}/_ext/812168374/testn2draw.o: ../source/testn2draw.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -I../../n2draw/header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/testn2draw.o ../source/testn2draw.cpp

${OBJECTDIR}/_ext/812168374/testn2objmanager.o: ../source/testn2objmanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -I../../n2draw/header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/testn2objmanager.o ../source/testn2objmanager.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tests.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
