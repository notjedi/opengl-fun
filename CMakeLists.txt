cmake_minimum_required(VERSION 3.2)
project(learn_opengl)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2 -g")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2 -g -fsanitize=address")

set(SOURCES src/main.cc)

add_executable(gl ${SOURCES})
target_link_libraries(gl PRIVATE glfw GLEW OpenGL::GL)
