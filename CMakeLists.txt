# CMakeList.txt : CMake project for ScriptCut, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/runtime)
set(VCPKG_TARGET_TRIPLET "x64-windows")

set(VCPKG_INSTALL_DEPENDENCIES 1)

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg)
    execute_process(COMMAND git submodule update --init --recursive --depth=1
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "'git submodule update --init --recursive --depth=1' failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()

    execute_process(COMMAND bootstrap-vcpkg.bat
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg
		RESULT_VARIABLE VCPKG_RESULT)
    if(NOT VCPKG_RESULT EQUAL "0")
        message(FATAL_ERROR "VCPKG failed bootstrapping, with result: ${VCPKG_RESULT}")
    endif()

	set(VCPKG_INSTALL_DEPENDENCIES 0)
endif()

if (NOT ${VCPKG_INSTALL_DEPENDENCIES} EQUAL 1 AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg)
	execute_process(
		COMMAND vcpkg.exe install imgui[core,glfw-binding,opengl3-binding] opengl --triplet=${VCPKG_TARGET_TRIPLET}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg
		RESULT_VARIABLE VCPKG_RESULT)
    if(NOT VCPKG_RESULT EQUAL "0")
        message(FATAL_ERROR "VCPKG failed with ${VCPKG_RESULT}, please check installation of VCPKG")
    endif()
endif()

project ("ScriptCut")

find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)

set(LIBRARIES
    ${OPENGL_LIBRARIES}
    imgui::imgui)

add_subdirectory(src)
