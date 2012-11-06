/*
 * The class declaration for a Ship.
 */

#ifndef __DCPU_GAME_SERVER_SHIP
#define __DCPU_GAME_SERVER_SHIP

#include "../Network/SpatialObject.h"

namespace Server
{
    class Ship : public Network::SpatialObject
    {
    public:
        Ship(Network::Controller& controller, std::string identifier)
            : Network::SpatialObject(controller, identifier) {}
        virtual void ReceiveMessage(Network::Message& message);
    };
}

#endif
