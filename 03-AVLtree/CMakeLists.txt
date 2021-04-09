cmake_minimum_required(VERSION 3.15)

project(AVLTree)

add_library(AVLTree INTERFACE)

target_include_directories(AVLTree INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_features(AVLTree INTERFACE cxx_std_17)

set(CMAKE_BUILD_TYPE Debug)    

add_subdirectory(examples)

add_subdirectory(tests)
