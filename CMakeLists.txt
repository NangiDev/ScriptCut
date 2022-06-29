# CMakeList.txt : CMake project for ScriptCut, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/runtime)

set(VCPKG_TARGET_TRIPLET "x64-windows")

if(UNIX)
    set(VCPKG_TARGET_TRIPLET "x64-linux")
endif()

# Count files inside vcpkg submodule
set(VCPKG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/)
file(GLOB RESULT ${VCPKG_DIR}/*)
list(LENGTH RESULT RES_LEN)

# Flag for checking if dependencies already was installed
set(VCPKG_INSTALL_DEPENDENCIES 1)

# Download and bootstrap VCPKG if not already done
if(RES_LEN EQUAL 0)
    execute_process(COMMAND git submodule update --init --recursive --depth=1
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT)

    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "'git submodule update --init --recursive --depth=1' failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()

    set(BOOTSTRAP_SCRIPT bootstrap-vcpkg.bat)

    if(UNIX)
        set(BOOTSTRAP_SCRIPT ./bootstrap-vcpkg.sh)
    endif()

    execute_process(COMMAND ${BOOTSTRAP_SCRIPT}
        WORKING_DIRECTORY ${VCPKG_DIR}
        RESULT_VARIABLE VCPKG_RESULT)

    if(NOT VCPKG_RESULT EQUAL "0")
        message(FATAL_ERROR "VCPKG failed running ${BOOTSTRAP_SCRIPT}, with result: ${VCPKG_RESULT}")
    endif()

    set(VCPKG_INSTALL_DEPENDENCIES 0)
endif()

# Install dependencies if not already done
if(${VCPKG_INSTALL_DEPENDENCIES} EQUAL 0)
    set(VCPKG_EXE vcpkg.exe)

    if(UNIX)
        set(VCPKG_EXE ./vcpkg)
    endif()

    execute_process(
        COMMAND ${VCPKG_EXE} install imgui[core,glfw-binding,opengl3-binding] opengl --triplet=${VCPKG_TARGET_TRIPLET}
        WORKING_DIRECTORY ${VCPKG_DIR}
        RESULT_VARIABLE VCPKG_RESULT)

    if(NOT VCPKG_RESULT EQUAL "0")
        message(FATAL_ERROR "VCPKG failed with ${VCPKG_RESULT}, please check installation of VCPKG")
    endif()
endif()

project("ScriptCut")

find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)

set(LIBRARIES
    ${OPENGL_LIBRARIES}
    imgui::imgui)

add_subdirectory(src)
