cmake_minimum_required(VERSION 3.16) # Bumped to 3.16 for built-in, robust Unity Build support
project(OpenGLProject VERSION 1.0 LANGUAGES C CXX)

option(OPTIMIZE "Enable aggressive optimization" OFF)
option(ENABLE_UNITY_BUILD "Enable Unity (Jumbo) builds for compilation speed" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Turning this OFF. Verbose makefiles flood the console, slowing down the IDE terminal output.
set(CMAKE_VERBOSE_MAKEFILE OFF)

# ==============================================================================
# Compiler Flags & Optimization Setup
# ==============================================================================
if(MSVC)
	add_compile_options(/EHs-c- /GR-) # Disable exceptions and RTTI for engine performance
	if(OPTIMIZE)
		add_compile_options(/O2 /Oi /Ot /GL) # Max speed, intrinsic functions, favor fast code, LTCG
		add_link_options(/LTCG)
	else()
		add_compile_options(/Od /Zi)
	endif()
else()
	add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
	add_compile_options(-fno-exceptions)
	
	if(OPTIMIZE)
		add_compile_options(-O3 -march=native -flto=auto) # Enable native vectorization and Link Time Optimization
		add_link_options(-flto=auto)
	else()
		add_compile_options(-O0 -g)
	endif()
endif()

# ==============================================================================
# Optimized Linker Selection (Unified Execution)
# ==============================================================================
if(NOT MSVC AND NOT APPLE)
	execute_process(COMMAND mold --version OUTPUT_VARIABLE HAS_MOLD ERROR_QUIET)
	execute_process(COMMAND lld --version OUTPUT_VARIABLE HAS_LLD ERROR_QUIET)
	
	if(HAS_MOLD)
		add_link_options("-fuse-ld=mold")
	elseif(HAS_LLD)
		add_link_options("-fuse-ld=lld")
	else()
		add_link_options("-fuse-ld=gold")
	endif()
elseif(MSVC)
	if(OPTIMIZE)
		add_link_options(/INCREMENTAL:NO)
	else()
		add_link_options(/INCREMENTAL) # Crucial for fast MSVC iteration loops
	endif()
endif()

# ==============================================================================
# Dependencies & Sources
# ==============================================================================
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
set(GLAD_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c")

# ==============================================================================
# Target Configuration
# ==============================================================================
add_executable(${PROJECT_NAME} ${ENGINE_SOURCES} ${GLAD_SOURCE})

target_include_directories(${PROJECT_NAME} PRIVATE
	${OPENGL_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/glad/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/libraries
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	glfw
	OpenGL::GL
)

if(APPLE)
	target_link_libraries(${PROJECT_NAME} PRIVATE 
		"-framework Cocoa"
		"-framework IOKit"
		"-framework CoreVideo"
	)
endif()

# ==============================================================================
# Unity Build & Multithreading Optimization
# ==============================================================================
if(ENABLE_UNITY_BUILD)
	set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)

	# Calculate optimal batch size based on hardware threads to prevent core starvation
	include(ProcessorCount)
	ProcessorCount(NUM_CORES)
	
	if(NOT NUM_CORES EQUAL 0)
		list(LENGTH ENGINE_SOURCES FILE_COUNT)
		# Goal: Generate roughly 2 to 3 batches per CPU core for optimal pipeline saturation
		math(EXPR TARGET_BATCH_SIZE "${FILE_COUNT} / (${NUM_CORES} * 2)")
		
		# Clamping limits: Never batch fewer than 4 files (loses Unity value), never more than 20 (kills incremental)
		if(TARGET_BATCH_SIZE LESS 4)
			set(TARGET_BATCH_SIZE 4)
		elseif(TARGET_BATCH_SIZE GREATER 20)
			set(TARGET_BATCH_SIZE 20)
		endif()
		
		set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD_BATCH_SIZE ${TARGET_BATCH_SIZE})
	else()
		set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD_BATCH_SIZE 8) # Safe default fallback
	endif()

	# Placeholder: Exclude volatile macro/include-sensitive files from batching
	set_source_files_properties(
		${CMAKE_CURRENT_SOURCE_DIR}/src/Mesh.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/Shader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/Texture.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/Window.cpp
		PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON
	)
endif()
