# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Use Cinder CMake macros.
get_filename_component(CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../" ABSOLUTE)
include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")
include("${FinalProject_SOURCE_DIR}/cmake/make_cinder_library.cmake")


file(GLOB SOURCE_LIST CONFIGURE_DEPENDS
        "${FinalProject_SOURCE_DIR}/src/*.h"
        "${FinalProject_SOURCE_DIR}/src/*.hpp"
        "${FinalProject_SOURCE_DIR}/src/*.cc"
        "${FinalProject_SOURCE_DIR}/src/*.cpp")


ci_make_library(
        LIBRARY_NAME spaceimpact
        CINDER_PATH ${CINDER_PATH}
        SOURCES ${SOURCE_LIST}
        INCLUDES "${FinalProject_SOURCE_DIR}/include"
        LIBRARIES sqlite-modern-cpp sqlite3
        BLOCKS Box2D
)

# All users of this library will need at least C++14
target_compile_features(spaceimpact PUBLIC cxx_std_14)

set_property(TARGET spaceimpact PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Cross-platform compiler lints
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
        OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(spaceimpact PRIVATE
            -Wall
            -Wextra
            -Wswitch
            -Wconversion
            -Wparentheses
            -Wfloat-equal
            -Wzero-as-null-pointer-constant
            -Wpedantic
            -pedantic
            -pedantic-errors)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    cmake_policy(SET CMP0091 NEW)
    target_compile_options(spaceimpact PRIVATE
            /W3)
endif ()

# IDEs should put the headers in a nice place
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
