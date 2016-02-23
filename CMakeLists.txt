#Author:Gliu
#Created:2015/12/02

#工程名称
PROJECT(GameServer)

MESSAGE(STATUS "This BINARY dir is ${PROJECT_BINARY_DIR}")
MESSAGE(STATUS "This SOURCE dir is ${PROJECT_SOURCE_DIR}")

#cmake的最低版本
cmake_minimum_required(VERSION 2.6)

# Cmake module setting
set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH}  ${CMAKE_SOURCE_DIR}/build/Cmake)
include(${CMAKE_SOURCE_DIR}/build/Cmake/EchoConfig.cmake)
include(${CMAKE_SOURCE_DIR}/build/Cmake/GetDir.cmake)
#平台是64位还是32位的check
find_package(Platform REQUIRED)

# Force out-of-source build，强制外部建立，内部建立报错
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" BUILDING_IN_SOURCE)
if(BUILDING_IN_SOURCE)
    cecho(cred "This project requires an out of source build. \nRemove the file 'CMakeCache.txt' found in this directory before continuing. \nCreate a separate build directory and run 'cmake <srcs> [options]' from there.")
    message(FATAL_ERROR "exit")
endif()

# Platform check
if(PLATFORM MATCHES X86)
	cecho(cred "-- Check: ARCH ${PLATFORM} detected.")		
	#message(FATAL_ERROR "only x86_64 platform support")
else()
    cecho(cred "-- Check: ARCH ${PLATFORM} detected.")
endif()

## 依赖库定义
#####################################################################
# 系统库
set (SYSTEM_LAS z dl pthread)

# 第三方组件库
#set third party lib dir
set (3RD_PARTY_DIR ${PROJECT_SOURCE_DIR}/lib/3RdParty)
MESSAGE(STATUS "This 3RdParty dir is ${3RD_PARTY_DIR}")

#zero mq
set (ZMQLIB_INC ${3RD_PARTY_DIR}/zeromq-3.2.3)
link_directories(${3RD_PARTY_DIR}/zeromq-3.2.3)
set (ZMQLIB_LIB zmq rt)

#gtest 
set (GTESTLIB_INC ${3RD_PARTY_DIR}/gtest-1.6.0)
link_directories(${3RD_PARTY_DIR}/gtest-1.6.0)
set (GTESTLIB_LIB gtest)



# 编译选项
#####################################################################
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

cecho(cgreen "-- BUILD: ${CMAKE_BUILD_TYPE}")
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" 
        OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    add_definitions(-D_DEBUG_)
    set(MT_NAME_TAG ".dbg")
else()
    remove_definitions(D_DEBUG_)
    set(MT_NAME_TAG ".run")
endif()

#这个找个要处理掉
add_definitions(-D_POSIX_MT_)
# 库文件的附加参数 -fPIC
set(ALL_FLAGS_IN_ONE_COMMON "-Wall -fPIC -rdynamic -D_FILE_OFFSET_BITS=64 -Werror -Wno-deprecated-declarations -lcrypto -lidn -lssl") 
set(ALL_FLAGS_IN_ONE_DEBUG "-g -ggdb -O0 ${ALL_FLAGS_IN_ONE_COMMON}")
set(ALL_FLAGS_IN_ONE_RELEASE "-O3 ${ALL_FLAGS_IN_ONE_COMMON} -DNDEBUG")
set(ALL_FLAGS_IN_ONE_RELWITHDEBINFO "-O2 -g -ggdb ${ALL_FLAGS_IN_ONE_COMMON}")
set(ALL_FLAGS_IN_ONE_MINSIZEREL "-Os ${ALL_FLAGS_IN_ONE_COMMON} -DNDEBUG")

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} ${ALL_FLAGS_IN_ONE_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} ${ALL_FLAGS_IN_ONE_RELEASE}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "$ENV{CXXFLAGS} ${ALL_FLAGS_IN_ONE_RELWITHDEBINFO}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "$ENV{CXXFLAGS} ${ALL_FLAGS_IN_ONE_MINSIZEREL}")

set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} ${ALL_FLAGS_IN_ONE_DEBUG}")
set(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} ${ALL_FLAGS_IN_ONE_RELEASE}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "$ENV{CFLAGS} ${ALL_FLAGS_IN_ONE_RELWITHDEBINFO}")
set(CMAKE_C_FLAGS_MINSIZEREL "$ENV{CFLAGS} ${ALL_FLAGS_IN_ONE_MINSIZEREL}")

message(STATUS "CMAKE_CXX_FLAGS_DEBUG =" ${CMAKE_CXX_FLAGS_DEBUG})
message(STATUS "CMAKE_CXX_FLAGS_RELEASE =" ${CMAKE_CXX_FLAGS_RELEASE})
message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO =" ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
message(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL =" ${CMAKE_CXX_FLAGS_MINSIZEREL})

#####################################################################
# 固定生成目录为
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/release/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/release/lib)

#全局引用，每个分项目进程，可以不需要单独引用相应的头文件
#INCLUDES
include_directories(${ZMQLIB_INC})
include_directories(${GTESTLIB_INC})


#Link Libraries
link_directories(${LIBRARY_OUTPUT_PATH})

# sub projects
set (MAIN_EXE			main${MT_NAME_TAG})
set (TESTSERVER_EXE		testserver${MT_NAME_TAG})
set (TESTZMQ_EXE		zmqserver${MT_NAME_TAG})
set (TESTZMQCLIENT_EXE	zmqclient${MT_NAME_TAG})
set (TESTGTEST_EXE		gtestlib${MT_NAME_TAG})

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(server)
ADD_SUBDIRECTORY(002_Zeromq)
ADD_SUBDIRECTORY(003_ZeromqClient)
ADD_SUBDIRECTORY(009_UnitTest)
#ADD_SUBDIRECTORY(lib)
