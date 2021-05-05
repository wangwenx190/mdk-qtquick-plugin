cmake_minimum_required(VERSION 3.20)

project(MDKPlayer LANGUAGES CXX)

option(BUILD_DEMO "Build MDKPlayer demo application." ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    if(WIN32)
        set(CMAKE_DEBUG_POSTFIX d)
    else()
        set(CMAKE_DEBUG_POSTFIX _debug)
    endif()
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()

if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()

if(NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Gui Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui Quick REQUIRED)
find_package(Vulkan)

set(_MDK_SDK_DIR_PATH $ENV{MDK_SDK_DIR})
if("x${_MDK_SDK_DIR_PATH}" STREQUAL "x")
    set(_MDK_SDK_DIR_PATH ${MDK_SDK_DIR})
    if("x${_MDK_SDK_DIR_PATH}" STREQUAL "x")
        message(FATAL_ERROR "You have to set MDK_SDK_DIR first! Either through environment variable or command line parameter.")
    endif()
endif()
set(_MDK_SDK_DIR_PATH ${_MDK_SDK_DIR_PATH}/lib/cmake/FindMDK.cmake)
if(EXISTS ${_MDK_SDK_DIR_PATH})
    include(${_MDK_SDK_DIR_PATH})
else()
    message(FATAL_ERROR "Can't find ${_MDK_SDK_DIR_PATH}!")
endif()

set(SOURCES
    mdkplayer_global.h
    mdkplayer.h
    mdkplayer.cpp
    mdkwrapper.h
    mdkwrapper.cpp
    videotexturenode.h
    videotexturenode.cpp
    videotexturenode_public.cpp
    videotexturenode_private.cpp
)

if(WIN32 AND BUILD_SHARED_LIBS)
    enable_language(RC)
    list(APPEND SOURCES mdkplayer.rc)
endif()

add_library(${PROJECT_NAME} ${SOURCES})
add_library(wangwenx190::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC
        MDKPLAYER_STATIC
    )
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
    if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
        target_compile_options(${PROJECT_NAME} PRIVATE /guard:cf)
        target_link_options(${PROJECT_NAME} PRIVATE /GUARD:CF)
    endif()
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
    QT_NO_KEYWORDS
    QT_DEPRECATED_WARNINGS
    QT_DISABLE_DEPRECATED_BEFORE=0x060100
    MDKPLAYER_BUILD_LIBRARY
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIN32_LEAN_AND_MEAN
        _CRT_SECURE_NO_WARNINGS
        UNICODE
        _UNICODE
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::GuiPrivate
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickPrivate
    mdk
)

if(TARGET Vulkan::Vulkan)
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${Vulkan_INCLUDE_DIRS}
    )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
)

if(BUILD_DEMO)
    add_subdirectory(example)
endif()
