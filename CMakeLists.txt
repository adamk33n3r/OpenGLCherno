cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

include_directories(Dependencies)
include_directories(Dependencies/glew-2.1.0)
include_directories(Dependencies/glew-2.1.0/include)
include_directories(Dependencies/glew-2.1.0/include/GL)
include_directories(Dependencies/GLFW)
include_directories(Dependencies/GLFW/include)
include_directories(Dependencies/GLFW/include/GLFW)
include_directories(OpenGLCherno/src)
include_directories(OpenGLCherno/src/vendor)
include_directories(OpenGLCherno/src/vendor/stb_image)
include_directories(OpenGLCherno/src/vendor/imgui)
include_directories(OpenGLCherno/src/vendor/glm)

set(OpenGL_GL_PREFERENCE GLVND)
set( BUILD_SHARED_LIBS OFF )
set( GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE )
set( GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE )
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE )

execute_process(
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/glew/auto
        COMMAND ${CMAKE_MAKE_PROGRAM}
)
#add_custom_command(
#    OUTPUT ${PROJECT_SOURCE_DIR}/glew/src/glew.c ${PROJECT_SOURCE_DIR}/glew/src/glewinfo.c
#    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/glew/auto
#    COMMAND ${CMAKE_MAKE_PROGRAM}
#)

add_subdirectory(${PROJECT_SOURCE_DIR}/glew/build/cmake ${PROJECT_SOURCE_DIR}/bin/glew EXCLUDE_FROM_ALL)
add_subdirectory(${PROJECT_SOURCE_DIR}/glfw ${PROJECT_SOURCE_DIR}/bin/glfw EXCLUDE_FROM_ALL)
add_subdirectory(${PROJECT_SOURCE_DIR}/OpenGLCherno)

