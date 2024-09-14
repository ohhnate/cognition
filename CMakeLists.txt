cmake_minimum_required(VERSION 3.10)
project(CognitionEngine VERSION 0.1.0 LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set SDL2 directory
set(SDL2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL2")
set(SDL2_IMAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image")
set(SDL2_MIXER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_mixer")

# Set SDL2, SDL2_image, and SDL2_mixer include directories explicitly
set(SDL2_INCLUDE_DIR "${SDL2_DIR}/include")
set(SDL2_IMAGE_INCLUDE_DIR "${SDL2_IMAGE_DIR}/include")
set(SDL2_MIXER_INCLUDE_DIR "${SDL2_MIXER_DIR}/include")

# Set Lua paths
set(LUA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lua")
set(LUA_INCLUDE_DIR "${LUA_DIR}/include")
set(LUA_LIBRARIES "${LUA_DIR}/lua54.lib")

# Collect all source files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/modules/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/modules/*/*.c"
)

# Add executable
add_executable(cognition ${SOURCES})

# Include directories
target_include_directories(cognition PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/modules
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
)

# Find SDL2 library
find_library(SDL2_LIBRARY
    NAMES SDL2
    PATHS "${SDL2_DIR}/lib/x64"
    NO_DEFAULT_PATH
)
if(NOT SDL2_LIBRARY)
    message(FATAL_ERROR "SDL2 library not found")
endif()

# Find SDL2main library
find_library(SDL2MAIN_LIBRARY
    NAMES SDL2main
    PATHS "${SDL2_DIR}/lib/x64"
    NO_DEFAULT_PATH
)
if(NOT SDL2MAIN_LIBRARY)
    message(FATAL_ERROR "SDL2main library not found")
endif()

# Find SDL2_image library
find_library(SDL2_IMAGE_LIBRARY
    NAMES SDL2_image
    PATHS "${SDL2_IMAGE_DIR}/lib/x64"
    NO_DEFAULT_PATH
)
if(NOT SDL2_IMAGE_LIBRARY)
    message(FATAL_ERROR "SDL2_image library not found")
endif()

# Find SDL2_mixer library
find_library(SDL2_MIXER_LIBRARY
    NAMES SDL2_mixer
    PATHS "${SDL2_MIXER_DIR}/lib/x64"
    NO_DEFAULT_PATH
)
if(NOT SDL2_MIXER_LIBRARY)
    message(FATAL_ERROR "SDL2_mixer library not found")
endif()

# Link libraries
target_link_libraries(cognition PRIVATE
    ${SDL2_LIBRARY}
    ${SDL2MAIN_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${SDL2_MIXER_LIBRARY}
    ${LUA_LIBRARIES}
)

# Copy SDL2.dll, SDL2_image.dll, SDL2_mixer.dll, and lua54.dll for Windows builds
if(WIN32)
    add_custom_command(TARGET cognition POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_DIR}/lib/x64/SDL2.dll"
        "${SDL2_IMAGE_DIR}/lib/x64/SDL2_image.dll"
        "${SDL2_MIXER_DIR}/lib/x64/SDL2_mixer.dll"
        "${LUA_DIR}/lua54.dll"
        $<TARGET_FILE_DIR:cognition>)
endif()

# Print include directories and libraries for debugging
message(STATUS "SDL2_INCLUDE_DIR: ${SDL2_INCLUDE_DIR}")
message(STATUS "SDL2_IMAGE_INCLUDE_DIR: ${SDL2_IMAGE_INCLUDE_DIR}")
message(STATUS "SDL2_MIXER_INCLUDE_DIR: ${SDL2_MIXER_INCLUDE_DIR}")
message(STATUS "LUA_INCLUDE_DIR: ${LUA_INCLUDE_DIR}")
message(STATUS "SDL2_LIBRARY: ${SDL2_LIBRARY}")
message(STATUS "SDL2MAIN_LIBRARY: ${SDL2MAIN_LIBRARY}")
message(STATUS "SDL2_IMAGE_LIBRARY: ${SDL2_IMAGE_LIBRARY}")
message(STATUS "SDL2_MIXER_LIBRARY: ${SDL2_MIXER_LIBRARY}")
message(STATUS "LUA_LIBRARIES: ${LUA_LIBRARIES}")