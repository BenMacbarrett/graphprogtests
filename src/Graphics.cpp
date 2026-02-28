#include "Graphics.h"
#include "SDL3_image/SDL_image.h"


#define SQUARE_RATIO ( 0.01f )

Graphics::Graphics( const struct GraphicsConfiguration *config )
    : _directionMonitor(),
      _player( &_directionMonitor )
{
    size_t i;
    SDL_AppResult ret = SDL_APP_SUCCESS; 
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
            // this->_window = SDL_CreateWindow( "Test window", config->width, config->height, 0 );
            initSuccess = SDL_CreateWindowAndRenderer( config->appTitle.data(), config->width, config->height, SDL_WINDOW_RESIZABLE, &this->_window, &this->_renderer );
            if( ( false == initSuccess ) ||
                ( NULL == this->_window ) ||
                ( NULL == this->_renderer ) )
            {
                ret = SDL_APP_FAILURE;
            }
            else
            {
                this->bonom = IMG_LoadTexture( this->_renderer, "images/pixelart/Abigail.png" );
                this->bg = IMG_LoadTexture( this->_renderer, "images/pixelart/dg.png" );
                if( ( NULL == this->bonom ) ||
                    ( NULL == this->bg ) )
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
        this->_width = config->width;
        this->_height = config->height;
        this->config = config;
        this->lastTick = SDL_GetTicks();
        this->isInitialized = true;

        // SDL_SetRenderLogicalPresentation( this->_renderer, config->width, config->height, SDL_LOGICAL_PRESENTATION_LETTERBOX );
        SDL_SetRenderDrawColor(this->_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(this->_renderer);

        SDL_RenderTexture(this->_renderer, this->bonom, NULL, NULL);

        SDL_RenderPresent(this->_renderer);

    }
    else
    {
        this->isInitialized = false;
    }
}

void Graphics::run( void )
{
    this->running = true;
    static SDL_FRect r;
    static SDL_FRect rin =
    {
        .x = 0,
        .y = 33,
        .w = 16,
        .h = 32,
    };
    r.w = 3.0f * 16.0f;
    r.h = 3.0f * 32.0f;
    static float pos_x = 0.5f;
    static float pos_y = 0.5f;

    uint64_t runTick = SDL_GetTicks();

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
                    this->_width = event.window.data1;
                    this->_height = event.window.data2;
                }
                break;
                case SDL_EVENT_KEY_DOWN:
                case SDL_EVENT_KEY_UP:
                {
                    this->_HandleKeyboardCallback( event.key.scancode, ( SDL_EVENT_KEY_DOWN == event.type ) );
                }
                default:
                {

                }
                break;
            }
        }

        SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(this->_renderer);
        float square_size = this->_width * 0.01f;
        
        rin = this->_player.getUpdatedPlayerData( tick_now );

        this->_player.getPlayerPosition( &pos_x, &pos_y );
        r.x = ( float )( ( float )this->_width * pos_x );
        r.y = ( float )( ( float )this->_height * pos_y );

        SDL_RenderTexture(this->_renderer, this->bg, NULL, NULL );
        SDL_RenderTextureRotated(this->_renderer, this->bonom, &rin, &r, 0.0, NULL, SDL_FLIP_NONE );
        SDL_RenderPresent(this->_renderer);

    } while( true == running );
}

Graphics::~Graphics( void )
{
    if( NULL != this->config )
    {
        this->config = NULL;
    }

    if( NULL != this->_window )
    {
        SDL_DestroyWindow( this->_window );
    }

    if( NULL != this->_renderer )
    {
        SDL_DestroyRenderer( this->_renderer );
    }

    if( NULL != this->bonom )
    {
        SDL_DestroyTexture( this->bonom );
    }

   if( NULL != this->bg )
    {
        SDL_DestroyTexture( this->bg );
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
                this->running = false;
            }
        }
        break;
        case SDL_SCANCODE_W:
        {
            this->_directionMonitor.update( Direction::DIRECTION_UP, isPressed );
        }
        break;
        case SDL_SCANCODE_D:
        {
            this->_directionMonitor.update( Direction::DIRECTION_RIGHT, isPressed );
        }
        break;
        case SDL_SCANCODE_A:
        {
            this->_directionMonitor.update( Direction::DIRECTION_LEFT, isPressed );
        }
        break;
        case SDL_SCANCODE_S:
        {
            this->_directionMonitor.update( Direction::DIRECTION_DOWN, isPressed );
        }
        break;
        default:
        {

        }
        break;
    }
}
