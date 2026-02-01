#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include <iostream>
#include "Graphics.h"




int main( int argc, char** argv )
{
    const static GraphicsConfiguration graphicConfiguration( "This is my Test", "0.0.1", "com.das.test", "My Test", 800, 600 );
    int ret = -1;

    Graphics* appGraphics = new Graphics( &graphicConfiguration );
    if( true == appGraphics->isGraphicsInitialized() )
    {

        appGraphics->run();
        delete appGraphics;

        ret = 0;
    }
    else
    {
        std::cout << "Error initializing SDL... " << std::endl;
    }

    return ret;
}

