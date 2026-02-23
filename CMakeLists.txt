cmake_minimum_required (VERSION 3.24)


project( MyGame LANGUAGES C CXX )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL_image EXCLUDE_FROM_ALL)
# Désactiver la construction des exemples/tests de tmxlite pour gagner du temps
set(TMXLITE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(TMXLITE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# Optionnel : forcer la version statique si tu préfères
set(TMXLITE_STATIC_LIB ON CACHE BOOL "" FORCE)

# Ajouter le répertoire du submodule
add_subdirectory(external/tmxlite/tmxlite)

add_subdirectory(src)
# 
# Add your executable
add_executable( MyGame main.cpp )

# Link the libraries
target_link_libraries( MyGame PRIVATE Graphics )


file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/images" 
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")