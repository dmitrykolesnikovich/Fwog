# the project's main CMakeLists file

cmake_minimum_required(VERSION 3.15)

project(Fwog)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external)

set(fwog_source_files
	src/Buffer.cpp
	src/DebugMarker.cpp
	src/Fence.cpp
	src/Shader.cpp
	src/Texture.cpp
	src/Rendering.cpp
	src/Pipeline.cpp
	src/Timer.cpp
	src/detail/ApiToEnum.cpp
	src/detail/PipelineManager.cpp
	src/detail/FramebufferCache.cpp
	src/detail/SamplerCache.cpp
	src/detail/VertexArrayCache.cpp
)

set(fwog_header_files
	include/Fwog/Common.h
	include/Fwog/BasicTypes.h
	include/Fwog/Buffer.h
	include/Fwog/DebugMarker.h
	include/Fwog/Fence.h
	include/Fwog/Shader.h
	include/Fwog/Texture.h
	include/Fwog/Rendering.h
	include/Fwog/Pipeline.h
	include/Fwog/Timer.h
	include/Fwog/Exception.h
	include/Fwog/detail/Flags.h
	include/Fwog/detail/ApiToEnum.h
	include/Fwog/detail/PipelineManager.h
	include/Fwog/detail/FramebufferCache.h
	include/Fwog/detail/Hash.h
	include/Fwog/detail/SamplerCache.h
	include/Fwog/detail/VertexArrayCache.h
)

add_library(fwog ${fwog_source_files} ${fwog_header_files})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT fwog)
target_include_directories(fwog PUBLIC include)

find_package(OpenGL REQUIRED)

# enable asan for debug builds
if (DEBUG)
    if (WIN32)
        target_compile_options(fwog PUBLIC /fsanitize=address)
    else()
        target_compile_options(fwog PUBLIC -fsanitize=address)
    endif()
endif()

# Determine whether we're compiling with clang++
string(FIND "${CMAKE_CXX_COMPILER}" "clang++" COMPILER_CLANGPP)
if(COMPILER_CLANGPP GREATER -1)
    set(COMPILER_CLANGPP 1)
else()
    set(COMPILER_CLANGPP 0)
endif()

target_compile_options(fwog
	INTERFACE
	$<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>,${COMPILER_CLANGPP}>:
	-Wall
	-Wextra
	-pedantic-errors
	-Wno-missing-field-initializers
	-Wno-unused-result
	#-Werror
	#-Wconversion
	#-Wsign-conversion
	>
	$<$<CXX_COMPILER_ID:MSVC>:
	/W4
	/WX
	/permissive-
	/wd4324 # structure was padded
	>
)

option (FWOG_FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()


target_link_libraries(fwog lib_glad)

option (FWOG_BUILD_EXAMPLES "Build the example projects for Fwog." TRUE)
if (${FWOG_BUILD_EXAMPLES})
	add_subdirectory(example)
endif ()