cmake_minimum_required (VERSION 3.24)


project( MyGame LANGUAGES C CXX )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(external/tinytmx)

add_subdirectory(src)
# 
# Add your executable
add_executable( MyGame main.cpp )

# Link the libraries
target_link_libraries( MyGame PRIVATE Graphics )
