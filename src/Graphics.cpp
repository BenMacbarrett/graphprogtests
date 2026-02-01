#include "Graphics.h"


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
                this->_width = config->width;
                this->_height = config->height;
                // SDL_SetRenderLogicalPresentation( this->_renderer, config->width, config->height, SDL_LOGICAL_PRESENTATION_LETTERBOX );
                SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(this->_renderer);
                SDL_RenderPresent(this->_renderer);
            }
        }
    }
    else
    {
        ret = SDL_APP_FAILURE;
    }

    if( SDL_APP_SUCCESS == ret )
    {
        SDL_FRect r;
        std::cout << "C spose marche" << std::endl;
        this->_width = config->width;
        this->_height = config->height;
        this->config = config;
        this->lastTick = SDL_GetTicks();
        this->isInitialized = true;
        SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(this->_renderer);

        r.x = 288;
        r.y = 288;
        r.w = 24;
        r.h = 24;
        
        SDL_SetRenderDrawColor(this->_renderer, 255, 255, 0, SDL_ALPHA_OPAQUE); /*head*/
        bool ret = SDL_RenderFillRect(this->_renderer, &r);
        std::cout << "w " << r.w << " h " << r.h << " x " << r.x << " y " << r.y << " suc " << ret << std::endl;
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
    static float pos_x = 0.5f;
    static float pos_y = 0.5f;
    do
    {
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
                        case SDL_SCANCODE_D:
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
                        break;
                        case SDL_SCANCODE_W:
                        {
                            if( pos_y > 0.0f )
                            {
                                pos_y = pos_y - 0.01f;
                                if( pos_y < 0.0f )
                                {
                                    pos_y = 0.0f;
                                }
                            }
                        }
                        break;
                        case SDL_SCANCODE_A:
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
                        break;
                        case SDL_SCANCODE_S:
                        {
                            if( pos_y < 1.0f )
                            {
                                pos_y = pos_y + 0.01f;
                                if( pos_y > 1.0f )
                                {
                                    pos_y = 1.0f;
                                }
                            }
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

        SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(this->_renderer);
        float square_size = this->_width * 0.01f;
        r.x = ( float )( ( float )this->_width * pos_x ) - ( square_size / 2.0f );
        r.y = ( float )( ( float )this->_height * pos_y ) - ( square_size / 2.0f );
        r.w = square_size;
        r.h = square_size;
        
        SDL_SetRenderDrawColor(this->_renderer, 150, 23, 0, SDL_ALPHA_OPAQUE); /*head*/
        bool ret = SDL_RenderFillRect(this->_renderer, &r);
        std::cout << "w " << r.w << " h " << r.h << " x " << r.x << " y " << r.y << " suc " << ret << std::endl;
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

    SDL_Quit();
}
