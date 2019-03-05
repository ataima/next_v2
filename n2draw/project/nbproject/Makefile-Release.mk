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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/cf974b4a/n2appmanager.o \
	${OBJECTDIR}/_ext/cf974b4a/n2caption.o \
	${OBJECTDIR}/_ext/cf974b4a/n2capturepos.o \
	${OBJECTDIR}/_ext/cf974b4a/n2childapp.o \
	${OBJECTDIR}/_ext/cf974b4a/n2commander.o \
	${OBJECTDIR}/_ext/cf974b4a/n2connection.o \
	${OBJECTDIR}/_ext/cf974b4a/n2draw.o \
	${OBJECTDIR}/_ext/cf974b4a/n2drawmanager.o \
	${OBJECTDIR}/_ext/cf974b4a/n2exthandler.o \
	${OBJECTDIR}/_ext/cf974b4a/n2fontlist.o \
	${OBJECTDIR}/_ext/cf974b4a/n2fontmanager.o \
	${OBJECTDIR}/_ext/cf974b4a/n2iface.o \
	${OBJECTDIR}/_ext/cf974b4a/n2imagemanager.o \
	${OBJECTDIR}/_ext/cf974b4a/n2logger.o \
	${OBJECTDIR}/_ext/cf974b4a/n2miniXml.o \
	${OBJECTDIR}/_ext/cf974b4a/n2moreinfo.o \
	${OBJECTDIR}/_ext/cf974b4a/n2resource.o \
	${OBJECTDIR}/_ext/cf974b4a/n2scroller.o \
	${OBJECTDIR}/_ext/cf974b4a/n2selector.o \
	${OBJECTDIR}/_ext/cf974b4a/n2toolview.o \
	${OBJECTDIR}/_ext/cf974b4a/n2utils.o \
	${OBJECTDIR}/_ext/cf974b4a/n2view.o \
	${OBJECTDIR}/_ext/cf974b4a/n2viewglue.o


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

${OBJECTDIR}/_ext/cf974b4a/n2appmanager.o: ../source/n2appmanager.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2appmanager.o ../source/n2appmanager.cpp

${OBJECTDIR}/_ext/cf974b4a/n2caption.o: ../source/n2caption.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2caption.o ../source/n2caption.cpp

${OBJECTDIR}/_ext/cf974b4a/n2capturepos.o: ../source/n2capturepos.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2capturepos.o ../source/n2capturepos.cpp

${OBJECTDIR}/_ext/cf974b4a/n2childapp.o: ../source/n2childapp.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2childapp.o ../source/n2childapp.cpp

${OBJECTDIR}/_ext/cf974b4a/n2commander.o: ../source/n2commander.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2commander.o ../source/n2commander.cpp

${OBJECTDIR}/_ext/cf974b4a/n2connection.o: ../source/n2connection.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2connection.o ../source/n2connection.cpp

${OBJECTDIR}/_ext/cf974b4a/n2draw.o: ../source/n2draw.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2draw.o ../source/n2draw.cpp

${OBJECTDIR}/_ext/cf974b4a/n2drawmanager.o: ../source/n2drawmanager.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2drawmanager.o ../source/n2drawmanager.cpp

${OBJECTDIR}/_ext/cf974b4a/n2exthandler.o: ../source/n2exthandler.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2exthandler.o ../source/n2exthandler.cpp

${OBJECTDIR}/_ext/cf974b4a/n2fontlist.o: ../source/n2fontlist.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2fontlist.o ../source/n2fontlist.cpp

${OBJECTDIR}/_ext/cf974b4a/n2fontmanager.o: ../source/n2fontmanager.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2fontmanager.o ../source/n2fontmanager.cpp

${OBJECTDIR}/_ext/cf974b4a/n2iface.o: ../source/n2iface.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2iface.o ../source/n2iface.cpp

${OBJECTDIR}/_ext/cf974b4a/n2imagemanager.o: ../source/n2imagemanager.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2imagemanager.o ../source/n2imagemanager.cpp

${OBJECTDIR}/_ext/cf974b4a/n2logger.o: ../source/n2logger.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2logger.o ../source/n2logger.cpp

${OBJECTDIR}/_ext/cf974b4a/n2miniXml.o: ../source/n2miniXml.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2miniXml.o ../source/n2miniXml.cpp

${OBJECTDIR}/_ext/cf974b4a/n2moreinfo.o: ../source/n2moreinfo.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2moreinfo.o ../source/n2moreinfo.cpp

${OBJECTDIR}/_ext/cf974b4a/n2resource.o: ../source/n2resource.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2resource.o ../source/n2resource.cpp

${OBJECTDIR}/_ext/cf974b4a/n2scroller.o: ../source/n2scroller.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2scroller.o ../source/n2scroller.cpp

${OBJECTDIR}/_ext/cf974b4a/n2selector.o: ../source/n2selector.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2selector.o ../source/n2selector.cpp

${OBJECTDIR}/_ext/cf974b4a/n2toolview.o: ../source/n2toolview.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2toolview.o ../source/n2toolview.cpp

${OBJECTDIR}/_ext/cf974b4a/n2utils.o: ../source/n2utils.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2utils.o ../source/n2utils.cpp

${OBJECTDIR}/_ext/cf974b4a/n2view.o: ../source/n2view.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2view.o ../source/n2view.cpp

${OBJECTDIR}/_ext/cf974b4a/n2viewglue.o: ../source/n2viewglue.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../header -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/n2viewglue.o ../source/n2viewglue.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
