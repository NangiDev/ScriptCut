# CMakeList.txt : CMake project for ScriptCut, include source and define
# project specific logic here.
#
add_executable(ScriptCut ScriptCut.cpp ScriptCut.h)
target_link_libraries(ScriptCut PRIVATE ${LIBRARIES})
