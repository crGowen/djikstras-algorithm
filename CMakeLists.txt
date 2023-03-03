cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Building library
project(DjikstrasAlgorithm)
add_library(DjikstrasAlgorithm SHARED ./djikalg/main.cxx)
target_include_directories(DjikstrasAlgorithm PRIVATE ./djikalg)
target_compile_definitions(DjikstrasAlgorithm PRIVATE DJIKALG_EXPORT)

# Building example
project(UsageExample)
add_executable(UsageExample ./usage-example/main.cxx)
target_include_directories(UsageExample PRIVATE ./djikalg)
target_link_libraries(UsageExample PRIVATE DjikstrasAlgorithm)