/*
 * The class declaration for Actors.
 */

#ifndef __DCPU_GAME_SERVER_ACTOR_H
#define __DCPU_GAME_SERVER_ACTOR_H

#include "../Network/SpatialObject.h"

namespace Server
{
    class Actor : public Network::SpatialObject
    {
    public:
        Actor(Network::Controller& controller, std::string identifier)
            : Network::SpatialObject(controller, identifier) {}
        virtual void ReceiveMessage(Network::Message& message);
    };
}
#endif
