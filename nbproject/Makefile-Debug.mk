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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/AuthenticatedNode.o \
	${OBJECTDIR}/AuthenticatedSkipList.o \
	${OBJECTDIR}/CashException.o \
	${OBJECTDIR}/Ciphertext.o \
	${OBJECTDIR}/CommonFunctions.o \
	${OBJECTDIR}/DPDPFlexProver.o \
	${OBJECTDIR}/DPDPFlexVerifier.o \
	${OBJECTDIR}/Data.o \
	${OBJECTDIR}/FourSquares.o \
	${OBJECTDIR}/Group.o \
	${OBJECTDIR}/GroupRSA.o \
	${OBJECTDIR}/Hash.o \
	${OBJECTDIR}/Key.o \
	${OBJECTDIR}/Merkle.o \
	${OBJECTDIR}/MultiExp.o \
	${OBJECTDIR}/NTL/ZZ.o \
	${OBJECTDIR}/Node.o \
	${OBJECTDIR}/ProofNode.o \
	${OBJECTDIR}/RankBasedNode.o \
	${OBJECTDIR}/RankBasedSkipList.o \
	${OBJECTDIR}/Serialize.o \
	${OBJECTDIR}/SkipList.o \
	${OBJECTDIR}/Timer.o \
	${OBJECTDIR}/base64.o \
	${OBJECTDIR}/build_challenge_prove_verify.o \
	${OBJECTDIR}/build_flexlist.o \
	${OBJECTDIR}/check_daemon_existence.o \
	${OBJECTDIR}/create_daemon.o \
	${OBJECTDIR}/get_mount_location.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/mdstat_entry.o \
	${OBJECTDIR}/read_mdstat.o \
	${OBJECTDIR}/watch.o


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
LDLIBSOPTIONS=-lboost_serialization -lboost_thread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/project

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/project: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/project ${OBJECTFILES} ${LDLIBSOPTIONS} -lboost_serialization -lboost_thread -lboost_thread-mt -lboost_serialization-mt -lboost_iostreams-mt -lboost_system -lantlr -lgmpxx -lgmp -lm -lssl -lcrypto -ldl

${OBJECTDIR}/AuthenticatedNode.o: AuthenticatedNode.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AuthenticatedNode.o AuthenticatedNode.cpp

${OBJECTDIR}/AuthenticatedSkipList.o: AuthenticatedSkipList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AuthenticatedSkipList.o AuthenticatedSkipList.cpp

${OBJECTDIR}/CashException.o: CashException.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CashException.o CashException.cpp

${OBJECTDIR}/Ciphertext.o: Ciphertext.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Ciphertext.o Ciphertext.cpp

${OBJECTDIR}/CommonFunctions.o: CommonFunctions.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CommonFunctions.o CommonFunctions.cpp

${OBJECTDIR}/DPDPFlexProver.o: DPDPFlexProver.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DPDPFlexProver.o DPDPFlexProver.cpp

${OBJECTDIR}/DPDPFlexVerifier.o: DPDPFlexVerifier.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DPDPFlexVerifier.o DPDPFlexVerifier.cpp

${OBJECTDIR}/Data.o: Data.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Data.o Data.cpp

${OBJECTDIR}/FourSquares.o: FourSquares.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FourSquares.o FourSquares.cpp

${OBJECTDIR}/Group.o: Group.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Group.o Group.cpp

${OBJECTDIR}/GroupRSA.o: GroupRSA.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GroupRSA.o GroupRSA.cpp

${OBJECTDIR}/Hash.o: Hash.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Hash.o Hash.cpp

${OBJECTDIR}/Key.o: Key.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Key.o Key.cpp

${OBJECTDIR}/Merkle.o: Merkle.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Merkle.o Merkle.cpp

${OBJECTDIR}/MultiExp.o: MultiExp.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MultiExp.o MultiExp.cpp

${OBJECTDIR}/NTL/ZZ.o: NTL/ZZ.cpp
	${MKDIR} -p ${OBJECTDIR}/NTL
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NTL/ZZ.o NTL/ZZ.cpp

${OBJECTDIR}/Node.o: Node.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Node.o Node.cpp

${OBJECTDIR}/ProofNode.o: ProofNode.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ProofNode.o ProofNode.cpp

${OBJECTDIR}/RankBasedNode.o: RankBasedNode.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/RankBasedNode.o RankBasedNode.cpp

${OBJECTDIR}/RankBasedSkipList.o: RankBasedSkipList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/RankBasedSkipList.o RankBasedSkipList.cpp

${OBJECTDIR}/Serialize.o: Serialize.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Serialize.o Serialize.cpp

${OBJECTDIR}/SkipList.o: SkipList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SkipList.o SkipList.cpp

${OBJECTDIR}/Timer.o: Timer.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Timer.o Timer.cpp

${OBJECTDIR}/base64.o: base64.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/base64.o base64.cpp

${OBJECTDIR}/build_challenge_prove_verify.o: build_challenge_prove_verify.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/build_challenge_prove_verify.o build_challenge_prove_verify.cpp

${OBJECTDIR}/build_flexlist.o: build_flexlist.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/build_flexlist.o build_flexlist.cpp

${OBJECTDIR}/check_daemon_existence.o: check_daemon_existence.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/check_daemon_existence.o check_daemon_existence.cpp

${OBJECTDIR}/create_daemon.o: create_daemon.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/create_daemon.o create_daemon.cpp

${OBJECTDIR}/get_mount_location.o: get_mount_location.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/get_mount_location.o get_mount_location.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/mdstat_entry.o: mdstat_entry.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mdstat_entry.o mdstat_entry.cpp

${OBJECTDIR}/read_mdstat.o: read_mdstat.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/read_mdstat.o read_mdstat.cpp

${OBJECTDIR}/watch.o: watch.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/watch.o watch.cpp

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
