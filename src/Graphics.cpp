#include "Graphics.h"
#include "SDL3_image/SDL_image.h"

#define SQUARE_RATIO ( 0.01f )

Graphics::Graphics( const struct GraphicsConfiguration *config )
    : _directionMonitor(),
      _player( _directionMonitor ),
      _mapRenderer()
{
    SDL_AppResult ret = SDL_APP_SUCCESS; 
    int screenWidth = config->width;
    int screenHeight = config->height;

    bool initSuccess = SDL_SetAppMetadata( config->appName.data(), 
                                   config->appVersion.data(), 
                                   config->appID.data() );
    if( true == initSuccess ) 
    {
        bool initSuccess = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK );
        if( false == initSuccess )
        {
            SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
            ret = SDL_APP_FAILURE;
        }
        else
        {
            const SDL_DisplayID primaryDisplay = SDL_GetPrimaryDisplay();
            const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode( primaryDisplay );
            if( NULL != displayMode )
            {
                screenWidth = displayMode->w;
                screenHeight = displayMode->h;
                SDL_Log( "Screen resolution detected: %dx%d", screenWidth, screenHeight );
            }

            initSuccess = SDL_CreateWindowAndRenderer( config->appTitle.data(), screenWidth, screenHeight, SDL_WINDOW_MAXIMIZED, &_window, &_renderer );
            if( ( false == initSuccess ) ||
                ( NULL == _window ) ||
                ( NULL == _renderer ) )
            {
                ret = SDL_APP_FAILURE;
            }
            else
            {
                bonom = IMG_LoadTexture( _renderer, "images/pixelart/Abigail.png" );
                bg = IMG_LoadTexture( _renderer, "images/pixelart/dg.png" );
                if( ( NULL == bonom ) ||
                    ( NULL == bg ) )
                {
                    ret = SDL_APP_FAILURE;
                }
            }
        }
    }
    else
    {
        ret = SDL_APP_FAILURE;
    }

    if( SDL_APP_SUCCESS == ret )
    {
        _width = screenWidth;
        _height = screenHeight;
        this->config = config;
        lastTick = SDL_GetTicks();
        isInitialized = true;

        SDL_SetRenderLogicalPresentation( _renderer, _width, _height, SDL_LOGICAL_PRESENTATION_LETTERBOX );
        SDL_SetRenderDrawColor(_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(_renderer);

        SDL_RenderTexture(_renderer, bonom, NULL, NULL);

        SDL_RenderPresent(_renderer);

    }
    else
    {
        isInitialized = false;
    }
}

void Graphics::run( void )
{
    running = true;

    uint64_t runTick = SDL_GetTicks();

    _mapRenderer.loadMap( _renderer, "images/map/room_test.tmx" );
    
    // Calculate integer pixel scale based on map size
    int mapWidth = _mapRenderer.getMapPixelWidth();
    int mapHeight = _mapRenderer.getMapPixelHeight();
    
    int pixelScale = 2;
    // if (mapWidth > 0 && mapHeight > 0)
    // {
    //     int scaleX = _width / mapWidth;
    //     int scaleY = _height / mapHeight;
    //     pixelScale = (scaleX < scaleY) ? scaleX : scaleY;
    //     if (pixelScale < 1) pixelScale = 1;
        
    //     SDL_Log("Map pixel size: %dx%d, Screen: %dx%d, Using %dx pixel scale", 
    //             mapWidth, mapHeight, _width, _height, pixelScale);
    // }
    
    int renderMapWidth = mapWidth * pixelScale;
    int renderMapHeight = mapHeight * pixelScale;

    do
    {
        uint64_t tick_now = SDL_GetTicks();

        SDL_Event event{ 0 };
        while( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                }
                break;
                case SDL_EVENT_WINDOW_RESIZED:
                {
                    // Keep a fixed logical resolution even if the output size changes.
                }
                break;
                case SDL_EVENT_KEY_DOWN:
                case SDL_EVENT_KEY_UP:
                {
                    _HandleKeyboardCallback( event.key.scancode, ( SDL_EVENT_KEY_DOWN == event.type ) );
                }
                default:
                {

                }
                break;
            }
        }

        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(_renderer);
  
        _player.udpate( tick_now );

        _player.getPlayerPosition( &pos_x, &pos_y );
        r.x = ( float )( ( float )_width * pos_x );
        r.y = ( float )( ( float )_height * pos_y );
        SDL_RenderTextureRotated(_renderer, bonom, &rin, &r, 0.0, NULL, SDL_FLIP_NONE );
        _mapRenderer.draw( _renderer, renderMapWidth, renderMapHeight );
    
        SDL_RenderPresent(_renderer);
                //clear/draw/display
    } while( true == running );
}

Graphics::~Graphics( void )
{
    if( NULL != config )
    {
        config = NULL;
    }

    if( NULL != _window )
    {
        SDL_DestroyWindow( _window );
    }

    if( NULL != _renderer )
    {
        SDL_DestroyRenderer( _renderer );
    }

    if( NULL != bonom )
    {
        SDL_DestroyTexture( bonom );
    }

   if( NULL != bg )
    {
        SDL_DestroyTexture( bg );
    }

    SDL_Quit();
}

void Graphics::_HandleKeyboardCallback( const SDL_Scancode scancode, const bool isPressed )
{
    switch( scancode )
    {
        /* Quit. */
        case SDL_SCANCODE_ESCAPE:
        case SDL_SCANCODE_Q:
        {
            if( true == isPressed )
            {
                running = false;
            }
        }
        break;
        case SDL_SCANCODE_W:
        {
            _directionMonitor.update( Direction::DIRECTION_UP, isPressed );
        }
        break;
        case SDL_SCANCODE_D:
        {
            _directionMonitor.update( Direction::DIRECTION_RIGHT, isPressed );
        }
        break;
        case SDL_SCANCODE_A:
        {
            _directionMonitor.update( Direction::DIRECTION_LEFT, isPressed );
        }
        break;
        case SDL_SCANCODE_S:
        {
            _directionMonitor.update( Direction::DIRECTION_DOWN, isPressed );
        }
        break;
        default:
        {

        }
        break;
    }
}


