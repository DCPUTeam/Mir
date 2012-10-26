/*
 * The class declaration for Ships.
 */

#ifndef __DCPU_GAME_SHIP
#define __DCPU_GAME_SHIP

#include "CachedSpatial.h"
#include "Network/IdentifiableObject.h"

class CachedShip : public CachedSpatial, public Network::IdentifiableObject
{
public:
    CachedShip(Network::Controller& controller, std::string identifier)
        : Network::IdentifiableObject(controller, identifier) {}
    virtual void ReceiveMessage(Network::Message& message);
};

#endif
