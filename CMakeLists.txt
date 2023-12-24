cmake_minimum_required(VERSION 3.22)
project(simpleUI)

set(CMAKE_CXX_STANDARD 23)
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g") # debug mode

#valgrind --tool=massif ./hkNode 
#ms_print massif.out.454065 > masssif.txt

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../debug) 

# include_directories ("/usr/include/freetype2") # linux specific

add_executable(${PROJECT_NAME}
    main.cpp
)

add_subdirectory(src/shaderHelpers)
add_subdirectory(src/meshHelpers)

target_link_libraries(${PROJECT_NAME} glfw GLEW GL X11 pthread Xrandr dl freetype)
