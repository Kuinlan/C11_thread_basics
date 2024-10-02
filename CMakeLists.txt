cmake_minimum_required(VERSION 3.20)

project(c11_thread)

find_package(Threads)

set(CMAKE_CXX_STANDARD 11)

add_executable(c11_thread1 1_basic.cpp)
target_link_libraries(c11_thread1 ${CMAKE_THREAD_LIBS_INIT})

add_executable(c11_thread2 2_common_error.cpp)
target_link_libraries(c11_thread2 ${CMAKE_THREAD_LIBS_INIT})

add_executable(c11_thread3 3_mutex.cpp)
target_link_libraries(c11_thread3 ${CMAKE_THREAD_LIBS_INIT})

add_executable(c11_thread4 4_deadlock.cpp)
target_link_libraries(c11_thread4 ${CMAKE_THREAD_LIBS_INIT})

add_executable(c11_thread5 5_lock_management.cpp)
target_link_libraries(c11_thread5 ${CMAKE_THREAD_LIBS_INIT})

add_executable(c11_thread6 6_call_once_usage.cpp)
target_link_libraries(c11_thread6 ${CMAKE_THREAD_LIBS_INIT})

add_executable(c11_thread7 7_condition_variable.cpp)
target_link_libraries(c11_thread7 ${CMAKE_THREAD_LIBS_INIT})

add_executable(c11_thread8 8_thread_poll.cpp)
target_link_libraries(c11_thread8 ${CMAKE_THREAD_LIBS_INIT})

add_executable(c11_thread9 9_thread_async.cpp)
target_link_libraries(c11_thread9 ${CMAKE_THREAD_LIBS_INIT})

add_executable(c11_thread10 10_thread_atomic.cpp)
target_link_libraries(c11_thread10 ${CMAKE_THREAD_LIBS_INIT})
