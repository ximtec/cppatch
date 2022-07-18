cmake_minimum_required(VERSION 3.14)

# specify the C++ standard
set(CMAKE_CXX_COMPILER "/mn/stornext/u3/michhaa/gcc-offload/install/bin/g++")#"~/gcc-offload/install/bin/g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(CMAKE_CXX_COMPILER nvc++)

# set the project name
project(dispatch)


add_executable(dispatch.x  src/dispatch.cpp)

#Warnings as errors:
if(MSVC)
  target_compile_options(dispatch.x PRIVATE /W4 /WX)
else()
  target_compile_options(dispatch.x PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

