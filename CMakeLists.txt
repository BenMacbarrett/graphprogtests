cmake_minimum_required( VERSION 3.5 )
project( OpenGLProject )

add_subdirectory(glad)
add_subdirectory(src)

# 
# Add your executable
add_executable( MyApp main.cpp )

# Link the libraries
target_link_libraries( MyApp GLAD window )
