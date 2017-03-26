cmake_minimum_required(VERSION 2.6)
project(Apollo)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(include)
add_executable(main src/main.cpp src/handler.cpp src/renderable.cpp)
target_link_libraries(main ${GLFW_LIBRARIES} ${GLEW_LIBRARIES}  ${OPENGL_gl_LIBRARY})
