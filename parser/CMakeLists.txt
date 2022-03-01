cmake_minimum_required(VERSION 3.21)

target_sources(chclang PRIVATE parser.cpp)

target_include_directories(chclang PRIVATE include)