cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(raylib_jam_256_unexpected)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)

# Adding Raylib
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 4.2.0)
FetchContent_MakeAvailable(raylib)

# Our Project
include_directories(${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE all_SRCS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${all_SRCS} "src/camera.h" "src/camera.c")

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    file(INSTALL resources DESTINATION .)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--preload-file resources --shell-file ../minshell.html")
else()
 add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_directory
                            ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
