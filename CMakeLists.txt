#进入build目录   cmake .. && cmake --build .
cmake_minimum_required(VERSION 2.8)
PROJECT(asio_learn) 
set(CMAKE_CXX_STANDARD 14)  # 将 C++ 标准设置为 C++ 11
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # C++ 11 是强制要求，不会衰退至低版本
SET(CMAKE_BUILD_TYPE "Debug") #Debug模式
#SET(CMAKE_BUILD_TYPE "Release") #Release模式

include(GNUInstallDirs)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${CMAKE_INSTALL_BINDIR})

message(STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")
message(STATUS "CMAKE_INSTALL_BINDIR = ${CMAKE_INSTALL_BINDIR}")
SET(CMAKE_CXX_FLAGS_DEBUG " -O0 -fPIC -Wall -g3 -ggdb")  #Debug模式下添加的编译选项
SET(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O3 -Wall")  #Release模式下添加的编译选项

find_package(Boost REQUIRED COMPONENTS system thread)
find_package(Protobuf REQUIRED)

message(STATUS "BOOST VERSION = ${Boost_VERSION}")


include_directories(include ${Boost_INCLUDE_DIRS} ${Protobuf_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR}/proto)

add_subdirectory(proto)

#add_library(hello2 SHARED libsrc_hello2/hello2.cpp) #添加hello2动态库
#set_target_properties(hello2 PROPERTIES
#  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
#)

##SET(SOURCE_FILES
##    src/sync_client.cpp
##    src/sync_server.cpp
##    )


#ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES}) #生成可执行文件 
ADD_EXECUTABLE(sync_client src/sync_client.cpp)
ADD_EXECUTABLE(sync_server src/sync_server.cpp)
ADD_EXECUTABLE(test_sync_client src/test_sync_client.cpp)
ADD_EXECUTABLE(async_server src/server.cpp  src/msg_node.cpp src/logic_node.cpp src/logic_system.cpp  src/session.cpp src/iocontextpool.cpp main.cpp)

set_target_properties(async_server PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

target_link_libraries(async_server
  PUBLIC
     ${Boost_LIBRARIES}
     ${Protobuf_LIBRARIES}
     proto
	)

set_target_properties(test_sync_client PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

target_link_libraries(test_sync_client
  PUBLIC
    ${Boost_LIBRARIES}
    ${Protobuf_LIBRARIES}
    proto
  )

set_target_properties(sync_client PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

target_link_libraries(sync_client
  PUBLIC
    ${Boost_LIBRARIES}
  )

set_target_properties(sync_server PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

target_link_libraries(sync_server
  PUBLIC
    ${Boost_LIBRARIES}
  )


#target_link_libraries(${PROJECT_NAME} PRIVATE hello2) #链接动态库