cmake_minimum_required( VERSION 3.5 )

include(FetchContent)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)

FetchContent_MakeAvailable(stb)
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})

project( window )

add_library( ${PROJECT_NAME} )

# 
# Add your executable
target_sources( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp )
target_include_directories( ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

# Link the libraries
target_link_libraries( ${PROJECT_NAME} GLAD stb_image )