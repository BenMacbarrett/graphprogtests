#include "Graphics.h"


int main( int argc, char** argv )
{
    const static GraphicsConfiguration graphicConfiguration( "This is my Test", "0.0.1", "com.das.test", "My Test", 1536, 1024 );
    int ret = -1;

    Graphics* appGraphics = new Graphics( &graphicConfiguration );
    if( true == appGraphics->isGraphicsInitialized() )
    {

        appGraphics->run();

        ret = 0;
    }
    else
    {
        std::cout << "Error initializing SDL... " << std::endl;
    }

    if( appGraphics )
    {
        delete appGraphics;
    }
    
    return ret;
}

