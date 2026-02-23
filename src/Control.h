
#pragma once

#include <iostream>
#include <list>

enum class ControlDirection : uint8_t
{
    DIRECTION_DOWN,
    DIRECTION_RIGHT,
    DIRECTION_UP,
    DIRECTION_LEFT,
    DIRECTION_NONE
};

enum class ControlStatus : uint8_t
{
    CONTROL_STATUS_OFF,
    CONTROL_STATUS_ON,
};

class Control
{
    public:
        Control( void )
        {
            this->_direction_y.clear();
            this->_direction_x.clear();
        }
        ~Control( void ){}

        void update( const ControlDirection direction, const ControlStatus status )
        {
            switch( direction )
            {
                case ControlDirection::DIRECTION_DOWN:
                case ControlDirection::DIRECTION_UP:
                {
                    if( ControlStatus::CONTROL_STATUS_ON != status )
                    {
                        this->_direction_y.remove( direction );
                    }
                    else
                    {
                        this->_direction_y.push_front( direction );
                    }

                    this->_direction_y.unique();
                }
                break;
                case ControlDirection::DIRECTION_LEFT:
                case ControlDirection::DIRECTION_RIGHT:
                {
                    if( ControlStatus::CONTROL_STATUS_ON != status )
                    {
                        this->_direction_x.remove( direction );
                    }
                    else
                    {
                        this->_direction_x.push_front( direction );
                    }

                    this->_direction_x.unique();
                }
                break;
                default:
                {

                }
                break;
            }
        }

        ControlDirection getVerticalDirection( void )
        {
            return ( false == this->_direction_y.empty() ) ? this->_direction_y.front() : ControlDirection::DIRECTION_NONE;
        }

        ControlDirection getHorizontalDirection( void )
        {
            return ( false == this->_direction_x.empty() ) ? this->_direction_x.front() : ControlDirection::DIRECTION_NONE;
        }

    private:
        std::list< ControlDirection > _direction_y;
        std::list< ControlDirection > _direction_x;
};

