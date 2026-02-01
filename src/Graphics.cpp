#include "Graphics.h"


Graphics::Graphics( const struct GraphicsConfiguration *config )
{
    size_t i;
    SDL_AppResult ret = SDL_APP_SUCCESS; 
    bool initSuccess = SDL_SetAppMetadata( config->appName.data(), 
                                   config->appVersion.data(), 
                                   config->appID.data() );
    if( true == initSuccess ) 
    {
        bool initSuccess = SDL_Init( SDL_INIT_VIDEO );
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
                SDL_SetRenderLogicalPresentation( this->_renderer, config->width, config->height, SDL_LOGICAL_PRESENTATION_LETTERBOX );
                SDL_SetRenderDrawColor(this->_renderer, 75, 0, 0, 129);
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

        std::cout << "C spose marche" << std::endl;
        this->config = config;
        this->lastTick = SDL_GetTicks();
        this->isInitialized = true;
    }
    else
    {
        this->isInitialized = false;
    }
}

void Graphics::run( void )
{
    static bool running = true;
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
                default:
                {

                }
                break;
            }
        }

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

    // if( NULL != this->_renderer )
    // {
    //     SDL_DestroyRenderer( this->_renderer );
    // }

    SDL_Quit();
}
