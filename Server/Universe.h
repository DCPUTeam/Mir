/*
 * The class declaration for a Universe.
 */

#ifndef __DCPU_GAME_SERVER_UNIVERSE_H
#define __DCPU_GAME_SERVER_UNIVERSE_H

#include <list>
#include "Actor.h"
#include "Ship.h"
#include "../Network/Controller.h"
#include "../Network/IdentifiableObject.h"

using namespace Network;

namespace Server
{
    class Universe : public IdentifiableObject
    {
    private:
        std::list<Actor*> m_Actors;
        std::list<Ship*> m_Ships;
    public:
        Universe(Controller& controller, std::string identifier)
            : IdentifiableObject(controller, identifier) { }
        virtual void ReceiveMessage(Message& message);
    };
}

#endif
