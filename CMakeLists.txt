cmake_minimum_required(VERSION 3.0.0)
project(air)

# Find packages
if (MSVC)
    list(APPEND CMAKE_PREFIX_PATH "third/glfw/install/lib/cmake/glfw3")
endif()
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Set source files
set(SOURCES
    game/src/main.cc
    third/glad/src/glad.c
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Specify the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)  # Path to the desired output directory

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${OPENGL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIRS}
    engine/include/
    third/glad/include/
    third/glfw/include/
    third/glm/
    third/json/
    third/stb/
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OPENGL_LIBRARIES}
    glfw
)

# Set C++ standard
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)