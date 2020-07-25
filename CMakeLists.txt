cmake_minimum_required(VERSION 3.11)

# Compiler flags
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
elseif(APPLE)
  message("hello apple")
  set(CMAKE_CXX_COMPILER "clang++")
  set(CMAKE_CXX_FLAGS "${CXXFLAGS} ${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -std=c++1y --debug")
endif()

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
		string(REGEX REPLACE "^([a-zA-Z0-9_]*\/)" "" _source_path "${_source_path}")
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_filter}\\${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

# Project initialisation
project(DogFight)



#### Setup Directories ####
# Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Output all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
# Copy resources folder
file(COPY res DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY res DESTINATION ${OUTPUT_DIRECTORY}/)
file(COPY res DESTINATION ${OUTPUT_DIRECTORY}/Debug)
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

### Resources ###
file(GLOB_RECURSE RESOURCE_FILES "res/*.*")


### Engine ###
file(GLOB_RECURSE SOURCE_FILES engine/*.cpp engine/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
assign_source_group(${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_engine INTERFACE "${CMAKE_SOURCE_DIR}/engine/" PUBLIC SYSTEM ${B2D_INCS} )
target_link_libraries(lib_engine lib_maths lib_ecm Box2D sfml-graphics sfml-audio)

### Maths lib
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths" INTERFACE ${SFML_INCS})

### ECM lib
file(GLOB_RECURSE SOURCE_FILES lib_ecm/*.cpp lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
assign_source_group(${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm" )
target_link_libraries(lib_ecm PRIVATE lib_maths)

### Actual game code
file(GLOB_RECURSE SOURCES dogfight/*.cpp dogfight/*.h)

# Source files
file(GLOB_RECURSE CMPNTS dogfight/components/*.cpp dogfight/components/*.h)
file(GLOB_RECURSE SCENES dogfight/scenes/*.cpp dogfight/scenes/*.h)
file(GLOB_RECURSE ENTITIES dogfight/entities/*.cpp dogfight/entities/*.h)

# Add all to project
add_executable(DOGFIGHT ${SOURCES} ${CMPNTS} ${SCENES} ${ENTITIES} ${RESOURCE_FILES})

# Filter header/source files
assign_source_group(${SOURCES} ${CMPNTS} ${SCENES} ${ENTITIES} ${RESOURCE_FILES})

# Include and link libs
target_include_directories(DOGFIGHT SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS} ${JSON_INCS})
target_link_libraries(DOGFIGHT lib_engine)
set(EXECUTABLES ${EXECUTABLES} DOGFIGHT)