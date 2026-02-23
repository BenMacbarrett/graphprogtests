#include "Graphics.h"
#include "SDL3_image/SDL_image.h"


#define SQUARE_RATIO ( 0.01f )

Graphics::Graphics( const struct GraphicsConfiguration *config )
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
        this->_control = Control();
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
    static bool running = true;
    static SDL_FRect r;
    static SDL_FRect rin =
    {
        .x = 0,
        .y = 33,
        .w = 16,
        .h = 32,
    };
    static float pos_x = 0.5f;
    static float pos_y = 0.5f;
    static uint8_t runCount = 0;
    // bool flip = false;

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
                case SDL_EVENT_KEY_UP:
                {
                    switch( event.key.scancode )
                    {
                        case SDL_SCANCODE_W:
                        {
                            this->_control.update( ControlDirection::DIRECTION_UP, ControlStatus::CONTROL_STATUS_OFF );
                        }
                        break;
                        case SDL_SCANCODE_D:
                        {
                            this->_control.update( ControlDirection::DIRECTION_RIGHT, ControlStatus::CONTROL_STATUS_OFF );
                        }
                        break;
                        case SDL_SCANCODE_A:
                        {
                            this->_control.update( ControlDirection::DIRECTION_LEFT, ControlStatus::CONTROL_STATUS_OFF );
                        }
                        break;
                        case SDL_SCANCODE_S:
                        {
                            this->_control.update( ControlDirection::DIRECTION_DOWN, ControlStatus::CONTROL_STATUS_OFF );
                        }
                        break;
                    }
                }
                break;
                case SDL_EVENT_KEY_DOWN:
                {
                    switch( event.key.scancode ) 
                    {
                    /* Quit. */
                        case SDL_SCANCODE_ESCAPE:
                        case SDL_SCANCODE_Q:
                        {
                            running = false;
                        }
                        break;
                        case SDL_SCANCODE_W:
                        {
                            this->_control.update( ControlDirection::DIRECTION_UP, ControlStatus::CONTROL_STATUS_ON );
                        }
                        break;
                        case SDL_SCANCODE_D:
                        {
                            this->_control.update( ControlDirection::DIRECTION_RIGHT, ControlStatus::CONTROL_STATUS_ON );
                        }
                        break;
                        case SDL_SCANCODE_A:
                        {
                            this->_control.update( ControlDirection::DIRECTION_LEFT, ControlStatus::CONTROL_STATUS_ON );
                        }
                        break;
                        case SDL_SCANCODE_S:
                        {
                            this->_control.update( ControlDirection::DIRECTION_DOWN, ControlStatus::CONTROL_STATUS_ON );
                        }
                        break;
                        default:
                        {

                        }
                        break;
                    }
                }
                break;
                default:
                {

                }
                break;
            }
        }

        ControlDirection direction_x = this->_control.getHorizontalDirection();
        ControlDirection direction_y = this->_control.getVerticalDirection();
        ControlDirection direction = ( ControlDirection::DIRECTION_NONE != direction_x ) ? direction_x : direction_y;
        if( 55 <= ( tick_now - runTick ) )
        {
            runTick = tick_now;

            if( ControlDirection::DIRECTION_NONE != direction_x )
            {
                if( ControlDirection::DIRECTION_LEFT == direction_x )
                {
                    if( pos_x > 0.0f )
                    {
                        pos_x = pos_x - 0.01f;
                        if( pos_x < 0.0f )
                        {
                            pos_x = 0.0f;
                        }
                    }
                }
                else
                {
                    if( pos_x < 1.0f )
                    {
                        pos_x = pos_x + 0.01f;
                        if( pos_x > 1.0f )
                        {
                            pos_x = 1.0f;
                        }
                    }
                }
            }

            if( ControlDirection::DIRECTION_NONE != direction_y )
            {
                if( ControlDirection::DIRECTION_UP == direction_y )
                {
                    if( pos_y > 0.0f )
                    {
                        pos_y = pos_y - 0.005f;
                        if( pos_y < 0.0f )
                        {
                            pos_y = 0.0f;
                        }
                    }
                }
                else
                {
                    if( pos_y < 1.0f )
                    {
                        pos_y = pos_y + 0.005f;
                        if( pos_y > 1.0f )
                        {
                            pos_y = 1.0f;
                        }
                    }
                }
            }
        }



        uint64_t spriteUpdateTimeout = 150.0f;
        if( spriteUpdateTimeout <= ( tick_now - this->lastTick ) )
        {
            this->lastTick = tick_now;
            runCount++;

            if( 4 <= runCount )
            {
                runCount = 0;
            }
        }

        switch( direction )
        {
            case ControlDirection::DIRECTION_UP:
            {
                rin.y = ( ( 32.0f * ( float )2.0f ) + 1.0f );
            }
            break;
            case ControlDirection::DIRECTION_RIGHT:
            {
                rin.y = ( ( 32.0f * ( float )1.0f ) + 1.0f );
            }
            break;
            case ControlDirection::DIRECTION_LEFT:
            {
                rin.y = ( ( 32.0f * ( float )3.0f ) + 1.0f );
            }
            break;
            case ControlDirection::DIRECTION_DOWN:
            {
                rin.y = ( ( 32.0f * ( float )0.0f ) + 1.0f );
            }
            break;
            default:
            {
                runCount = 0;
            }
            break;
        }
    
        SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(this->_renderer);
        float square_size = this->_width * 0.01f;
        r.x = ( float )( ( float )this->_width * pos_x );
        r.y = ( float )( ( float )this->_height * pos_y );;
        r.w = 4.0f * 8.0f;
        r.h = 4.0f * 16.0f;
        
        rin.x = 16.0f * ( float )runCount;
        SDL_RenderTexture(this->_renderer, this->bg, NULL, NULL );
        SDL_RenderTextureRotated(this->_renderer, this->bonom, &rin, &r, 0.0, NULL, SDL_FLIP_NONE );
        SDL_RenderPresent(this->_renderer);
        // SDL_RenderPresent(this->_renderer);
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
