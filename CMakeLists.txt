cmake_minimum_required(VERSION 3.26)
project(Linked_List)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Linked_List
        DLL.cpp
        DLL.hpp
        DNode.cpp
        DNode.hpp
        MainDLL.cpp
        Task.cpp
        Task.hpp
        TaskManager.cpp
        TaskManager.hpp)
