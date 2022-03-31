cmake_minimum_required(VERSION 3.21)

target_sources(chclang PRIVATE
        type.cpp
        integral_type.cpp
        floating_type.cpp)

target_include_directories(chclang PRIVATE include)