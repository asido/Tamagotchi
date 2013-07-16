cmake_minimum_required(VERSION 2.8.8)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(3RD_PARTY_DIR ${SOURCE_DIR}/3rdParty)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

set(PROJECT_NAME "Tamagotchi")
project(${PROJECT_NAME})


###############################################################################
## 3rd party libraries

include_directories(
    ${SOURCE_DIR}/Engine
    ${SOURCE_DIR}/EngineTests
    ${SOURCE_DIR}/Tamagotchi
    ${3RD_PARTY_DIR}/Mali-GLES-emulator-1.2.0/include
    ${3RD_PARTY_DIR}/cppunit/include
    ${3RD_PARTY_DIR}/eigen-3.1.3/include
    ${3RD_PARTY_DIR}/FastDelegate/include
    ${3RD_PARTY_DIR}/freetype-2.5.0.1/include
    ${3RD_PARTY_DIR}/libpng-1.6.2/include
    ${3RD_PARTY_DIR}/tinyxml2/include
    ${3RD_PARTY_DIR}/zlib/include)

if (MSVC)
    find_library(LIB_CPPUNIT_RELEASE    NAMES cppunit       PATHS ${3RD_PARTY_DIR}/cppunit)
    find_library(LIB_CPPUNIT_DEBUG      NAMES cppunitd      PATHS ${3RD_PARTY_DIR}/cppunit)
    find_library(LIB_FREETYPE_RELEASE   NAMES freetype      PATHS ${3RD_PARTY_DIR}/freetype-2.5.0.1)
    find_library(LIB_FREETYPE_DEBUG     NAMES freetyped     PATHS ${3RD_PARTY_DIR}/freetype-2.5.0.1)
    find_library(LIB_PNG_RELEASE        NAMES libpng        PATHS ${3RD_PARTY_DIR}/libpng-1.6.2)
    find_library(LIB_PNG_DEBUG          NAMES libpngd       PATHS ${3RD_PARTY_DIR}/libpng-1.6.2)
    find_library(LIB_EGL_RELEASE        NAMES EGL           PATHS ${3RD_PARTY_DIR}/Mali-GLES-emulator-1.2.0)
    find_library(LIB_EGL_DEBUG          NAMES EGL           PATHS ${3RD_PARTY_DIR}/Mali-GLES-emulator-1.2.0)
    find_library(LIB_GLES_RELEASE       NAMES GLESv2        PATHS ${3RD_PARTY_DIR}/Mali-GLES-emulator-1.2.0)
    find_library(LIB_GLES_DEBUG         NAMES GLESv2        PATHS ${3RD_PARTY_DIR}/Mali-GLES-emulator-1.2.0)
    find_library(LIB_TINYXML2_RELEASE   NAMES tinyxml2      PATHS ${3RD_PARTY_DIR}/tinyxml2)
    find_library(LIB_TINYXML2_DEBUG     NAMES tinyxml2d     PATHS ${3RD_PARTY_DIR}/tinyxml2)
    find_library(LIB_ZLIB_RELEASE       NAMES zlib          PATHS ${3RD_PARTY_DIR}/zlib)
    find_library(LIB_ZLIB_DEBUG         NAMES zlibd         PATHS ${3RD_PARTY_DIR}/zlib)
elseif (APPLE)
    find_library(LIB_CPPUNIT_RELEASE    NAMES libcppunit    PATHS ${3RD_PARTY_DIR}/cppunit)
    find_library(LIB_CPPUNIT_DEBUG      NAMES libcppunit    PATHS ${3RD_PARTY_DIR}/cppunit)
    find_library(LIB_PNG_RELEASE        NAMES libpng        PATHS ${3RD_PARTY_DIR}/libpng-1.6.2)
    find_library(LIB_PNG_DEBUG          NAMES libpng        PATHS ${3RD_PARTY_DIR}/libpng-1.6.2)
    find_library(LIB_TINYXML2_RELEASE   NAMES libtinyxml2   PATHS ${3RD_PARTY_DIR}/tinyxml2)
    find_library(LIB_TINYXML2_DEBUG     NAMES libtinyxml2   PATHS ${3RD_PARTY_DIR}/tinyxml2)
    find_library(LIB_ZLIB_RELEASE       NAMES libz          PATHS ${3RD_PARTY_DIR}/zlib)
    find_library(LIB_ZLIB_DEBUG         NAMES libz          PATHS ${3RD_PARTY_DIR}/zlib)
endif (MSVC)


###############################################################################
## Engine

file(GLOB ENGINE_ACTORS_SRCS            ${SOURCE_DIR}/Engine/Actors/*.cpp)
file(GLOB ENGINE_ACTORS_HDRS            ${SOURCE_DIR}/Engine/Actors/*.h)
file(GLOB ENGINE_EVENT_MANAGER_SRCS     ${SOURCE_DIR}/Engine/EventManager/*.cpp)
file(GLOB ENGINE_EVENT_MANAGER_HDRS     ${SOURCE_DIR}/Engine/EventManager/*.h)
file(GLOB ENGINE_RENDERING_SRCS         ${SOURCE_DIR}/Engine/Rendering/*.cpp)
file(GLOB ENGINE_RENDERING_HDRS         ${SOURCE_DIR}/Engine/Rendering/*.h)
file(GLOB ENGINE_RESOURCE_MANAGER_SRCS  ${SOURCE_DIR}/Engine/ResourceManager/*.cpp)
file(GLOB ENGINE_RESOURCE_MANAGER_HDRS  ${SOURCE_DIR}/Engine/ResourceManager/*.h)
file(GLOB ENGINE_THREADING_SRCS         ${SOURCE_DIR}/Engine/Threading/*.cpp)
file(GLOB ENGINE_THREADING_HDRS         ${SOURCE_DIR}/Engine/Threading/*.h)
file(GLOB ENGINE_UI_SRCS                ${SOURCE_DIR}/Engine/UI/*.cpp)
file(GLOB ENGINE_UI_HDRS                ${SOURCE_DIR}/Engine/UI/*.h)
file(GLOB ENGINE_SRCS                   ${SOURCE_DIR}/Engine/*.cpp)
file(GLOB ENGINE_HDRS                   ${SOURCE_DIR}/Engine/*.h)

add_library(Engine STATIC
    ${ENGINE_ACTORS_SRCS}
    ${ENGINE_ACTORS_HDRS}
    ${ENGINE_EVENT_MANAGER_SRCS}
    ${ENGINE_EVENT_MANAGER_HDRS}
    ${ENGINE_RENDERING_SRCS}
    ${ENGINE_RENDERING_HDRS}
    ${ENGINE_RESOURCE_MANAGER_SRCS}
    ${ENGINE_RESOURCE_MANAGER_HDRS}
    ${ENGINE_THREADING_SRCS}
    ${ENGINE_THREADING_HDRS}
    ${ENGINE_UI_SRCS}
    ${ENGINE_UI_HDRS}
    ${ENGINE_SRCS}
    ${ENGINE_HDRS})

target_link_libraries(Engine
    debug ${LIB_TINYXML2_DEBUG}
    debug ${LIB_PNG_DEBUG}
    debug ${LIB_ZLIB_DEBUG}
    debug ${LIB_FREETYPE_DEBUG}
    debug ${LIB_GLES_DEBUG}
    optimized ${LIB_TINYXML2_RELEASE}
    optimized ${LIB_PNG_RELEASE}
    optimized ${LIB_ZLIB_RELEASE}
    optimized ${LIB_FREETYPE_RELEASE}
    optimized ${LIB_GLES_RELEASE})


###############################################################################
## Engine Unit Tests

file(GLOB ENGINE_TESTS_SRCS ${SOURCE_DIR}/EngineTests/*.cpp)
file(GLOB ENGINE_TESTS_HDRS ${SOURCE_DIR}/EngineTests/*.h)

if (MSVC)
    file(GLOB ENGINE_TESTS_MSVC_SRCS ${SOURCE_DIR}/EngineTests/win32/*.cpp)
    file(GLOB ENGINE_TESTS_MSVC_HDRS ${SOURCE_DIR}/EngineTests/win32/*.h)
endif(MSVC)

add_executable(EngineTests
    ${ENGINE_TESTS_SRCS}
    ${ENGINE_TESTS_HDRS}
    ${ENGINE_TESTS_MSVC_SRCS}
    ${ENGINE_TESTS_MSVC_HDRS})

target_link_libraries(EngineTests
    Engine
    debug ${LIB_CPPUNIT_DEBUG}
    debug ${LIB_EGL_DEBUG}
    optimized ${LIB_CPPUNIT_RELEASE}
    optimized ${LIB_EGL_RELEASE})


###############################################################################
## Tamagotchi

file(GLOB TAMAGOTCHI_SRCS ${SOURCE_DIR}/Tamagotchi/*.cpp)
file(GLOB TAMAGOTCHI_HDRS ${SOURCE_DIR}/Tamagotchi/*.h)

if (MSVC)
    file(GLOB TAMAGOTCHI_MSVC_SRCS ${SOURCE_DIR}/Tamagotchi/win32/*.cpp)
    file(GLOB TAMAGOTCHI_MSVC_HDRS ${SOURCE_DIR}/Tamagotchi/win32/*.h)
endif(MSVC)

add_executable(${PROJECT_NAME}
    ${TAMAGOTCHI_SRCS}
    ${TAMAGOTCHI_HDRS}
    ${TAMAGOTCHI_MSVC_SRCS}
    ${TAMAGOTCHI_MSVC_HDRS})

target_link_libraries(${PROJECT_NAME}
    Engine
    debug ${LIB_EGL_DEBUG}
    optimized ${LIB_EGL_RELEASE})


###############################################################################
## Preprocessor

function(create_definitions target)
    # Define DEBUG during debug builds.
    list(APPEND DEBUG_DEFINES "DEBUG")
    # Eigen does the alignment by overriding 'new' keyword which is not compatible
    # with 'TG_NEW' and so during debug builds we disable the alignment.
    # Note that this makes Eigen linear algebra *very* slow.
    list(APPEND DEBUG_DEFINES "EIGEN_DONT_VECTORIZE")
    list(APPEND DEBUG_DEFINES "EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT")

    set_target_properties(${target} PROPERTIES COMPILE_DEFINITIONS_DEBUG "${DEBUG_DEFINES}")
endfunction()

create_definitions(Engine)
create_definitions(EngineTests)
create_definitions(Tamagotchi)

# Windows require UNICODE.
add_definitions(-DUNICODE)
# Preserve VS warnings when using unsafe standard lib C functions.
add_definitions(-D_CRT_SECURE_NO_WARNINGS)


###############################################################################
## Linker

if (MSVC)
    # VS linker gives errors because some lib image safe exception handlers cannot be produced.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
endif(MSVC)