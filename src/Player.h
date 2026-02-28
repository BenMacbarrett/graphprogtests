#pragma once

#include "SDL3/SDL.h"
#include "DirectionMonitor.h"
#include "Texture.hpp"

#define PLAYER_SPRITE_DIRECTION_COUNT   ( ( uint8_t )4 )
#define PLAYER_X_POS_TICK_UPDATE        ( ( uint64_t )55 )
#define PLAYER_Y_POS_TICK_UPDATE        ( ( uint64_t )55 )
#define PLAYER_SPRITE_MOTION_COUNT      ( ( uint32_t )4 )
#define PLAYER_SPRITE_MOTION_PERIOD     ( ( uint64_t )150 )
#define PLAYER_SPRITE_X_PIX             ( 16.0f )
#define PLAYER_SPRITE_Y_PIX             ( 32.0f )
#define PLAYER_SPRITE_POSITION( X )     ( ( PLAYER_SPRITE_Y_PIX * ( X ) ) + 1.0f )

class Player
{
    public:
        Player( DirectionMonitor &directionMonitor ) : 
        _directionMonitor( directionMonitor ),
        _texture()
        {
            _playerSpriteData.h = PLAYER_SPRITE_Y_PIX;
            _playerSpriteData.w = PLAYER_SPRITE_X_PIX;
            _playerSpriteData.x = 0.0f;
            _playerSpriteData.y = 0.0f;

            _postion_x = 0.0f;
            _position_y = 0.0f;

            rout;
            _playerSpriteData =
            {
                .x = 0,
                .y = 33,
                .w = 16,
                .h = 32,
            };
            _playerTextureOutput.w = 16.0f;
            _playerTextureOutput.h = 32.0f;
        }

        ~Player( void ){}

        void loadPlayer( SDL_Renderer* renderer )
        {
            _texture.loadFromFile( "images/pixelart/Abigail.png", renderer );
        }

        void udpate( const uint64_t tick )
        {
            Direction direction_x = _directionMonitor.getHorizontalDirection();
            Direction direction_y = _directionMonitor.getVerticalDirection();
            Direction direction = ( Direction::DIRECTION_NONE != direction_x ) ? direction_x : direction_y;

            _udpate_position_x( tick, direction_x );
            _udpate_position_y( tick, direction_y );
            _udpdateSpritePosition( direction );
            _updateSpriteMotion( tick, ( Direction::DIRECTION_NONE != direction ) );
        }

        void getPlayerPosition( float *x, float *y )
        {
            if( ( nullptr != x ) &&
                ( nullptr != y) )
            {
                *x = _postion_x;
                *y = _position_y;
            }
        }

        void draw( SDL_Renderer* renderer, float scale )
        {
            r
            r.x = ( float )( ( float )_width * pos_x );
            r.y = ( float )( ( float )_height * pos_y );
            SDL_RenderTextureRotated(_renderer, bonom, &rin, &r, 0.0, NULL, SDL_FLIP_NONE );
        }

        // char * loadTexture( void )
        // {
        //     return "images/pixelart/Abigail.png\0";
        // }

        
    private:
        Texture _texture;
        DirectionMonitor &_directionMonitor;
        SDL_FRect _playerSpriteData;
        SDL_FRect _playerTextureOutput;

        float _postion_x = 0.0f;
        float _position_y = 0.0f;
        uint64_t _postion_x_tick = ( uint64_t )0;
        uint64_t _postion_y_tick = ( uint64_t )0;
        uint32_t _motion = ( uint32_t )0;
        uint64_t _motion_tick = ( uint64_t )0;

        const float _directionSpritePosition[ 4 ] =
        {
            [ 0 ] = PLAYER_SPRITE_POSITION( 0.0f ),
            [ 1 ] = PLAYER_SPRITE_POSITION( 1.0f ),
            [ 2 ] = PLAYER_SPRITE_POSITION( 2.0f ),
            [ 3 ] = PLAYER_SPRITE_POSITION( 3.0f ),
        };

        void _udpate_position_x( const uint64_t tick, const Direction direction )
        {
            if( PLAYER_X_POS_TICK_UPDATE <= ( tick - _postion_x_tick ) )
            {
                _postion_x_tick = tick;

                if( Direction::DIRECTION_NONE != direction )
                {
                    if( Direction::DIRECTION_LEFT == direction )
                    {
                        if( _postion_x > 0.0f )
                        {
                            _postion_x = _postion_x - 0.005f;
                            if( _postion_x < 0.0f )
                            {
                                _postion_x = 0.0f;
                            }
                        }
                    }
                    else
                    {
                        if( _postion_x < 1.0f )
                        {
                            _postion_x = _postion_x + 0.01f;
                            if( _postion_x > 1.0f )
                            {
                                _postion_x = 1.0f;
                            }
                        }
                    }
                }
            }
        }

        void _udpate_position_y( const uint64_t tick, const Direction direction )
        {
            if( PLAYER_Y_POS_TICK_UPDATE <= ( tick - _postion_y_tick ) )
            {
                _postion_y_tick = tick;

                if( Direction::DIRECTION_NONE != direction )
                {
                    if( Direction::DIRECTION_UP == direction )
                    {
                        if( _position_y > 0.0f )
                        {
                            _position_y = _position_y - 0.005f;
                            if( _position_y < 0.0f )
                            {
                                _position_y = 0.0f;
                            }
                        }
                    }
                    else
                    {
                        if( _position_y < 1.0f )
                        {
                            _position_y = _position_y + 0.005f;
                            if( _position_y > 1.0f )
                            {
                                _position_y = 1.0f;
                            }
                        }
                    }
                }
            }
        }


        bool _assert_position( const Direction direction )
        {
            return ( ( Direction::DIRECTION_DOWN == direction ) ||
                     ( Direction::DIRECTION_RIGHT == direction ) ||
                     ( Direction::DIRECTION_UP == direction ) ||
                     ( Direction::DIRECTION_LEFT == direction ) );
        }

        void _udpdateSpritePosition( const Direction direction )
        {
            if( true == _assert_position( direction ) )
            {
                _playerSpriteData.y = _directionSpritePosition[ static_cast<int>(direction) ];
            }
        }

        void _updateSpriteMotion( const uint64_t tick, const bool isInMovement )
        {
            if( false == isInMovement )
            {
                _motion_tick = tick - ( PLAYER_SPRITE_MOTION_PERIOD - 1 );
                _motion = 0;
            }
            else if( PLAYER_SPRITE_MOTION_PERIOD <= ( tick - _motion_tick ) )
            {
                _motion_tick = tick;
                _motion++;

                if( PLAYER_SPRITE_MOTION_COUNT <= _motion )
                {
                    _motion = 0;
                }
            }
            else
            {
            }

            _playerSpriteData.x = PLAYER_SPRITE_X_PIX * ( float )_motion;
        }
};