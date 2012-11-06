/*
 * The class declaration for a Universe.
 */

#ifndef __DCPU_GAME_SERVER_UNIVERSE_H
#define __DCPU_GAME_SERVER_UNIVERSE_H

#include <list>
#include "Actor.h"
#include "Ship.h"
#include "../Network/Controller.h"
#include "../Network/SpatialObject.h"

using namespace Network;

namespace Server
{
    class Universe : public Network::SpatialObject
    {
    private:
        std::list<Actor*> m_Actors;
        std::list<Ship*> m_Ships;
    public:
        Universe(Controller& controller, std::string identifier)
            : Network::SpatialObject(controller, identifier) { }
        virtual void ReceiveMessage(Message& message);
    };
}

#endif
