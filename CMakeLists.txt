cmake_minimum_required(VERSION 4.0)

project(opengl_sandbox VERSION 0.1.0 LANGUAGES CXX)

find_package(glfw3 3.3 REQUIRED)

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/shaders shaders SYMBOLIC)
file(CREATE_LINK ${CMAKE_SOURCE_DIR}/assets assets SYMBOLIC)

add_executable(${PROJECT_NAME})
add_subdirectory(assets)
add_subdirectory(lib)
add_subdirectory(shaders)
add_subdirectory(src)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	glad
	glm::glm-header-only
	glfw
	stb_image
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_compile_options(
	${PROJECT_NAME} PRIVATE
	-pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion -Werror
	$<$<CONFIG:Debug>:-Og -ggdb>
	$<$<CONFIG:Release>:-O2 -DNDEBUG>
)

set_target_properties(
	${PROJECT_NAME} PROPERTIES 
	DEBUG_POSTFIX "d"
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED on
	CXX_EXTENSIONS off
)
