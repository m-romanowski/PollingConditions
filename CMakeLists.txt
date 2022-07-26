cmake_minimum_required(VERSION 3.22)
project(PollingConditions)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(SOURCES ${CMAKE_SOURCE_DIR}/src)
set(TESTS ${CMAKE_SOURCE_DIR}/test)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include_directories(${SOURCES})
include_directories(${TESTS})

set(SOURCE_FILES
        ${SOURCES}/pollingconditions.h
        ${SOURCES}/predefinedpollingconditions.h
        ${TESTS}/pollingconditionstests.cpp)

add_executable(pollingConditionsTests ${SOURCE_FILES})
target_link_libraries(pollingConditionsTests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
