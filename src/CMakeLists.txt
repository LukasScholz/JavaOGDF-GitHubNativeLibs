project(Java_OGDF_JNI CXX)

find_package(JNI)

if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()

find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})
set(HEADER "de_unijena_bioinf_TreeVisualization_TreeVisualizer.h")
add_library(OGDF_JNI_Wrapper STATIC LayoutFromEdgeList_internal.cpp ${HEADER})
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ogdf")
target_include_directories(OGDF_JNI_Wrapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if (STATIC_STD)
  set(EXTRA_STATIC_LINKS "-static-libgcc -static-libstdc++")
  message("STATIC_STD set: ${EXTRA_STATIC_LINKS}")
endif()

target_include_directories(OGDF_JNI_Wrapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(OGDF_JNI_Wrapper PUBLIC "../tmap/src/_tmap/")
target_include_directories(OGDF_JNI_Wrapper PUBLIC "../ogdf")
target_include_directories(OGDF_JNI_Wrapper PUBLIC "../ogdf/include")
target_include_directories(OGDF_JNI_Wrapper PUBLIC "/usr/lib/jvm/default-java/include")
target_include_directories(OGDF_JNI_Wrapper PUBLIC "/usr/lib/jvm/default-java/include/linux")

# find_package("../ogdf")

if (STATIC_STD)
  set(EXTRA_STATIC_LINKS "-static-libgcc -static-libstdc++")
  message("STATIC_STD set: ${EXTRA_STATIC_LINKS}")
endif()

target_link_libraries(OGDF_JNI_Wrapper PUBLIC ${EXTRA_STATIC_LINKS})

# install path corresponding to os/arch
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/../build/")
set(MAVEN_PATH "native")
if(WINDOWS)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}${MAVEN_PATH}/w${TARGET_ARCH}")
elseif(LINUX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}${MAVEN_PATH}/l${TARGET_ARCH}")
elseif(MACOS)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}${MAVEN_PATH}/osx")
endif()

set_target_properties(OGDF_JNI_Wrapper PROPERTIES INSTALL_RPATH ".:$ORIGIN")
# install wrapper lib
install (TARGETS OGDF_JNI_Wrapper DESTINATION ${CMAKE_INSTALL_PREFIX})
# install dependencies
# INSTALL_PATH has to be set manually; otherwise not available at install time
cmake_policy(SET CMP0087 NEW)	# allows generator expressions in install commands
install(CODE "set(INSTALL_PATH \"${CMAKE_INSTALL_PREFIX}\")")
install(CODE "set(MACOS \"${MACOS}\")")
install(CODE "set(WINDOWS \"${WINDOWS}\")")
install(CODE [[
  file(GET_RUNTIME_DEPENDENCIES
    RESOLVED_DEPENDENCIES_VAR RESOLVED
    UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED
    CONFLICTING_DEPENDENCIES_PREFIX CONF
    LIBRARIES $<TARGET_FILE:OGDF_JNI_Wrapper>)
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
  ]])
