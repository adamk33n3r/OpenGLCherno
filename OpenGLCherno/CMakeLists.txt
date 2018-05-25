if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
endif()

cmake_minimum_required(VERSION 3.6)
project(openglCherno)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)


#LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/glfw/src)
find_library(GLFW_LIBRARY NAMES glfw PATHS ${PROJECT_SOURCE_DIR}/glfw/src)
find_library(GLEW_LIBRARY NAMES GLEW PATHS ${PROJECT_SOURCE_DIR}/glew/lib)

# setting two custom build types
# variable CMAKE_CONFIGURATION_TYPES shall be defined prior to other definitions:
set(CMAKE_CONFIGURATION_TYPES "CustomType1;CustomType2" CACHE STRING "" FORCE)

set(CMAKE_CXX_STANDARD 11)                     # setting the language standard

add_executable(openglCherno "")

FILE(GLOB IMGUI_SRC "${CMAKE_CURRENT_LIST_DIR}/src/vendor/imgui/*.cpp")
target_sources(openglCherno
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/Application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Texture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Shader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Renderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/IndexBuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/VertexBuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/VertexArray.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/vendor/stb_image/stb_image.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/vendor/imgui/imgui.cpp
    ${IMGUI_SRC}
)
target_compile_definitions(openglCherno PUBLIC USE_BARRY)
target_include_directories(openglCherno PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

target_include_directories(openglCherno PUBLIC
  ${PROJECT_SOURCE_DIR}/glfw/include
)
target_link_libraries( openglCherno ${GLFW_LIBRARY} ${GLEW_LIBRARY} GL )
