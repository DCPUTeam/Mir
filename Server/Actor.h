/*
 * The class declaration for Actors.
 */

#ifndef __DCPU_GAME_SERVER_ACTOR_H
#define __DCPU_GAME_SERVER_ACTOR_H

#include "Spatial.h"
#include "../Network/IdentifiableObject.h"

namespace Server
{
    class Actor : public Spatial, public Network::IdentifiableObject
    {
    public:
        Actor(Network::Controller& controller, std::string identifier)
            : Network::IdentifiableObject(controller, identifier) {}
        virtual void ReceiveMessage(Network::Message& message);
    };
}
#endif
