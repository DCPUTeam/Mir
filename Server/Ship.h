/*
 * The class declaration for a Ship.
 */

#ifndef __DCPU_GAME_SERVER_SHIP
#define __DCPU_GAME_SERVER_SHIP

#include "Spatial.h"
#include "../Network/IdentifiableObject.h"

namespace Server
{
    class Ship : public Spatial, public Network::IdentifiableObject
    {
    public:
        Ship(Network::Controller& controller, std::string identifier)
            : Network::IdentifiableObject(controller, identifier) {}
        virtual void ReceiveMessage(Network::Message& message);
    };
}

#endif
