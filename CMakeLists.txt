cmake_minimum_required(VERSION 3.11)

# Compiler flags
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
elseif(APPLE)
  message("hello apple")
  set(CMAKE_CXX_COMPILER "clang++")
  set(CMAKE_CXX_FLAGS "${CXXFLAGS} ${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -std=c++1y --debug")
endif()

#GLFW options
option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_DOCUMENT_INTERNALS "" OFF)
option(GLFW_INSTALL "" OFF)
#GLEW options
option(BUILD_UTILS "" OFF)
option(BUILD_SHARED_LIBS "" ON)
option(glew-cmake_BUILD_MULTI_CONTEXT "" OFF)
option(glew-cmake_BUILD_SHARED "" ON)
option(glew-cmake_BUILD_SINGLE_CONTEXT "" ON)
option(glew-cmake_BUILD_STATIC "" ON)
#GLM options
option(GLM_DYNAMIC_LIBRARY_ENABLE "" OFF)
option(GLM_INSTALL_ENABLE "" OFF)
option(GLM_TEST_ENABLE "" OFF)

add_definitions(-DGLEW_STATIC)

set(BuildTests OFF CACHE BOOL "" FORCE)

# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Helper function for file sorting
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" DIRECTORY)
        get_filename_component(_extension "${_source_rel}" EXT)
		if(_extension MATCHES "(\.[h][p]*)")
			set(_filter "Header Files")
		elseif(_extension MATCHES "(\.[c][p]*)")
			set(_filter "Source Files")
		else()
			set(_filter "Resources")
		endif()
		message(STATUS, "File ${_source_rel} Filter ${_filter}")
		message(STATUS, "File dir ${_source_path}")
		string(REGEX REPLACE "^\\/[^\\/]*(\\/.*)$" "\\1" _source_path "/${_source_path}")
		message(STATUS, "Regex replaced path [${_source_path}]")
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
		message(STATUS, "MSVC path [${_source_path_msvc}]")
		
        source_group("${_filter}\\${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

# Project initialisation
project(PlanetProtector)

#### Setup Directories ####
# Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Output all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
# Copy resources folder
file(COPY res DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY res DESTINATION ${OUTPUT_DIRECTORY}/)
file(COPY res DESTINATION ${OUTPUT_DIRECTORY}/Debug)
file(COPY res DESTINATION ${OUTPUT_DIRECTORY}/Release)
# Copy required dll
file(COPY lib/sfml/extlibs/bin/x86/ DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY lib/sfml/extlibs/bin/x86/ DESTINATION ${OUTPUT_DIRECTORY}/)
file(COPY lib/sfml/extlibs/bin/x86/ DESTINATION ${OUTPUT_DIRECTORY}/Debug)
file(COPY lib/sfml/extlibs/bin/x86/ DESTINATION ${OUTPUT_DIRECTORY}/Release)
# Output all DLLs from all libs into main build folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})


#### Add External Dependencies ####
# SFML
add_subdirectory("lib/sfml" EXCLUDE_FROM_ALL)
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")
# Box2D
add_subdirectory("lib/b2d/Box2D" EXCLUDE_FROM_ALL)
set(B2D_INCS "lib/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/lib/b2d")
# JSON
add_subdirectory("lib/json" EXCLUDE_FROM_ALL)
set(JSON_INCS "lib/json/include")
link_directories("${CMAKE_BINARY_DIR}/lib/json/lib")
# GLFW
add_subdirectory("lib/glfw" EXCLUDE_FROM_ALL)
set(GLFW_INCS "lib/glfw/include")
link_directories("${CMAKE_BINARY_DIR}/lib/glfw/lib")
# GLM
add_subdirectory("lib/glm" EXCLUDE_FROM_ALL)
set(GLM_INCS "lib/glm/include")
link_directories("${CMAKE_BINARY_DIR}/lib/glm/lib")
# GLEW
add_subdirectory("lib/glew" EXCLUDE_FROM_ALL)
set(GLEW_INCS "lib/glew/include")
link_directories("${CMAKE_BINARY_DIR}/lib/glew/lib")

### Resources ###
file(GLOB_RECURSE RESOURCE_FILES "res/*.*")


### Engine ###
file(GLOB_RECURSE SOURCE_FILES engine/*.cpp engine/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES})
assign_source_group(${SOURCE_FILES})
target_include_directories(lib_engine INTERFACE "${CMAKE_SOURCE_DIR}/engine/" PUBLIC SYSTEM ${B2D_INCS} ${GLEW_INCS})
target_link_libraries(lib_engine 
	PUBLIC lib_maths 
	PUBLIC lib_ecm 
	PUBLIC Box2D 
	PUBLIC sfml-graphics 
	PUBLIC sfml-audio 
	PUBLIC glfw 
	PUBLIC libglew_static
	PUBLIC opengl32
)

# OpenGL
FIND_PACKAGE(OpenGL REQUIRED)
if(NOT ${OPENGL_FOUND})
  message(FATAL_ERROR "OPENGL NOT FOUND")
endif()
set(deps glfw libglew_static)

FOREACH(jnk ${junk})
	#Hide junks in junk VS project folder
	set_target_properties(${jnk} PROPERTIES FOLDER "JUNK")
ENDFOREACH()

FOREACH(dep ${deps})
	#Hide deps in dep VS project folder
	set_target_properties(${dep} PROPERTIES FOLDER "DEPS")
	#Disable warnings for deps
	target_compile_options(${dep} PUBLIC "/W0")
	#use all core compilation
	target_compile_options(${dep} PUBLIC "/MP")
ENDFOREACH()

### Maths lib
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_link_libraries(lib_maths INTERFACE glm)
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths" INTERFACE ${SFML_INCS} INTERFACE ${GLM_INCS})

### ECM lib
file(GLOB_RECURSE SOURCE_FILES lib_ecm/*.cpp lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
assign_source_group(${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm" )
target_link_libraries(lib_ecm PRIVATE lib_maths)

### Actual game code
file(GLOB_RECURSE SOURCES planet_protector/*.cpp planet_protector/*.h)

# Source files
file(GLOB_RECURSE CMPNTS planet_protector/components/*.cpp planet_protector/components/*.h)
file(GLOB_RECURSE SCENES planet_protector/scenes/*.cpp planet_protector/scenes/*.h)
file(GLOB_RECURSE ENTITIES planet_protector/entities/*.cpp planet_protector/entities/*.h)

# Add all to project
add_executable(PLANET WIN32 ${SOURCES} ${CMPNTS} ${SCENES} ${ENTITIES} ${RESOURCE_FILES})

# Filter header/source files
assign_source_group(${SOURCES} ${CMPNTS} ${SCENES} ${ENTITIES} ${RESOURCE_FILES})

# Include and link libs
target_include_directories(PLANET SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS} ${JSON_INCS})
target_link_libraries(PLANET lib_engine)
set(EXECUTABLES ${EXECUTABLES} PLANET)



#Install directory within build folder, so it doesn't go into /lib or program-files
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install/")
#Installer.exe/zip built into /installer
SET(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/installer/")

#Install component for runtime dlls and stuff
set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE) #stop this happening on non-release buildss
INCLUDE(InstallRequiredSystemLibraries)
install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "bin/" CONFIGURATIONS Release COMPONENT System )
#Install component for resources
install(DIRECTORY "${PROJECT_SOURCE_DIR}/res" DESTINATION "bin/" CONFIGURATIONS Release COMPONENT resources_cmpt)
#We disabled SFML's INSTALL component as it brings along junk, so we gotta make one ourselves
install(DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/" DESTINATION "bin" CONFIGURATIONS Release COMPONENT libs_cmpt FILES_MATCHING PATTERN "*.dll" )

#Tell CPack about the libs and resources install components
set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} resources_cmpt libs_cmpt)
#CPACK_INSTALL_CMAKE_PROJECTS IS only necessary if not using CPACK_COMPONENTS_ALL
#set(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${CMAKE_PROJECT_NAME};resources_cmpt;/)
#set(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${CMAKE_PROJECT_NAME};libs_cmpt;/)

#Install component for all labs, also tell CPack about them
foreach (exe ${EXECUTABLES})
  install(TARGETS ${exe} DESTINATION "bin" CONFIGURATIONS Release COMPONENT ${exe}_cmpt )
  #set(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${CMAKE_PROJECT_NAME};${exe}_cmpt;/)
  set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} ${exe}_cmpt)
  set(CPACK_COMPONENT_${exe}_CMPT_DEPENDS resources_cmpt libs_cmpt)
  set(CPACK_COMPONENT_${exe}_CMPT_GROUP "Labs")
endforeach ()


IF(WIN32)
  set(CPACK_GENERATOR NSIS)
  set(CPACK_NSIS_COMPONENT_INSTALL ON)
  set(CPACK_NSIS_HELP_LINK "https://github.com/edinburgh-napier/set09121")
  set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/edinburgh-napier/set09121")
  set(CPACK_NSIS_CONTACT "https://github.com/edinburgh-napier")
ELSE()
  set(CPACK_GENERATOR ZIP)
ENDIF(WIN32)


set(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "set09119")
set(CPACK_PACKAGE_VENDOR "Edinburgh Napier University")
find_file (LICENSE NAMES LICENSE LICENSE.txt PATHS  "${CMAKE_SOURCE_DIR}/../" "${CMAKE_SOURCE_DIR}" NO_DEFAULT_PATH)
if(${LICENSE})
  set(CPACK_RESOURCE_FILE_LICENSE ${LICENSE})
endif(${LICENSE})
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")

INCLUDE(CPack)