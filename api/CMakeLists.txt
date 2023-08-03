project(JavaOGDF)

# cmake flags/requirements
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for IDEs
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # for compiling shared lib
cmake_minimum_required (VERSION 3.18)
set(CMAKE_CXX_FLAGS "-fpermissive") # DEBUG, remove later
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# determine architecture
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
  set (TARGET_ARCH "64")
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86"
    OR CMAKE_SYSTEM_PROCESSOR STREQUAL "i386"
    OR CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
  set (TARGET_ARCH "32")
endif()

# determine host platform
if (CMAKE_HOST_WIN32)
  set(WINDOWS 1)
elseif(CMAKE_HOST_APPLE)
  set(MACOS 1)
elseif(CMAKE_HOST_UNIX)
  set(LINUX 1)
endif()

add_subdirectory(extern)
find_package(./extern/cmake/ogdf.cmake)
add_subdirectory(src)
# add_subdirectory(test)