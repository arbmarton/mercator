cmake_minimum_required(VERSION 3.9)
project(mercator)

# Definitions to use STB_image
add_definitions(-DSTB_IMAGE_IMPLEMENTATION -DSTB_IMAGE_STATIC)

set(GLM_TEST_ENABLE OFF)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/glfw-3.3/)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/glm/)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/glad/)

set(VERTEX_SHADERS
	shader/displaymap.vs
	shader/litsphere.vs
	shader/unlitsphere.vs
)

set(FRAGMENT_SHADERS
	shader/displaymap.fs
	shader/litsphere.fs
	shader/unlitsphere.fs
)

set(THIRDPARTY_HEADERS
	thirdparty/stb_image/stb_image.h
)

set(HEADER_FILES
	include/Camera.h
	include/DisplayMap.h
	include/Shader.h
	include/StellarObject.h
	include/Utilities.h
	include/UVSphere.h
)

set(SOURCE_FILES
	src/Camera.cpp
	src/DisplayMap.cpp
	src/main.cpp
	src/Shader.cpp
	src/StellarObject.cpp
	src/Utilities.cpp
	src/UVSphere.cpp
)

add_executable(${PROJECT_NAME}
			   ${THIRDPARTY_HEADERS}
			   ${HEADER_FILES}
			   ${SOURCE_FILES}
			   ${VERTEX_SHADERS}
			   ${FRAGMENT_SHADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
						   ${CMAKE_CURRENT_LIST_DIR}/include
						   ${CMAKE_CURRENT_LIST_DIR}/thirdparty/glm/glm/
						   ${CMAKE_CURRENT_LIST_DIR}/thirdparty/glfw-3.3/include/
						   ${CMAKE_CURRENT_LIST_DIR}/thirdparty/glad/include/
						   ${CMAKE_CURRENT_LIST_DIR}/thirdparty/stb_image/
)

target_link_libraries(${PROJECT_NAME} 
					  ${CMAKE_CURRENT_LIST_DIR}/out/thirdparty/glm/glm/$<CONFIG>/glm_static.lib
					  ${CMAKE_CURRENT_LIST_DIR}/out/thirdparty/glfw-3.3/src/$<CONFIG>/glfw3.lib
					  ${CMAKE_CURRENT_LIST_DIR}/out/thirdparty/glad/$<CONFIG>/glad.lib
)

add_dependencies(${PROJECT_NAME} glfw)

source_group("shaders" FILES ${VERTEX_SHADERS} ${FRAGMENT_SHADERS})
source_group("thirdparty" FILES ${THIRDPARTY_HEADERS})
				
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Parallel compilation and C++17
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /MP")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4554")
endif(MSVC)


# Clang format setup
# http://mariobadr.com/using-clang-format-to-enforce-style.html
find_program(CLANG_FORMAT_EXE NAMES "clang-format")
if(NOT CLANG_FORMAT_EXE)
	message(STATUS "clang-format not found.")
else()
	message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")
	set(DO_CLANG_FORMAT "${CLANG_FORMAT_EXE}" "-i -style=file")
endif()

function(prepend var prefix)
	set(listVar "")
	
	foreach(f ${ARGN})
	  list(APPEND listVar "${prefix}/${f}")
	endforeach()
	
	set(${var} "${listVar}" PARENT_SCOPE)
endfunction()

if(CLANG_FORMAT_EXE)
	prepend(FILES_TO_FORMAT ${CMAKE_CURRENT_SOURCE_DIR} ${HEADER_FILES} ${SOURCE_FILES})
	
	add_custom_target(
		CLANG_FORMAT
		COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${FILES_TO_FORMAT}
	)
endif()