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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/cf974b4a/miniXml.o \
	${OBJECTDIR}/_ext/cf974b4a/n2connection.o \
	${OBJECTDIR}/_ext/cf974b4a/n2draw.o \
	${OBJECTDIR}/_ext/cf974b4a/n2drawmanager.o \
	${OBJECTDIR}/_ext/cf974b4a/n2view.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libn2draw.a

../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libn2draw.a: ${OBJECTFILES}
	${MKDIR} -p ../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libn2draw.a
	${AR} -rv ../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libn2draw.a ${OBJECTFILES} 
	$(RANLIB) ../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libn2draw.a

${OBJECTDIR}/_ext/cf974b4a/miniXml.o: ../source/miniXml.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/miniXml.o ../source/miniXml.cpp

${OBJECTDIR}/_ext/cf974b4a/n2connection.o: ../source/n2connection.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2connection.o ../source/n2connection.cpp

${OBJECTDIR}/_ext/cf974b4a/n2draw.o: ../source/n2draw.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2draw.o ../source/n2draw.cpp

${OBJECTDIR}/_ext/cf974b4a/n2drawmanager.o: ../source/n2drawmanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2drawmanager.o ../source/n2drawmanager.cpp

${OBJECTDIR}/_ext/cf974b4a/n2view.o: ../source/n2view.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2view.o ../source/n2view.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libn2draw.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
