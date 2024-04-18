cmake_minimum_required(VERSION 3.2)
project(learn_opengl)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2 -g")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2 -g -fsanitize=address")

set(SOURCES 
    src/main.cc
    src/display.cc
    src/shader.cc
)

include_directories(include)
set(HEADERS
    include/display.h
    include/shader.h
)

add_executable(gl ${SOURCES} ${HEADERS})
target_link_libraries(gl PRIVATE OpenGL::GL glfw glm::glm GLEW)
