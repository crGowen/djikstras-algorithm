cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(OUTDIR "${CMAKE_SOURCE_DIR}/output")

# Print building info
message("\nConfiguration:")
message("\tOS: ${CMAKE_HOST_SYSTEM_NAME}")
message("\tPlatform: ${CMAKE_GENERATOR_PLATFORM}")
message("\tDirectory: ${CMAKE_SOURCE_DIR}\n")

# Building djikalg library
project(djikalg)
add_library(djikalg SHARED ./djikalg/main.cxx)

set(DJIKALG_LIB_INCLUDE "${CMAKE_SOURCE_DIR}/djikalg/include")
target_include_directories(djikalg PRIVATE "${DJIKALG_LIB_INCLUDE}")
target_compile_definitions(djikalg PRIVATE DJIKALG_EXPORT)
add_custom_command(
    TARGET djikalg POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_LINKER_FILE:djikalg>"
        "${OUTDIR}/lib/$<TARGET_LINKER_FILE_NAME:djikalg>"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${DJIKALG_LIB_INCLUDE}"
        "${OUTDIR}/include"
)
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    # .dll copy for Windows
    add_custom_command(
        TARGET djikalg POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy
            "$<TARGET_FILE:djikalg>"
            "${OUTDIR}/$<TARGET_FILE_NAME:djikalg>"
    )
endif()

# Building usage example
project(usage_example)
add_executable(usage_example ./usage_example/main.cxx)

add_dependencies(usage_example djikalg)
target_include_directories(usage_example PRIVATE "${OUTDIR}/include")
target_link_libraries(usage_example PRIVATE "${OUTDIR}/lib/$<TARGET_LINKER_FILE_NAME:djikalg>")
add_custom_command(
    TARGET usage_example POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:usage_example>"
        "${OUTDIR}/$<TARGET_FILE_NAME:usage_example>"
)

# Building test
project(test)
add_executable(test ./test/main.cxx)

add_dependencies(test djikalg)
target_include_directories(test PRIVATE "${OUTDIR}/include")
target_link_libraries(test PRIVATE "${OUTDIR}/lib/$<TARGET_LINKER_FILE_NAME:djikalg>")
add_custom_command(
    TARGET test POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:test>"
        "${OUTDIR}/$<TARGET_FILE_NAME:test>"
)