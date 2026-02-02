

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include "SDL3_image/SDL_image.h"
#include <string>
#include <iostream>

#pragma once

struct graphic_metadata
{
    const std::string appName;
    const std::string appVersion;
    const std::string appID;

    // Constructor with initializer list
    graphic_metadata( const std::string& name, const std::string& version, const std::string& id )
        : appName(name), appVersion( version ), appID( id ) {
        // Constructor body is empty, all initialization is done in the list
    }
};

struct GraphicsConfiguration
{
    const std::string appName;
    const std::string appVersion;
    const std::string appID;
    const std::string appTitle;
    const int width;
    const int height;
    // Constructor with initializer list
    GraphicsConfiguration( const std::string& name, const std::string& version, const std::string& id, const std::string& title, const int w, const int h )
        : appName(name), appVersion( version ), appID( id ), appTitle( title ), width( w ), height( h ) {
        // Constructor body is empty, all initialization is done in the list
    }
};

class Graphics
{
    public:
        Graphics( const struct GraphicsConfiguration *config );
        ~Graphics( void );

        bool isGraphicsInitialized( void ){ return this->isInitialized; }

        void run( void );

    private:
        uint64_t lastTick;
        SDL_Window *_window;
        SDL_Renderer *_renderer;
        int _width;
        int _height;
        bool isInitialized;
        SDL_Texture *bonom;
        SDL_Texture *bg;
        const struct GraphicsConfiguration *config;
};

