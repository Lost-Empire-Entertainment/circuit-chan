cmake_minimum_required(VERSION 3.29.2)

project("CircuitGame" VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform Detection
if (WIN32)
    message(STATUS "[CIRCUITGAME] Platform = Windows")
elseif(UNIX)
    find_package(X11 REQUIRED)
    message(STATUS "[CIRCUITGAME] Platform = Linux")
else()
    message(FATAL_ERROR "[CIRCUITGAME] Unsupported platform. Must be Windows or Linux.")
endif()

# Build Type Detection
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(IS_DEBUG TRUE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(IS_RELEASE TRUE)
else()
    message(FATAL_ERROR "Unknown CMAKE_BUILD_TYPE: '${CMAKE_BUILD_TYPE}'! Must be Debug, Release, or RelWithDebInfo.")
endif()

# Runtime lib handling for MSVC
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Optional Resource File
if (WIN32)
    set(RESOURCE_FILE "${CMAKE_SOURCE_DIR}/files/icon.rc")
endif()

# Directory Setup
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

set(EXT_WINDOW_DIR "${CMAKE_SOURCE_DIR}/_external_shared/KalaWindow")
set(EXT_CRASH_DIR "${CMAKE_SOURCE_DIR}/_external_shared/KalaCrashHandler")
set(EXT_GLM_DIR "${CMAKE_SOURCE_DIR}/_external_shared/glm")
set(EXT_STB_IMAGE_DIR "${CMAKE_SOURCE_DIR}/_external_shared/stb_image")

# Dependencies
# find_package(OpenGL REQUIRED)
# find_package(Vulkan REQUIRED)

# Library Paths
if (WIN32)
    if(IS_RELEASE)
        set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/release/KalaWindow.lib")
        set(CRASH_LIBRARY_PATH "${EXT_CRASH_DIR}/release/KalaCrashHandler.lib")
    else()
        set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/debug/KalaWindowD.lib")
        set(CRASH_LIBRARY_PATH "${EXT_CRASH_DIR}/debug/KalaCrashHandlerD.lib")
    endif()
else()
    if(IS_RELEASE)
        set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/release_opengl/libKalaWindow.so")
        #set(CRASH_LIBRARY_PATH "${EXT_CRASH_DIR}/release/libKalaCrashHandler.so")
    else()
        set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/debug/libKalaWindowD.so")
        #set(CRASH_LIBRARY_PATH "${EXT_CRASH_DIR}/debug/libKalaCrashHandlerD.so")
    endif()
endif()

# Source Files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*/*.cpp"
)

# Executable
add_executable(CircuitGame ${SOURCE_FILES})

# Enable exception handling for MSVC
if (MSVC)
    target_compile_options(CircuitGame PRIVATE /EHsc)
endif()

if (WIN32 AND RESOURCE_FILE)
    #set_source_files_properties(${RESOURCE_FILE} PROPERTIES LANGUAGE RC)
    #target_sources(CircuitGame PRIVATE ${RESOURCE_FILE})
endif()

set_target_properties(CircuitGame PROPERTIES OUTPUT_NAME "CircuitGame")
target_compile_features(CircuitGame PRIVATE cxx_std_20)

# Includes
file(GLOB_RECURSE HEADERS
	configure_depends
	"${CMAKE_SOURCE_DIR}/include/*.hpp"
)
target_sources(CircuitGame PRIVATE ${HEADERS})
target_include_directories(CircuitGame PRIVATE
	"${INCLUDE_DIR}"
    "${EXT_WINDOW_DIR}/include"
    "${EXT_CRASH_DIR}/include"
	"${EXT_GLM_DIR}"
	"${EXT_STB_IMAGE_DIR}"
)

# Preprocessor Defines
if (WIN32)
    target_compile_definitions(CircuitGame PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

# Link libraries
target_link_libraries(CircuitGame PRIVATE
	#Vulkan::Vulkan
	opengl32
	${WINDOW_LIBRARY_PATH}
	${CRASH_LIBRARY_PATH})
	#OpenGL::GL
	#vulkan-1)
if (WIN32)
else()
    target_link_libraries(CircuitGame PRIVATE ${X11_LIBRARIES})
endif()

# Windows Subsystem for Release builds
if(IS_RELEASE AND WIN32)
    set_target_properties(CircuitGame PROPERTIES WIN32_EXECUTABLE TRUE)
    if (MSVC)
        set_target_properties(CircuitGame PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
    endif()
endif()

# Installation
set(CMAKE_INSTALL_BINDIR bin)
install(TARGETS CircuitGame DESTINATION ${CMAKE_INSTALL_BINDIR})

# Copy files directory
add_custom_command(TARGET CircuitGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory "$<TARGET_FILE_DIR:CircuitGame>/files"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/files" "$<TARGET_FILE_DIR:CircuitGame>/files"
)

# Copy external DLLs (Windows only)
if (WIN32)
	if(IS_RELEASE)
		set(DLL_KALAWINDOW "${CMAKE_SOURCE_DIR}/_external_shared/KalaWindow/release")
		set(DLL_KALACRASHHANDLER "${CMAKE_SOURCE_DIR}/_external_shared/KalaCrashHandler/release")
	else()
		set(DLL_KALAWINDOW "${CMAKE_SOURCE_DIR}/_external_shared/KalaWindow/debug")
		set(DLL_KALACRASHHANDLER "${CMAKE_SOURCE_DIR}/_external_shared/KalaCrashHandler/debug")
	endif()

    file(GLOB DLL_FILES 
		"${DLL_KALAWINDOW}/*.dll"
		"${DLL_KALACRASHHANDLER}/*.dll"
	)
    set(DLL_TARGET_DIR "$<TARGET_FILE_DIR:CircuitGame>")

    foreach(DLL_FILE ${DLL_FILES})
        get_filename_component(DLL_NAME ${DLL_FILE} NAME)
        add_custom_command(TARGET CircuitGame POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${DLL_FILE}" "${DLL_TARGET_DIR}/${DLL_NAME}"
        )
    endforeach()
endif()

# Copy docs
#set(COPY_TARGETS LICENSE.md LIBRARIES.md)
#foreach(FILE ${COPY_TARGETS})
#  add_custom_command(TARGET CircuitGame POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E remove "$<TARGET_FILE_DIR:CircuitGame>/${FILE}"
#    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/${FILE}" #"$<TARGET_FILE_DIR:CircuitGame>/${FILE}"
#  )
#endforeach()

# Package
include(CPack)
