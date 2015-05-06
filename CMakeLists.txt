cmake_minimum_required (VERSION 2.8)
project (libSoneFiveIPC)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99 -D_XOPEN_SOURCE=500 -Wall")

include_directories(${CMAKE_CURRENT_DIRECTORY}/include)

add_library(SoneFiveIPC SHARED src/ipc.c)

install(TARGETS SoneFiveIPC DESTINATION lib)
install(FILES include/ipc.h DESTINATION include)

add_executable (test_IPC src/test_ipc.c)
target_link_libraries (test_IPC SoneFiveIPC)