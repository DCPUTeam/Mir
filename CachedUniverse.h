/*
 * The class declaration for a Universe.
 */

#ifndef __DCPU_GAME_UNIVERSE_H
#define __DCPU_GAME_UNIVERSE_H

#include <list>
#include "CachedActor.h"
#include "CachedShip.h"
#include "Network/Controller.h"
#include "Network/IdentifiableObject.h"

class CachedUniverse : public Network::IdentifiableObject
{
private:
    std::list<CachedActor*> m_CachedActors;
    std::list<CachedShip*> m_CachedShips;
    Network::Controller* m_Controller;

public:
    CachedUniverse(Network::Controller& controller, std::string identifier)
        : Network::IdentifiableObject(controller, identifier) {}
    virtual void ReceiveMessage(Network::Message& message);
};

#endif
