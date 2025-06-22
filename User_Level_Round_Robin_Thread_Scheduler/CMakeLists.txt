cmake_minimum_required(VERSION 3.22)
project(UserLevelThreads)

set(CMAKE_CXX_STANDARD 14)

add_executable(UserLevelThreads
        thread.cpp
        uthread.cpp
        test0_sanity.cpp
        utils.cpp
        scheduler.cpp
        signals.cpp

)
