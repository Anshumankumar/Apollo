cmake_minimum_required(VERSION 2.6)
project(Apollo)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(JPEG REQUIRED)
find_package(OpenCV REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

set(ARYA_PATH $ENV{ARYA_PATH})
link_directories(${ARYA_PATH}/bin)
include_directories(${ARYA_PATH}/bin)
include_directories(${ARYA_PATH}/include)

add_compile_options(-std=c++11)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(include)
set (imageloader src/image/image_loader.cpp src/image/jpeg_loader.cpp)
add_library(image_loader SHARED ${imageloader})
target_link_libraries(image_loader ${JPEG_LIBRARIES})

add_executable(main src/main.cpp src/handler.cpp src/renderable.cpp src/generator.cpp src/letter.cpp  src/texture/perlin_noise.cpp)
target_link_libraries(main ${GLFW_LIBRARIES} ${GLEW_LIBRARIES}  ${OpenCV_LIBS} ${OPENGL_gl_LIBRARY} params image_loader)


