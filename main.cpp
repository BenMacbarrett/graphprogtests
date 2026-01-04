#include <iostream>
#include "window.h"

int main( void ) 
{
    int return_code = 0;

    Window* window_instance = new Window( 800, 600, "OpenGL Window" );
    if( window_instance == nullptr ) 
    {
        std::cerr << "Failed to create Window instance" << std::endl;
        return_code = -1;
    }
    else
    {
        window_instance->run();
        delete window_instance;
    }

    return return_code;
}

