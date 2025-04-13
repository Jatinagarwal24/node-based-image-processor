cmake_minimum_required(VERSION 3.10)
project(NodeImageProcessor)

set(CMAKE_CXX_STANDARD 17)

# --- OpenCV ---
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# --- GLFW (Unix-style paths for MinGW) ---
set(GLFW_INCLUDE_DIR "/mingw64/include")
set(GLFW_LIBRARY "/mingw64/lib/libglfw3.a" CACHE FILEPATH "GLFW static library")

# --- GLEW (Unix-style paths for MinGW) ---
find_path(GLEW_INCLUDE_DIR GL/glew.h PATHS "/mingw64/include")
find_library(GLEW_LIBRARY NAMES glew32 PATHS "/mingw64/lib")
if (GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
    message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "GLEW not found")
endif()

# --- ImGui ---
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)
set(IMGUI_BACKENDS
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_BACKENDS})

target_compile_definitions(imgui PRIVATE IMGUI_INCLUDE_IMCONFIG_H)
add_definitions(-DIMGUI_DISABLE_PLATFORM_WINDOWS_FUNCTIONS)

# --- Source files ---
file(GLOB_RECURSE APP_SOURCES
    src/*.cpp
    src/*.h
    nodes/*.cpp
    nodes/*.h
)

# Create the main executable target
add_executable(NodeImageProcessor
    ${APP_SOURCES}
    src/tinyfiledialogs.c # Ensure tinyfiledialogs.c is included
)

# Now set compile definitions for the NodeImageProcessor target
target_compile_definitions(NodeImageProcessor PRIVATE IMGUI_INCLUDE_IMCONFIG_H _CRT_SECURE_NO_WARNINGS)

# --- Include directories for targets ---
target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${GLFW_INCLUDE_DIR}
)

target_include_directories(NodeImageProcessor PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${GLEW_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/nodes
)

# --- Link libraries ---
target_link_libraries(NodeImageProcessor
    PRIVATE
    imgui
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
    ${OpenCV_LIBS}
    opengl32
    gdi32
    user32
    shell32
    advapi32
    ole32
    comdlg32
    oleaut32
)