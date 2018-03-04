cmake_minimum_required(VERSION 2.6)
project(Apollo)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

set(ARYA_PATH $ENV{ARYA_PATH})
link_directories(${ARYA_PATH}/bin)
include_directories(${ARYA_PATH}/bin)
include_directories(${ARYA_PATH}/include)

add_compile_options(-std=c++11)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(include)
add_executable(main src/main.cpp src/handler.cpp src/renderable.cpp src/generator.cpp src/letter.cpp)
target_link_libraries(main ${GLFW_LIBRARIES} ${GLEW_LIBRARIES}  ${OPENGL_gl_LIBRARY} params)
