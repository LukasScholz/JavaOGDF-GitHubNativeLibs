# Install script for directory: /mnt/c/Users/lkass/Documents/GitHub/JavaOGDF/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/mnt/c/Users/lkass/Documents/GitHub/JavaOGDF/src/../build/native/l64")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/mnt/c/Users/lkass/Documents/GitHub/JavaOGDF/src/../build/native/l64/libOGDF_JNI_Wrapper.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/mnt/c/Users/lkass/Documents/GitHub/JavaOGDF/src/../build/native/l64" TYPE STATIC_LIBRARY FILES "/mnt/c/Users/lkass/Documents/GitHub/JavaOGDF/build/src/libOGDF_JNI_Wrapper.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  set(INSTALL_PATH "/mnt/c/Users/lkass/Documents/GitHub/JavaOGDF/src/../build/native/l64")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  set(MACOS "")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  set(WINDOWS "")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
    file(GET_RUNTIME_DEPENDENCIES
    RESOLVED_DEPENDENCIES_VAR RESOLVED
    UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED
    CONFLICTING_DEPENDENCIES_PREFIX CONF
    LIBRARIES /mnt/c/Users/lkass/Documents/GitHub/JavaOGDF/build/src/libOGDF_JNI_Wrapper.a)
  foreach (found_dep IN LISTS RESOLVED)
    # copy only these libs
    if (found_dep MATCHES ".*([Cc]bc|[Cc]lp|[Cc]oin|gcc_s|gfortran|lapack|quadmath|c\\+\\+).*"
	OR (MACOS AND found_dep MATCHES ".*gmp.*"))
      # TODO: WINDOWS: copy manually for now
      # copy all depencencies only as major version
      if (IS_SYMLINK "${found_dep}")
	file(READ_SYMLINK ${found_dep} found_dep_res)
	if(NOT IS_ABSOLUTE "${found_dep_res}")
	  get_filename_component(dir ${found_dep} DIRECTORY)
	  set(found_dep_res "${dir}/${found_dep_res}")
	endif()
	file(INSTALL ${found_dep_res} DESTINATION ${INSTALL_PATH})
	get_filename_component(found_dep ${found_dep} NAME)
	get_filename_component(found_dep_res ${found_dep_res} NAME)
	file(RENAME "${INSTALL_PATH}/${found_dep_res}" "${INSTALL_PATH}/${found_dep}")
	message("renaming ${found_dep_res} to ${found_dep}")
      else()
	file(INSTALL ${found_dep} DESTINATION ${INSTALL_PATH})
	get_filename_component(found_dep ${found_dep} NAME)
      endif()
      # adapt rpath
      # TODO: MACOS: install_name_tool; for now use macutils/int in native/osx folder
      # Windows: not needed
      if (NOT MACOS AND NOT WINDOWS)
	execute_process(COMMAND patchelf --set-rpath ".:$ORIGIN" ${found_dep}
	  WORKING_DIRECTORY ${INSTALL_PATH})
      endif()
    endif()
  endforeach()
  
endif()

