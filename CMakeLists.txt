cmake_minimum_required(VERSION 3.26)

project(
    GeneralType 
    VERSION 1.0
    LANGUAGES CXX
)

# The GeneralType heavily utilizes C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add all examples
add_executable(basicUsage Examples/basicUsage.cpp)
add_executable(functUsage Examples/functUsage.cpp)
add_executable(dictionary Examples/dictionary.cpp)

