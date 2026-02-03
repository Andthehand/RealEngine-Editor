cmake_minimum_required(VERSION 3.20)

project(ProjectCreator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(ProjectCreator "src/ProjectCreatorAPP.cpp" "src/ProjectCreatorLayer.h" "src/ProjectCreatorLayer.cpp" )

include_directories(ProjectCreator PRIVATE
  ${CMAKE_SOURCE_DIR}/RealEngine/src
  ${CMAKE_SOURCE_DIR}/RealEngine/vendor/ImGui
  src
)

target_link_libraries(ProjectCreator RealEngine)

file(COPY imgui.ini DESTINATION "${CMAKE_BINARY_DIR}/ProjectCreator")
