cmake_minimum_required(VERSION 3.5)
project(gg-engine)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(lib/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(lib/assimp)

#catch, glm, and stb_image are header only libraries

include_directories(src/)
include_directories(SYSTEM
    lib/assimp/include/
    lib/glad/
    lib/glfw/include/
    lib/glm/
    lib/
)

file(GLOB_RECURSE PROJECT_SRC
    src/*.c
    src/*.h
    src/*.cpp
    src/*.hpp
)
file(GLOB_RECURSE PROJECT_RES
    res/*
)
file(GLOB_RECURSE PROJECT_TEST
    test/*.cpp
    test/*.c
)
file(GLOB_RECURSE PROJECT_DOC
    doc/*
    .gitignore
    CMakeLists.txt
    README.md
)

source_group("test" FILES ${PROJECT_TEST})
source_group("doc" FILES ${PROJECT_DOC})
source_group("res" FILES ${PROJECT_RES})
source_group("src" FILES ${PROJECT_SRC})
source_group("lib" FILES ${PROJECT_LIB})

add_definitions(
    -DGLFW_INCLUDE_NONE
    -DPROJECT_SRC_DIR=\"${PROJECT_SOURCE_DIR}\"
)
if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_definitions(-DDEBUG)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /std:c++latest")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    #alternate for clang & gcc: -Wall -Wextra -pedantic -std=c++17 -Werror
    #Keep the following, though; it took forever to compile.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        -Wall -Wextra -Wpedantic -std=c++17 -Werror -Wfatal-errors\
        -Wold-style-cast -Wcast-align -Wcast-qual -Wuseless-cast \
        -Wconversion -Wsign-conversion -Wsign-promo -Wsign-promo \
        -Wdisabled-optimization -Wstrict-overflow=5 -Wabi \
        -Wfloat-equal -Wlogical-op \
        -Wmissing-declarations -Wmissing-include-dirs -Wundef \
        -Wformat=2 -Wzero-as-null-pointer-constant -Wstrict-null-sentinel \
        -Wredundant-decls -Wshadow -Wunused \
        -Woverloaded-virtual -Wctor-dtor-privacy")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    # DUno the flags
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        -Wall -Wextra -Wpedantic -std=c++17 -Werror")
endif()
if(NOT WIN32)
    set(DLLOAD dl)
endif()

#to avoid compiler warnings i seperately build these
add_library(glad ${PROJECT_SOURCE_DIR}/lib/glad/glad.c)
add_library(stb_image ${PROJECT_SOURCE_DIR}/lib/stb_image.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SRC} ${PROJECT_LIB})

target_link_libraries(
    ${PROJECT_NAME}
    assimp
    glad
    stb_image
    glfw ${GLFW_LIBRARIES}
    ${DLLOAD}
    #catch, glm, stb_image are header only
)

set_property(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
    PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
