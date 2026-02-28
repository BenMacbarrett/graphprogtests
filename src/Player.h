#pragma once

#include "SDL3/SDL.h"
#include "DirectionMonitor.h"

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
        Player( DirectionMonitor *directionMonitor ) : _directionMonitor( directionMonitor )
        {
            this->_playerSpriteData.h = PLAYER_SPRITE_Y_PIX;
            this->_playerSpriteData.w = PLAYER_SPRITE_X_PIX;
            this->_playerSpriteData.x = 0.0f;
            this->_playerSpriteData.y = 0.0f;

            this->_postion_x = 0.0f;
            this->_position_y = 0.0f;
        }

        ~Player( void ){}

        SDL_FRect getUpdatedPlayerData( const uint64_t tick )
        {
            Direction direction_x = this->_directionMonitor->getHorizontalDirection();
            Direction direction_y = this->_directionMonitor->getVerticalDirection();
            Direction direction = ( Direction::DIRECTION_NONE != direction_x ) ? direction_x : direction_y;

            this->_udpate_position_x( tick, direction_x );
            this->_udpate_position_y( tick, direction_y );
            this->_udpdateSpritePosition( direction );
            this->_updateSpriteMotion( tick, ( Direction::DIRECTION_NONE != direction ) );

            return this->_playerSpriteData;
        }

        void getPlayerPosition( float *x, float *y )
        {
            if( ( nullptr != x ) &&
                ( nullptr != y) )
            {
                *x = this->_postion_x;
                *y = this->_position_y;
            }
        }

        // char * loadTexture( void )
        // {
        //     return "images/pixelart/Abigail.png\0";
        // }

        
    private:
        DirectionMonitor *_directionMonitor;
        SDL_FRect _playerSpriteData;

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
            if( PLAYER_X_POS_TICK_UPDATE <= ( tick - this->_postion_x_tick ) )
            {
                this->_postion_x_tick = tick;

                if( Direction::DIRECTION_NONE != direction )
                {
                    if( Direction::DIRECTION_LEFT == direction )
                    {
                        if( this->_postion_x > 0.0f )
                        {
                            this->_postion_x = this->_postion_x - 0.01f;
                            if( this->_postion_x < 0.0f )
                            {
                                this->_postion_x = 0.0f;
                            }
                        }
                    }
                    else
                    {
                        if( this->_postion_x < 1.0f )
                        {
                            this->_postion_x = this->_postion_x + 0.01f;
                            if( this->_postion_x > 1.0f )
                            {
                                this->_postion_x = 1.0f;
                            }
                        }
                    }
                }
            }
        }

        void _udpate_position_y( const uint64_t tick, const Direction direction )
        {
            if( PLAYER_Y_POS_TICK_UPDATE <= ( tick - this->_postion_y_tick ) )
            {
                this->_postion_y_tick = tick;

                if( Direction::DIRECTION_NONE != direction )
                {
                    if( Direction::DIRECTION_UP == direction )
                    {
                        if( this->_position_y > 0.0f )
                        {
                            this->_position_y = this->_position_y - 0.005f;
                            if( this->_position_y < 0.0f )
                            {
                                this->_position_y = 0.0f;
                            }
                        }
                    }
                    else
                    {
                        if( this->_position_y < 1.0f )
                        {
                            this->_position_y = this->_position_y + 0.005f;
                            if( this->_position_y > 1.0f )
                            {
                                this->_position_y = 1.0f;
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
            if( true == this->_assert_position( direction ) )
            {
                this->_playerSpriteData.y = this->_directionSpritePosition[ static_cast<int>(direction) ];
            }
        }

        void _updateSpriteMotion( const uint64_t tick, const bool isInMovement )
        {
            if( false == isInMovement )
            {
                this->_motion_tick = tick - ( PLAYER_SPRITE_MOTION_PERIOD - 1 );
                this->_motion = 0;
            }
            else if( PLAYER_SPRITE_MOTION_PERIOD <= ( tick - this->_motion_tick ) )
            {
                this->_motion_tick = tick;
                this->_motion++;

                if( PLAYER_SPRITE_MOTION_COUNT <= this->_motion )
                {
                    this->_motion = 0;
                }
            }
            else
            {
            }

            this->_playerSpriteData.x = PLAYER_SPRITE_X_PIX * ( float )this->_motion;
        }
};