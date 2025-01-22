cmake_minimum_required(VERSION 3.10)
project(2dGameProject)
file(GLOB 2dGameProject_SRC
     "2dGameProject/*.h"
     "2dGameProject/*.cpp"
)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
add_subdirectory(deps/glfw)
add_subdirectory(deps/glad)
add_subdirectory(deps/spdlog)
add_executable(2dGameProject ${2dGameProject_SRC})
target_include_directories(2dGameProject PUBLIC
                          "${PROJECT_SOURCE_DIR}/deps/glfw/include"
                          "${PROJECT_SOURCE_DIR}/deps/glad/include"
                          "${PROJECT_SOURCE_DIR}/deps/glm"
                          "${PROJECT_SOURCE_DIR}/deps/spdlog/include"
                          )

target_link_libraries(2dGameProject PUBLIC glfw glad spdlog::spdlog ${OPENGL_LIBRARIES})