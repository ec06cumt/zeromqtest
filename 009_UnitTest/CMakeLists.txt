#Author:Gliu
#Created:2015/12/15

set(${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${EXECUTABLE_OUTPUT_PATH}/test)

file(GLOB SRC_LIST *.cpp *.hpp)

ADD_EXECUTABLE(${TESTGTEST_EXE} ${SRC_LIST})
target_link_libraries(${TESTGTEST_EXE} ${ZMQLIB_LIB} ${GTESTLIB_LIB})

ADD_TEST(${TESTGTEST_EXE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gtest)
