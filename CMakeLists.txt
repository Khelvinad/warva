cmake_minimum_required(VERSION 3.21)
project(warva VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(
        $<$<COMPILE_LANGUAGE:CXX>:/W4>
        $<$<COMPILE_LANGUAGE:CXX>:/permissive->
    )
else()
    add_compile_options(
        $<$<COMPILE_LANGUAGE:CXX>:-Wall>
        $<$<COMPILE_LANGUAGE:CXX>:-Wextra>
        $<$<COMPILE_LANGUAGE:CXX>:-Wpedantic>
    )
endif()

include(FetchContent)

# --- GLFW -------------------------------------------------------------------
# Prefer a system install. Otherwise fetch a pinned source release so every
# developer builds the same version.
find_package(glfw3 3.3 CONFIG QUIET)
if(NOT glfw3_FOUND)
    message(STATUS "GLFW not found; fetching pinned GLFW 3.4")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    # GLFW 3.4 declares CMake 3.4, which CMake 4.x refuses without this.
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
    FetchContent_Declare(
        glfw
        URL https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz
        URL_HASH SHA256=c038d34200234d071fae9345bc455e4a8f2f544ab60150765d7704e08f3dac01
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(glfw)
endif()

if(TARGET glfw)
    set(WARVA_GLFW_TARGET glfw)
elseif(TARGET glfw3)
    set(WARVA_GLFW_TARGET glfw3)
else()
    message(FATAL_ERROR "GLFW target not found after find_package/FetchContent")
endif()

# --- GLM --------------------------------------------------------------------
find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
    message(STATUS "GLM not found; fetching pinned GLM 1.0.1")
    FetchContent_Declare(
        glm
        URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.tar.gz
        URL_HASH SHA256=9f3174561fd26904b23f0db5e560971cbf9b3cbda0b280f04d5c379d03bf234c
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(glm)
endif()

if(TARGET glm::glm)
    set(WARVA_GLM_TARGET glm::glm)
elseif(TARGET glm)
    set(WARVA_GLM_TARGET glm)
else()
    message(FATAL_ERROR "GLM target not found after find_package/FetchContent")
endif()

# --- GLAD (generated for OpenGL 3.3 Core, vendored) --------------------------
add_library(glad STATIC third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)
if(UNIX)
    target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})
endif()

# --- Game -------------------------------------------------------------------
add_executable(warva
    src/main.cpp
    src/App.cpp
    src/Input.cpp
    src/Shader.cpp
    src/Texture.cpp
    src/World.cpp
    src/Renderer.cpp
)
target_include_directories(warva PRIVATE src third_party)
target_compile_definitions(warva PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(warva PRIVATE glad ${WARVA_GLFW_TARGET} ${WARVA_GLM_TARGET})

if(WIN32)
    target_link_libraries(warva PRIVATE opengl32)
elseif(UNIX)
    target_link_libraries(warva PRIVATE ${CMAKE_DL_LIBS})
endif()

# --- Tests ------------------------------------------------------------------
include(CTest)
if(BUILD_TESTING)
    add_executable(warva_tests tests/test_iso.cpp)
    target_include_directories(warva_tests PRIVATE src)
    add_test(NAME iso_math COMMAND warva_tests)
endif()
