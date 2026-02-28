
#pragma once

#include <iostream>
#include <list>

enum class Direction : uint8_t
{
    DIRECTION_DOWN,
    DIRECTION_RIGHT,
    DIRECTION_UP,
    DIRECTION_LEFT,
    DIRECTION_NONE
};

class DirectionMonitor
{
    public:
        DirectionMonitor( void )
        {
            _direction_y.clear();
            _direction_x.clear();
        }
        ~DirectionMonitor( void ){}

        void update( const Direction direction, const bool status )
        {
            switch( direction )
            {
                case Direction::DIRECTION_DOWN:
                case Direction::DIRECTION_UP:
                {
                    if( true == status )
                    {
                        _direction_y.push_front( direction );
                    }
                    else
                    {
                        _direction_y.remove( direction );                        
                    }

                    _direction_y.unique();
                }
                break;
                case Direction::DIRECTION_LEFT:
                case Direction::DIRECTION_RIGHT:
                {
                    if( true == status )
                    {
                        _direction_x.push_front( direction );
                    }
                    else
                    {
                        _direction_x.remove( direction );                        
                    }

                    _direction_x.unique();
                }
                break;
                default:
                {

                }
                break;
            }
        }

        Direction getVerticalDirection( void ) const
        {
            return ( false == _direction_y.empty() ) ? _direction_y.front() : Direction::DIRECTION_NONE;
        }

        Direction getHorizontalDirection( void ) const
        {
            return ( false == _direction_x.empty() ) ? _direction_x.front() : Direction::DIRECTION_NONE;
        }

    private:
        std::list< Direction > _direction_y;
        std::list< Direction > _direction_x;
};

