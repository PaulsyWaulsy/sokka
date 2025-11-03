cmake_minimum_required(VERSION 3.17)
project(sokka LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default build type to Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


# Set debug and release flags explicitly
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Find SDL2 and SDL extensions
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# (Optional) find_package(SDL2_gfx REQUIRED)

# Include and source directories
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Collect source files
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include SDL2 headers
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR} ${SDL2_INCLUDE_DIRS})

# Link SDL2 and extensions
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_image SDL2_ttf SDL2_gfx GLEW::GLEW OpenGL::GL)

# ImGui integration
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
# ImGui integration
set(FILE_DIALOG_DIR ${CMAKE_SOURCE_DIR}/external/filedialog)

set(FILE_DIALOG_SOURCES
    ${FILE_DIALOG_DIR}/ImGuiFileDialog.cpp
)

target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SOURCES} ${FILE_DIALOG_SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        ${FILE_DIALOG_DIR}
        ${CMAKE_SOURCE_DIR}/external
)

# Enable compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
