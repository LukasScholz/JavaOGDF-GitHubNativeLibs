
project(JavaOGDF)

find_package(JNI REQUIRED)

include_directories(${JNI_INCLUDE_DIRS})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(ogdf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ogdf")

include_directories(${JAVA_HOME}/include)

find_package(ogdf)
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

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tmap)

 #add_subdirectory(test) 
