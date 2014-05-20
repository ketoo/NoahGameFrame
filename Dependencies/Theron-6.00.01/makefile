

#
# This makefile is intended as a starting point for building Theron with GCC, typically
# under a flavor of Linux. The comments here are intentionally brief; see the Getting
# Started section of the Theron website for more detailed information:
# http://www.theron-library.com/index.php?t=page&p=getting%20started
#
# Example command line:
#  make clean all mode=release boost=on
#
# Syntax: make <targets> <options>
#
# targets:
#   clean            deletes previous build output
#   library          builds the Theron library
#   tests            builds the test executable
#   benchmarks       builds the benchmark executables
#   tutorial         builds the sample executables
#
# options:
#   mode=debug       checked build (defines _DEBUG)
#   mode=release     optimized build (defines NDEBUG)
#   windows=[on|off] Force-enables or disables use of Windows functionality (via THERON_WINDOWS).
#   boost=[on|off]   Force-enables or disables use of Boost (via THERON_BOOST)
#   c++11=[on|off]   Force-enables or disables use of C++11 features (via THERON_CPP11)
#   posix=[on|off]   Force-enables or disables use of POSIX OS features (via THERON_POSIX)
#   numa=[on|off]    Force-enables or disables use of NUMA features (via THERON_NUMA)
#   xs=[on|off]      Force-enables or disables use of Crossroads.io network features (via THERON_XS)
#   shared=[on|off]  generates shared code (adds -fPIC to GCC command line)
#


#
# Boost and libxs paths and library names. Change these if you need to.
# These settings are intended to match a 'typical' linux environment.
# If your environment differs, you may need to change these settings to match.
# Alternatively you may wish to copy your built or downloaded libraries to these paths.
#

BOOST_INCLUDE_PATH = /home/amason/boost_1_49_0/
BOOST_RELEASE_LIB_PATH = /home/amason/boost_1_49_0/stage/lib/
BOOST_DEBUG_LIB_PATH = /home/amason/boost_1_49_0/stage/lib/
BOOST_LIB = boost_thread

#BOOST_INCLUDE_PATH = /usr/include/
#BOOST_RELEASE_LIB_PATH = /usr/lib/
#BOOST_DEBUG_LIB_PATH = /usr/lib/debug/usr/lib/
#BOOST_LIB = boost_thread

XS_INCLUDE_PATH = /usr/include/
XS_RELEASE_LIB_PATH = /usr/lib/
XS_DEBUG_LIB_PATH = /usr/lib/debug/usr/lib/
XS_RELEASE_LIB = xs
XS_DEBUG_LIB = xs_d

#
# Tools
# Note that this assumes you have 'rm' available, if you're using MinGW with no rm then use del instead.
#

CC = g++
AR = ar
RM = rm -f

CFLAGS = -c -Wall -pthread -D_GLIBCXX_USE_NANOSLEEP -D_GLIBCXX_USE_SCHED_YIELD
LDFLAGS = -Wl,--allow-multiple-definition -pthread
INCLUDE_FLAGS = -IInclude -IInclude/External
BUILD = Build
BIN = Bin
LIB = Lib
LIB_FLAGS = -lrt
ARFLAGS = r

#
# Use "mode=debug" or "mode=release". Default is debug.
#

ifeq ($(mode),release)
	CFLAGS += -O3 -fno-strict-aliasing -DNDEBUG
	LDFLAGS += -O3 -DNDEBUG
	LIBNAME = theron
	BOOST_LIB_PATH = ${BOOST_RELEASE_LIB_PATH}
    XS_LIB_PATH = ${XS_RELEASE_LIB_PATH}
    XS_LIB = ${XS_RELEASE_LIB}
else
	CFLAGS += -g -D_DEBUG
	LDFLAGS += -g -D_DEBUG
	LIBNAME = therond
	BOOST_LIB_PATH = ${BOOST_DEBUG_LIB_PATH}
    XS_LIB_PATH = ${XS_DEBUG_LIB_PATH}
    XS_LIB = ${XS_DEBUG_LIB}
endif

#
# Use "windows=off" to disable use of Windows features, in particular Windows threads.
# By default Windows features are auto-detected and used only if available.
#

ifeq ($(windows),off)
	CFLAGS += -DTHERON_WINDOWS=0
else ifeq ($(windows),on)
	CFLAGS += -DTHERON_WINDOWS=1
endif

#
# Use "posix=off" to disable use of POSIX features, in particular pthread.
#

ifeq ($(posix),off)
	CFLAGS += -DTHERON_POSIX=0
else ifeq ($(posix),on)
	CFLAGS += -DTHERON_POSIX=1
endif

#
# Use "boost=on" to enable use of Boost features, in particular boost::thread and Boost atomics.
# By default Boost features are assumed to be unavailable.
#

ifeq ($(boost),off)
	CFLAGS += -DTHERON_BOOST=0
else ifeq ($(boost),on)
	CFLAGS += -DTHERON_BOOST=1
	INCLUDE_FLAGS += -I${BOOST_INCLUDE_PATH}
	LIB_FLAGS += -L${BOOST_LIB_PATH} -l${BOOST_LIB}
endif

#
# Use "c++11=on" to enable use of C++11 features, in particular std::thread.
# By default C++11 features are assumed to be unavailable.
#

ifeq ($(c++11),off)
	CFLAGS += -DTHERON_CPP11=0
else ifeq ($(c++11),on)
	CFLAGS += -DTHERON_CPP11=1 -std=c++11
endif

#
# Use "numa=on" to enable use of NUMA features.
# By default NUMA features are assumed to be unavailable on platforms other than Windows.
#

ifeq ($(numa),off)
	CFLAGS += -DTHERON_NUMA=0
else ifeq ($(numa),on)
	CFLAGS += -DTHERON_NUMA=1
	LIB_FLAGS += -lnuma
endif

#
# Use "numa=on" to enable use of Crossroads.io features for network/remote actor support.
# By default Crossroads.io is assumed to not be available, and remote actor support is disabled.
#

ifeq ($(xs),off)
	CFLAGS += -DTHERON_XS=0
else ifeq ($(xs),on)
	CFLAGS += -DTHERON_XS=1
	INCLUDE_FLAGS += -I${XS_INCLUDE_PATH}
	LIB_FLAGS += -L${XS_LIB_PATH} -l${XS_LIB}
endif

#
# Use "shared=off" to force code generation for statically linked libraries (default).
# Use "shared=on" to force code generation for dynamically linked/shared libraries.
#

ifeq ($(shared),on)
	CFLAGS += -fPIC
endif


#
# End of user-configurable settings.
#


THERON_LIB = ${LIB}/lib${LIBNAME}.a

TESTS = ${BIN}/Tests

THREADRING = ${BIN}/ThreadRing
PARALLELTHREADRING = ${BIN}/ParallelThreadRing
PINGPONG = ${BIN}/PingPong
PRIMEFACTORS = ${BIN}/PrimeFactors

ALIGNMENT = ${BIN}/Alignment
CUSTOMALLOCATORS = ${BIN}/CustomAllocators
FILEREADER = ${BIN}/FileReader
HELLOWORLD = ${BIN}/HelloWorld
MESSAGEREGISTRATION = ${BIN}/MessageRegistration
UNHANDLEDMESSAGES = ${BIN}/UnhandledMessages
CLIENT = ${BIN}/Client
SERVER = ${BIN}/Server


#
# Targets
#


all: library tests benchmarks tutorial

library: summary $(THERON_LIB)

summary:
	@echo **** Using these settings -- see makefile for options: ****
	@echo     CFLAGS = ${CFLAGS}
	@echo     LDFLAGS = ${LDFLAGS}
	@echo     LIBNAME = ${LIBNAME}
	@echo     INCLUDE_FLAGS = ${INCLUDE_FLAGS}
	@echo     LIB_FLAGS = ${LIB_FLAGS}

tests: library ${TESTS}

benchmarks: library \
	${THREADRING} \
	${PARALLELTHREADRING} \
	${PINGPONG} \
	${PRIMEFACTORS}

tutorial: library \
	${ALIGNMENT} \
	${CUSTOMALLOCATORS} \
	${FILEREADER} \
	${HELLOWORLD} \
	${MESSAGEREGISTRATION} \
	${UNHANDLEDMESSAGES} \
	${CLIENT} \
	${SERVER}

clean:
	${RM} ${BUILD}/*.o
	${RM} ${BUILD}/*.a
	${RM} ${BUILD}/*.ilk
	${RM} ${BIN}/*.pdb
	${RM} ${BIN}/*.ilk
	${RM} ${BIN}/*.exe
	${RM} ${LIB}/*.a
	${RM} ${LIB}/*.lib


#
# Library
#


THERON_HEADERS = \
	Include/Theron/Detail/Alignment/MessageAlignment.h \
	Include/Theron/Detail/Allocators/CachingAllocator.h \
	Include/Theron/Detail/Allocators/Pool.h \
	Include/Theron/Detail/Containers/List.h \
	Include/Theron/Detail/Containers/Map.h \
	Include/Theron/Detail/Containers/Queue.h \
	Include/Theron/Detail/Debug/BuildDescriptor.h \
	Include/Theron/Detail/Directory/Entry.h \
	Include/Theron/Detail/Directory/Directory.h \
	Include/Theron/Detail/Directory/StaticDirectory.h \
	Include/Theron/Detail/Handlers/BlindDefaultHandler.h \
	Include/Theron/Detail/Handlers/BlindFallbackHandler.h \
	Include/Theron/Detail/Handlers/DefaultFallbackHandler.h \
	Include/Theron/Detail/Handlers/DefaultHandler.h \
	Include/Theron/Detail/Handlers/DefaultHandlerCollection.h \
	Include/Theron/Detail/Handlers/FallbackHandler.h \
	Include/Theron/Detail/Handlers/FallbackHandlerCollection.h \
	Include/Theron/Detail/Handlers/HandlerCollection.h \
	Include/Theron/Detail/Handlers/IDefaultHandler.h \
	Include/Theron/Detail/Handlers/IFallbackHandler.h \
	Include/Theron/Detail/Handlers/IMessageHandler.h \
	Include/Theron/Detail/Handlers/IReceiverHandler.h \
	Include/Theron/Detail/Handlers/MessageHandler.h \
	Include/Theron/Detail/Handlers/MessageHandlerCast.h \
	Include/Theron/Detail/Handlers/ReceiverHandler.h \
    Include/Theron/Detail/Handlers/ReceiverHandlerCast.h \
	Include/Theron/Detail/Mailboxes/Mailbox.h \
	Include/Theron/Detail/Scheduler/BlockingMonitor.h \
	Include/Theron/Detail/Scheduler/Counting.h \
	Include/Theron/Detail/Scheduler/IScheduler.h \
	Include/Theron/Detail/Scheduler/MailboxContext.h \
	Include/Theron/Detail/Scheduler/MailboxProcessor.h \
	Include/Theron/Detail/Scheduler/MailboxQueue.h \
	Include/Theron/Detail/Scheduler/NonBlockingMonitor.h \
	Include/Theron/Detail/Scheduler/Scheduler.h \
	Include/Theron/Detail/Scheduler/SchedulerHints.h \
	Include/Theron/Detail/Scheduler/ThreadPool.h \
	Include/Theron/Detail/Scheduler/WorkerContext.h \
	Include/Theron/Detail/Scheduler/YieldImplementation.h \
	Include/Theron/Detail/Scheduler/YieldPolicy.h \
	Include/Theron/Detail/Messages/IMessage.h \
	Include/Theron/Detail/Messages/Message.h \
	Include/Theron/Detail/Messages/MessageCast.h \
	Include/Theron/Detail/Messages/MessageCreator.h \
	Include/Theron/Detail/Messages/MessageSize.h \
	Include/Theron/Detail/Messages/MessageTraits.h \
	Include/Theron/Detail/Network/Index.h \
	Include/Theron/Detail/Network/MessageFactory.h \
	Include/Theron/Detail/Network/NameGenerator.h \
	Include/Theron/Detail/Network/NameMap.h \
	Include/Theron/Detail/Network/NetworkMessage.h \
	Include/Theron/Detail/Strings/String.h \
	Include/Theron/Detail/Strings/StringHash.h \
	Include/Theron/Detail/Strings/StringPool.h \
	Include/Theron/Detail/Threading/Atomic.h \
	Include/Theron/Detail/Threading/Clock.h \
	Include/Theron/Detail/Threading/Condition.h \
	Include/Theron/Detail/Threading/Lock.h \
	Include/Theron/Detail/Threading/Mutex.h \
	Include/Theron/Detail/Threading/SpinLock.h \
	Include/Theron/Detail/Threading/Thread.h \
	Include/Theron/Detail/Threading/Utils.h \
	Include/Theron/Detail/Transport/Context.h \
	Include/Theron/Detail/Transport/InputMessage.h \
	Include/Theron/Detail/Transport/InputSocket.h \
	Include/Theron/Detail/Transport/OutputMessage.h \
	Include/Theron/Detail/Transport/OutputSocket.h \
	Include/Theron/Actor.h \
	Include/Theron/Address.h \
	Include/Theron/Align.h \
	Include/Theron/AllocatorManager.h \
	Include/Theron/Assert.h \
	Include/Theron/BasicTypes.h \
	Include/Theron/Catcher.h \
	Include/Theron/DefaultAllocator.h \
	Include/Theron/Defines.h \
	Include/Theron/Framework.h \
	Include/Theron/IAllocator.h \
	Include/Theron/EndPoint.h \
	Include/Theron/Receiver.h \
	Include/Theron/Register.h \
	Include/Theron/Theron.h \
	Include/Theron/YieldStrategy.h

THERON_SOURCES = \
	Theron/Actor.cpp \
	Theron/Address.cpp \
	Theron/AllocatorManager.cpp \
	Theron/BuildDescriptor.cpp \
	Theron/Clock.cpp \
	Theron/DefaultHandlerCollection.cpp \
	Theron/EndPoint.cpp \
	Theron/FallbackHandlerCollection.cpp \
	Theron/Framework.cpp \
	Theron/HandlerCollection.cpp \
	Theron/Receiver.cpp \
	Theron/StringPool.cpp \
	Theron/YieldPolicy.cpp

THERON_OBJECTS = \
	${BUILD}/Actor.o \
	${BUILD}/Address.o \
	${BUILD}/AllocatorManager.o \
	${BUILD}/BuildDescriptor.o \
	${BUILD}/Clock.o \
	${BUILD}/DefaultHandlerCollection.o \
	${BUILD}/EndPoint.o \
	${BUILD}/FallbackHandlerCollection.o \
	${BUILD}/Framework.o \
	${BUILD}/HandlerCollection.o \
	${BUILD}/Receiver.o \
	${BUILD}/StringPool.o \
	${BUILD}/YieldPolicy.o

$(THERON_LIB): $(THERON_OBJECTS)
	${AR} ${ARFLAGS} ${THERON_LIB} $(THERON_OBJECTS)

${BUILD}/Actor.o: Theron/Actor.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Theron/Actor.cpp -o ${BUILD}/Actor.o ${INCLUDE_FLAGS}

${BUILD}/Address.o: Theron/Address.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Theron/Address.cpp -o ${BUILD}/Address.o ${INCLUDE_FLAGS}

${BUILD}/AllocatorManager.o: Theron/AllocatorManager.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Theron/AllocatorManager.cpp -o ${BUILD}/AllocatorManager.o ${INCLUDE_FLAGS}

${BUILD}/BuildDescriptor.o: Theron/BuildDescriptor.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Theron/BuildDescriptor.cpp -o ${BUILD}/BuildDescriptor.o ${INCLUDE_FLAGS}

${BUILD}/Clock.o: Theron/Clock.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Theron/Clock.cpp -o ${BUILD}/Clock.o ${INCLUDE_FLAGS}

${BUILD}/DefaultHandlerCollection.o: Theron/DefaultHandlerCollection.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Theron/DefaultHandlerCollection.cpp -o ${BUILD}/DefaultHandlerCollection.o ${INCLUDE_FLAGS}

${BUILD}/EndPoint.o: Theron/EndPoint.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Theron/EndPoint.cpp -o ${BUILD}/EndPoint.o ${INCLUDE_FLAGS}

${BUILD}/FallbackHandlerCollection.o: Theron/FallbackHandlerCollection.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Theron/FallbackHandlerCollection.cpp -o ${BUILD}/FallbackHandlerCollection.o ${INCLUDE_FLAGS}

${BUILD}/Framework.o: Theron/Framework.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Theron/Framework.cpp -o ${BUILD}/Framework.o ${INCLUDE_FLAGS}

${BUILD}/HandlerCollection.o: Theron/HandlerCollection.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Theron/HandlerCollection.cpp -o ${BUILD}/HandlerCollection.o ${INCLUDE_FLAGS}

${BUILD}/Receiver.o: Theron/Receiver.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Theron/Receiver.cpp -o ${BUILD}/Receiver.o ${INCLUDE_FLAGS}

${BUILD}/StringPool.o: Theron/StringPool.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Theron/StringPool.cpp -o ${BUILD}/StringPool.o ${INCLUDE_FLAGS}

${BUILD}/YieldPolicy.o: Theron/YieldPolicy.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Theron/YieldPolicy.cpp -o ${BUILD}/YieldPolicy.o ${INCLUDE_FLAGS}

#
# Tests
#


TESTS_INCLUDE_FLAGS = -ITests/

TESTS_HEADERS = \
	Tests/TestFramework/ITestSuite.h \
	Tests/TestFramework/TestException.h \
	Tests/TestFramework/TestManager.h \
	Tests/TestFramework/TestSuite.h \
	Tests/TestSuites/FeatureTestSuite.h \
	Tests/TestSuites/NetworkTestSuite.h

TESTS_SOURCES = \
	Tests/Tests.cpp
	
TESTS_OBJECTS = \
	${BUILD}/Tests.o

${TESTS}: $(THERON_LIB) ${TESTS_OBJECTS}
	$(CC) $(LDFLAGS) ${TESTS_OBJECTS} $(THERON_LIB) -o ${TESTS} ${LIB_FLAGS}

${BUILD}/Tests.o: Tests/Tests.cpp ${THERON_HEADERS} ${TESTS_HEADERS}
	$(CC) $(CFLAGS) Tests/Tests.cpp -o ${BUILD}/Tests.o ${INCLUDE_FLAGS} ${TESTS_INCLUDE_FLAGS}


#
# Benchmarks
#


# ThreadRing benchmark
THREADRING_HEADERS = Benchmarks/Common/Timer.h

THREADRING_SOURCES = Benchmarks/ThreadRing/ThreadRing.cpp
THREADRING_OBJECTS = ${BUILD}/ThreadRing.o

${THREADRING}: $(THERON_LIB) ${THREADRING_OBJECTS}
	$(CC) $(LDFLAGS) ${THREADRING_OBJECTS} $(THERON_LIB) -o ${THREADRING} ${LIB_FLAGS}

${BUILD}/ThreadRing.o: Benchmarks/ThreadRing/ThreadRing.cpp ${THERON_HEADERS} ${THREADRING_HEADERS}
	$(CC) $(CFLAGS) Benchmarks/ThreadRing/ThreadRing.cpp -o ${BUILD}/ThreadRing.o ${INCLUDE_FLAGS}


# ParallelThreadRing benchmark
PARALLELTHREADRING_HEADERS = Benchmarks/Common/Timer.h

PARALLELTHREADRING_SOURCES = Benchmarks/ParallelThreadRing/ParallelThreadRing.cpp
PARALLELTHREADRING_OBJECTS = ${BUILD}/ParallelThreadRing.o

${PARALLELTHREADRING}: $(THERON_LIB) ${PARALLELTHREADRING_OBJECTS}
	$(CC) $(LDFLAGS) ${PARALLELTHREADRING_OBJECTS} $(THERON_LIB) -o ${PARALLELTHREADRING} ${LIB_FLAGS}

${BUILD}/ParallelThreadRing.o: Benchmarks/ParallelThreadRing/ParallelThreadRing.cpp ${THERON_HEADERS} ${PARALLELTHREADRING_HEADERS}
	$(CC) $(CFLAGS) Benchmarks/ParallelThreadRing/ParallelThreadRing.cpp -o ${BUILD}/ParallelThreadRing.o ${INCLUDE_FLAGS}


# PingPong benchmark
PINGPONG_SOURCES = Benchmarks/PingPong/PingPong.cpp
PINGPONG_OBJECTS = ${BUILD}/PingPong.o

${PINGPONG}: $(THERON_LIB) ${PINGPONG_OBJECTS}
	$(CC) $(LDFLAGS) ${PINGPONG_OBJECTS} $(THERON_LIB) -o ${PINGPONG} ${LIB_FLAGS}

${BUILD}/PingPong.o: Benchmarks/PingPong/PingPong.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Benchmarks/PingPong/PingPong.cpp -o ${BUILD}/PingPong.o ${INCLUDE_FLAGS}


# PrimeFactors benchmark
PRIMEFACTORS_SOURCES = Benchmarks/PrimeFactors/PrimeFactors.cpp
PRIMEFACTORS_OBJECTS = ${BUILD}/PrimeFactors.o

${PRIMEFACTORS}: $(THERON_LIB) ${PRIMEFACTORS_OBJECTS}
	$(CC) $(LDFLAGS) ${PRIMEFACTORS_OBJECTS} $(THERON_LIB) -o ${PRIMEFACTORS} ${LIB_FLAGS}

${BUILD}/PrimeFactors.o: Benchmarks/PrimeFactors/PrimeFactors.cpp ${THERON_HEADERS}
	$(CC) $(CFLAGS) Benchmarks/PrimeFactors/PrimeFactors.cpp -o ${BUILD}/PrimeFactors.o ${INCLUDE_FLAGS}


#
# Tutorial
#


# Alignment sample
ALIGNMENT_HEADERS =

ALIGNMENT_SOURCES = Tutorial/Alignment/Alignment.cpp
ALIGNMENT_OBJECTS = ${BUILD}/Alignment.o

${ALIGNMENT}: $(THERON_LIB) ${ALIGNMENT_OBJECTS}
	$(CC) $(LDFLAGS) ${ALIGNMENT_OBJECTS} $(THERON_LIB) -o ${ALIGNMENT} ${LIB_FLAGS}

${BUILD}/Alignment.o: Tutorial/Alignment/Alignment.cpp ${THERON_HEADERS} ${ALIGNMENT_HEADERS}
	$(CC) $(CFLAGS) Tutorial/Alignment/Alignment.cpp -o ${BUILD}/Alignment.o ${INCLUDE_FLAGS}


# CustomAllocators sample
CUSTOMALLOCATORS_HEADERS =

CUSTOMALLOCATORS_SOURCES = Tutorial/CustomAllocators/CustomAllocators.cpp
CUSTOMALLOCATORS_OBJECTS = ${BUILD}/CustomAllocators.o

${CUSTOMALLOCATORS}: $(THERON_LIB) ${CUSTOMALLOCATORS_OBJECTS}
	$(CC) $(LDFLAGS) ${CUSTOMALLOCATORS_OBJECTS} $(THERON_LIB) -o ${CUSTOMALLOCATORS} ${LIB_FLAGS}

${BUILD}/CustomAllocators.o: Tutorial/CustomAllocators/CustomAllocators.cpp ${THERON_HEADERS} ${CUSTOMALLOCATORS_HEADERS}
	$(CC) $(CFLAGS) Tutorial/CustomAllocators/CustomAllocators.cpp -o ${BUILD}/CustomAllocators.o ${INCLUDE_FLAGS}


# FileReader sample
FILEREADER_HEADERS =

FILEREADER_SOURCES = Tutorial/FileReader/FileReader.cpp
FILEREADER_OBJECTS = ${BUILD}/FileReader.o

${FILEREADER}: $(THERON_LIB) ${FILEREADER_OBJECTS}
	$(CC) $(LDFLAGS) ${FILEREADER_OBJECTS} $(THERON_LIB) -o ${FILEREADER} ${LIB_FLAGS}

${BUILD}/FileReader.o: Tutorial/FileReader/FileReader.cpp ${THERON_HEADERS} ${FILEREADER_HEADERS}
	$(CC) $(CFLAGS) Tutorial/FileReader/FileReader.cpp -o ${BUILD}/FileReader.o ${INCLUDE_FLAGS}


# HelloWorld sample
HELLOWORLD_HEADERS =

HELLOWORLD_SOURCES = Tutorial/HelloWorld/HelloWorld.cpp
HELLOWORLD_OBJECTS = ${BUILD}/HelloWorld.o

${HELLOWORLD}: $(THERON_LIB) ${HELLOWORLD_OBJECTS}
	$(CC) $(LDFLAGS) ${HELLOWORLD_OBJECTS} $(THERON_LIB) -o ${HELLOWORLD} ${LIB_FLAGS}

${BUILD}/HelloWorld.o: Tutorial/HelloWorld/HelloWorld.cpp ${THERON_HEADERS} ${HELLOWORLD_HEADERS}
	$(CC) $(CFLAGS) Tutorial/HelloWorld/HelloWorld.cpp -o ${BUILD}/HelloWorld.o ${INCLUDE_FLAGS}


# MessageRegistration sample
MESSAGEREGISTRATION_HEADERS =

MESSAGEREGISTRATION_SOURCES = Tutorial/MessageRegistration/MessageRegistration.cpp
MESSAGEREGISTRATION_OBJECTS = ${BUILD}/MessageRegistration.o

${MESSAGEREGISTRATION}: $(THERON_LIB) ${MESSAGEREGISTRATION_OBJECTS}
	$(CC) $(LDFLAGS) ${MESSAGEREGISTRATION_OBJECTS} $(THERON_LIB) -o ${MESSAGEREGISTRATION} ${LIB_FLAGS}

${BUILD}/MessageRegistration.o: Tutorial/MessageRegistration/MessageRegistration.cpp ${THERON_HEADERS} ${MESSAGEREGISTRATION_HEADERS}
	$(CC) $(CFLAGS) Tutorial/MessageRegistration/MessageRegistration.cpp -o ${BUILD}/MessageRegistration.o ${INCLUDE_FLAGS}


# UnhandledMessages sample
UNHANDLEDMESSAGES_HEADERS =

UNHANDLEDMESSAGES_SOURCES = Tutorial/UnhandledMessages/UnhandledMessages.cpp
UNHANDLEDMESSAGES_OBJECTS = ${BUILD}/UnhandledMessages.o

${UNHANDLEDMESSAGES}: $(THERON_LIB) ${UNHANDLEDMESSAGES_OBJECTS}
	$(CC) $(LDFLAGS) ${UNHANDLEDMESSAGES_OBJECTS} $(THERON_LIB) -o ${UNHANDLEDMESSAGES} ${LIB_FLAGS}

${BUILD}/UnhandledMessages.o: Tutorial/UnhandledMessages/UnhandledMessages.cpp ${THERON_HEADERS} ${UNHANDLEDMESSAGES_HEADERS}
	$(CC) $(CFLAGS) Tutorial/UnhandledMessages/UnhandledMessages.cpp -o ${BUILD}/UnhandledMessages.o ${INCLUDE_FLAGS}


# Client sample
CLIENT_HEADERS =

CLIENT_SOURCES = Tutorial/Client/Client.cpp
CLIENT_OBJECTS = ${BUILD}/Client.o

${CLIENT}: $(THERON_LIB) ${CLIENT_OBJECTS} Tutorial/Common/Utils.h
	$(CC) $(LDFLAGS) ${CLIENT_OBJECTS} $(THERON_LIB) -o ${CLIENT} ${LIB_FLAGS}

${BUILD}/Client.o: Tutorial/Client/Client.cpp ${THERON_HEADERS} ${CLIENT_HEADERS}
	$(CC) $(CFLAGS) Tutorial/Client/Client.cpp -o ${BUILD}/Client.o ${INCLUDE_FLAGS}


# Server sample
SERVER_HEADERS =

SERVER_SOURCES = Tutorial/Server/Server.cpp
SERVER_OBJECTS = ${BUILD}/Server.o

${SERVER}: $(THERON_LIB) ${SERVER_OBJECTS} Tutorial/Common/Utils.h
	$(CC) $(LDFLAGS) ${SERVER_OBJECTS} $(THERON_LIB) -o ${SERVER} ${LIB_FLAGS}

${BUILD}/Server.o: Tutorial/Server/Server.cpp ${THERON_HEADERS} ${SERVER_HEADERS}
	$(CC) $(CFLAGS) Tutorial/Server/Server.cpp -o ${BUILD}/Server.o ${INCLUDE_FLAGS}


