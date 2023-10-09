cmake_minimum_required(VERSION 3.6)

set(PROJECT Texty)

project(${PROJECT})

#grab all the files# grab all the files
# set a variable to easily access the root of the source path
set(source_root "${CMAKE_CURRENT_SOURCE_DIR}")
# recursively grab the source files relative to the source route and ignoring dirs
file(
  GLOB_RECURSE sources
  LIST_DIRECTORIES false
  RELATIVE ${source_root}
  "${source_root}/src/*.cpp"
  "${source_root}/src/*.h"
  "${source_root}/resources/shaders/*.glsl"
)

# add the files to the project
add_executable(${PROJECT} ${sources})

# grab the folder structure to maintain order
foreach(source IN ITEMS ${sources})
  # remove the filename from the path
  get_filename_component(source_dir "${source}" DIRECTORY)
  # replace slash direction required for source_group to work
  string(REPLACE "/" "\\" filter_path "${source_dir}")
  # group the source to the directory
  source_group("${filter_path}" FILES "${source}")
endforeach()

# just regular glob for shaders since this is for copying to the exe location
#file(GLOB_RECURSE data "shaders/*")

add_definitions(-std=c++11)

add_subdirectory(jamhengine)
target_link_libraries(${PROJECT} jamhengine)
include_directories(jamhengine/engine/)
include_directories(jamhengine/extern/glm/)
include_directories(jamhengine/extern/miniaudio/)
include_directories(jamhengine/extern/imgui/)

# this copies all resource files in the build directory
# we need this, because we want to work with paths relative to the executable
file(COPY resources DESTINATION /)
#copy the core shaders across from jamhengine
file(COPY ${shaders} DESTINATION resources/shaders/)

# make sure that this is the startup project in visual studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT})

#always redo the directory copies from above
add_custom_command(
    TARGET ${PROJECT} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources/ ${PROJECT_BINARY_DIR}/resources
    COMMENT "Copying from resources to project dir"
    VERBATIM
)

add_custom_command(
    TARGET ${PROJECT} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${shaders} ${PROJECT_BINARY_DIR}/resources/shaders
    COMMENT "Copying shaders from jamhengine to project dir"
    VERBATIM
)

#copy this new folder into the actual build folder
add_custom_command(
    TARGET ${PROJECT} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/build/resources/ $<TARGET_FILE_DIR:${PROJECT}>/resources
    COMMENT "Copying resources to target"
    VERBATIM
)
