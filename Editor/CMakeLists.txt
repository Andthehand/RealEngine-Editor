cmake_minimum_required(VERSION 3.20)

project(Editor)

option(BUILD_DOCS "Build Doxygen docs" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Editor "src/EditorAPP.cpp" "src/EditorLayer.h" "src/EditorLayer.cpp" "src/Panels/FileExplorerPanel.cpp" "src/Panels/PropertiesPanel.h" "src/Panels/PropertiesPanel.cpp" "src/Events/PanelEvents.h" "src/Panels/Panel.h" "src/Panels/SceneHierarchyPanel.cpp")

include_directories(Editor PRIVATE
  ${CMAKE_SOURCE_DIR}/RealEngine/src
  ${CMAKE_SOURCE_DIR}/RealEngine/vendor/ImGui
  src
)

target_link_libraries(Editor RealEngine)

file(COPY assets DESTINATION "${CMAKE_BINARY_DIR}/Editor")

if(BUILD_DOCS)
	add_subdirectory(docs)
endif(BUILD_DOCS)
