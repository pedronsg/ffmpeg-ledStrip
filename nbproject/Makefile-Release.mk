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
	${OBJECTDIR}/ledstrip.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/ws2811/dma.o \
	${OBJECTDIR}/ws2811/mailbox.o \
	${OBJECTDIR}/ws2811/pwm.o \
	${OBJECTDIR}/ws2811/rpihw.o \
	${OBJECTDIR}/ws2811/ws2811.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ledstrip

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ledstrip: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ledstrip ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/ledstrip.o: ledstrip.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ledstrip.o ledstrip.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/ws2811/dma.o: ws2811/dma.c
	${MKDIR} -p ${OBJECTDIR}/ws2811
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ws2811/dma.o ws2811/dma.c

${OBJECTDIR}/ws2811/mailbox.o: ws2811/mailbox.c
	${MKDIR} -p ${OBJECTDIR}/ws2811
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ws2811/mailbox.o ws2811/mailbox.c

${OBJECTDIR}/ws2811/pwm.o: ws2811/pwm.c
	${MKDIR} -p ${OBJECTDIR}/ws2811
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ws2811/pwm.o ws2811/pwm.c

${OBJECTDIR}/ws2811/rpihw.o: ws2811/rpihw.c
	${MKDIR} -p ${OBJECTDIR}/ws2811
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ws2811/rpihw.o ws2811/rpihw.c

${OBJECTDIR}/ws2811/ws2811.o: ws2811/ws2811.c
	${MKDIR} -p ${OBJECTDIR}/ws2811
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ws2811/ws2811.o ws2811/ws2811.c

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
