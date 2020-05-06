cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
	LearningDXCAPI
	LANGUAGES CXX
	VERSION 0.0.1
)

add_executable(
	LearningDXCAPI
	main.cpp
)

target_compile_definitions(
	LearningDXCAPI PRIVATE
	WORKING_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
)

target_include_directories(
	LearningDXCAPI PRIVATE
	include
)

target_link_directories(
	LearningDXCAPI PRIVATE
	$<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/lib/Debug>
	$<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/lib/Release>
)

target_link_libraries(
	LearningDXCAPI PRIVATE
	dxcompiler.lib
)

set_target_properties(
	LearningDXCAPI PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug/"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/Release/"
)
