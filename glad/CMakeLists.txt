cmake_minimum_required( VERSION 3.5 )
project( GLAD )

# Find system libraries
find_package( OpenGL REQUIRED )
find_package( glfw3 REQUIRED )
# If using GLM
find_package( glm REQUIRED )

add_library( ${PROJECT_NAME} )

# Add your executable
target_sources( ${PROJECT_NAME} PRIVATE src/glad.c ) # Assuming glad.c is in a src folder

# Link the libraries
target_link_libraries( ${PROJECT_NAME} OpenGL::GL glfw )

target_include_directories( ${PROJECT_NAME} PUBLIC include )
