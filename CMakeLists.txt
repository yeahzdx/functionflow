PROJECT(FunctionFlow)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
MESSAGE(STATUS "This is BINARY dir" ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir" ${PROJECT_SOURCE_DIR})
add_definitions(-std=c++11)
#add_definitions(-g)
add_definitions(-O2)
add_definitions(-DNDEBUG)
add_definitions(-DUSING_LOCK_FREE_QUEUE)
SET(DEBUG_DEFS "")

include_directories(${PROJECT_SOURCE_DIR}/ff/include)
#include_directories(/root/projects/ff)
link_directories(ff/build)
#SET(LIBS_LINK_DIRS "/home/athrun/boost_1_46_1/stage/lib")
#SET(LIBS_LINK_LIBS boost_date_time boost_thread log4cplus)
SET(ff_src ff/runtime/env.cpp 
  ff/runtime/runtime.cpp 
  ff/para/para.cpp 
  ff/common/common.cpp)
ADD_LIBRARY(ff SHARED ${ff_src})
ADD_LIBRARY(ff_s STATIC ${ff_src})
target_link_libraries(ff pthread)
target_link_libraries(ff_s pthread)

SET_TARGET_PROPERTIES(ff_s PROPERTIES OUTPUT_NAME "ff")

#add_subdirectory(test)
#add_subdirectory(benchmark)
#add_subdirectory(test)
add_subdirectory(benchmark)
