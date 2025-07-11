cmake_minimum_required(VERSION 3.15)

project(Verovio)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Options
option(VRV_DYNAMIC_CAST         "Enable <dynamic_cast>"                        OFF)
option(NO_DARMS_SUPPORT         "Disable DARMS support"                        ON)
option(NO_PAE_SUPPORT           "Disable Plaine & Easie support"               OFF)
option(NO_ABC_SUPPORT           "Disable ABC support"                          OFF)
option(NO_MUSICXML_SUPPORT      "Disable MusicXML support"                     OFF)
option(NO_MXL_SUPPORT           "Disable compressed MusicXML support"          OFF)
option(NO_HUMDRUM_SUPPORT       "Disable Humdrum support"                      OFF)
option(MUSICXML_DEFAULT_HUMDRUM "Enable MusicXML to Humdrum by default"        OFF)
option(NO_EDIT_SUPPORT          "Disable editor code"                          OFF)
option(NO_RUNTIME               "Disable runtime clock support"                ON)
option(BUILD_AS_LIBRARY         "Build Verovio as library"                     OFF)
option(BUILD_AS_ANDROID_LIBRARY "Build Verovio as library for Android"         OFF)
option(USE_PAE_OLD_PARSER       "Use old PAE parser"                           OFF)

if (NO_HUMDRUM_SUPPORT AND MUSICXML_DEFAULT_HUMDRUM)
    message(FATAL_ERROR "Default MusicXML to Humdrum cannot be enabled by default without Humdrum support")
endif()

# Compile definitions
set(COMPILE_DEFS
    RESOURCE_DIR="${CMAKE_INSTALL_PREFIX}/share/verovio"
)

if(VRV_DYNAMIC_CAST)
    list(APPEND COMPILE_DEFS VRV_DYNAMIC_CAST)
endif()
if(NO_DARMS_SUPPORT)
    list(APPEND COMPILE_DEFS NO_DARMS_SUPPORT)
endif()
if(NO_PAE_SUPPORT)
    list(APPEND COMPILE_DEFS NO_PAE_SUPPORT)
elseif(USE_PAE_OLD_PARSER)
    list(APPEND COMPILE_DEFS USE_PAE_OLD_PARSER)
endif()
if(NO_MUSICXML_SUPPORT)
    list(APPEND COMPILE_DEFS NO_MUSICXML_SUPPORT)
endif()
if(NO_MXL_SUPPORT)
    list(APPEND COMPILE_DEFS NO_MXL_SUPPORT)
endif()
if(NO_HUMDRUM_SUPPORT)
    list(APPEND COMPILE_DEFS NO_HUMDRUM_SUPPORT)
elseif(MUSICXML_DEFAULT_HUMDRUM)
    list(APPEND COMPILE_DEFS MUSICXML_DEFAULT_HUMDRUM)
endif()
if(NO_EDIT_SUPPORT)
    list(APPEND COMPILE_DEFS NO_EDIT_SUPPORT)
endif()
if(NO_RUNTIME)
    list(APPEND COMPILE_DEFS NO_RUNTIME)
endif()

# Includes
set(INCLUDE_DIRS
    ../include
    ../include/crc
    ../include/midi
    ../include/hum
    ../include/json
    ../include/pugi
    ../include/zip
    ../include/vrv
    ../libmei/dist
    ../libmei/addons
)

if(WIN32)
    list(APPEND INCLUDE_DIRS ../include/win32)
endif()

# Source files
file(GLOB verovio_SRC "../src/*.cpp")
file(GLOB libmei_dist_SRC "../libmei/dist/*.cpp")
file(GLOB libmei_addons_SRC "../libmei/addons/*.cpp")
file(GLOB midi_SRC "../src/midi/*.cpp")
file(GLOB crc_SRC "../src/crc/*.cpp")
file(GLOB hum_SRC "../src/hum/*.cpp")
set(all_SRC
    ${verovio_SRC}
    ${libmei_dist_SRC}
    ${libmei_addons_SRC}
    ${hum_SRC}
    ${crc_SRC}
    ${midi_SRC}
    ../src/json/jsonxx.cc
    ../src/pugi/pugixml.cpp
)

# Header für IDEs
file(GLOB_RECURSE LibFiles "../include/*.h")
add_custom_target(headers SOURCES ${LibFiles})

# Build-Typen
if (BUILD_AS_LIBRARY OR BUILD_AS_ANDROID_LIBRARY)
    add_library(verovio SHARED "../tools/c_wrapper.cpp" ${all_SRC})
elseif (BUILD_AS_WASM)
    add_library(verovio STATIC ../tools/c_wrapper.cpp ${all_SRC})
elseif (BUILD_AS_PYTHON)
    find_package(SWIG REQUIRED)
    include(${SWIG_USE_FILE})
    set(CMAKE_SWIG_FLAGS "")
    find_package(Python COMPONENTS Interpreter Development)
    include_directories(${Python_INCLUDE_DIRS})
    swig_add_library(verovio_module LANGUAGE python TYPE MODULE SOURCES ../bindings/python/verovio.i)
    swig_link_libraries(verovio_module verovio ${Python_LIBRARIES})
    add_library(verovio STATIC ${all_SRC})
else()
    add_executable(verovio ../tools/main.cpp ${all_SRC})
endif()

# Ziel-spezifische Einstellungen
if(TARGET verovio)
    target_include_directories(verovio PRIVATE ${INCLUDE_DIRS})
    target_compile_definitions(verovio PRIVATE ${COMPILE_DEFS})
    target_compile_options(verovio PRIVATE -Wall -Wpedantic -Wno-unused-parameter -std=c++20)
endif()

if (BUILD_AS_ANDROID_LIBRARY)
    find_library(log-lib log)
    target_link_libraries(verovio PRIVATE ${log-lib})
endif()

install(TARGETS verovio
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)
install(
    DIRECTORY ../data/
    DESTINATION share/verovio
    FILES_MATCHING PATTERN "*.xml" PATTERN "*.svg" PATTERN "*.css"
)
if (BUILD_AS_LIBRARY)
    file(GLOB_RECURSE all_HEADERS "../include/*/*.h*" "../libmei/*/*.h*" "../tools/c_wrapper.h")
    install(FILES ${all_HEADERS} DESTINATION include/verovio)
endif()
