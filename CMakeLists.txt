cmake_minimum_required(VERSION 3.14)
# set the project name
project(dispatch)


# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER nvc++)


add_executable(dispatch.x  src/dispatch.cpp)

#Warnings as errors:
if(MSVC)
  target_compile_options(dispatch.x PRIVATE /W4 /WX)
else()
  target_compile_options(dispatch.x PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

