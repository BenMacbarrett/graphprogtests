cmake_minimum_required( VERSION 3.16 )

project( Graphics )

add_library( ${PROJECT_NAME} )

add_compile_definitions(SDL_MAIN_HANDLED)

# Add your executable
target_sources( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Graphics.cpp )
target_include_directories( ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${PROJECT_NAME} PUBLIC SDL3_image::SDL3_image SDL3::SDL3 )