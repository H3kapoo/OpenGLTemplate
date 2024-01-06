cmake_minimum_required(VERSION 3.22)
project(simpleUI)

set(CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g") # debug mode

#valgrind --tool=massif ./hkNode 
#ms_print massif.out.454065 > masssif.txt

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../debug) 

include_directories ("/usr/include/freetype2") # linux specific

add_executable(${PROJECT_NAME}
    main.cpp
    src/Application.cpp
    src/Unzipper.cpp

    # Temporary solution until a lib is built to link against
    src/vendor/xzDecompressor/xz/decode.cpp
)

add_subdirectory(src/shaderHelpers)
add_subdirectory(src/meshHelpers)
add_subdirectory(src/renderHelpers)
add_subdirectory(src/inputHelpers)
add_subdirectory(src/treeHelpers)
add_subdirectory(src/textureHelpers)
add_subdirectory(src/utils)
add_subdirectory(src/textHelpers)
add_subdirectory(src/vendor/11Zip) # Path to the 11zip

# linux specific
target_link_libraries(${PROJECT_NAME} glfw GLEW GL X11 pthread Xrandr dl freetype elzip lzma)
