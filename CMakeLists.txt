cmake_minimum_required(VERSION 3.9)
project(mercator)

add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

set(GLM_TEST_ENABLE OFF)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/glfw-3.3/)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/glm/)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/glad/)

set(VERTEX_SHADERS
	shader/test.vs
)

set(FRAGMENT_SHADERS
	shader/test.fs
)

set(THIRDPARTY_HEADERS
	thirdparty/stb_image/stb_image.h
)

set(HEADER_FILES
	include/Converter.h
	include/Coordinate.h
	include/MercatorMath.h
	include/Shader.h
	include/Sphere.h
	include/Utilities.h
)

set(SOURCE_FILES
	src/Converter.cpp
	src/Coordinate.cpp
	src/MercatorMath.cpp
	src/main.cpp
	src/Shader.cpp
	src/Sphere.cpp
	src/Utilities.cpp
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

source_group("shaders" FILES ${VERTEX_SHADERS} ${FRAGMENT_SHADERS})
source_group("thirdparty" FILES ${THIRDPARTY_HEADERS})
				
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /MP")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4554")
endif(MSVC)