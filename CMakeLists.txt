cmake_minimum_required(VERSION 2.6)
project(Graphika)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
add_executable(main src/main.cpp)
target_link_libraries(main ${GLFW_LIBRARIES}   ${OPENGL_gl_LIBRARY})
