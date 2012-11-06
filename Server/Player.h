/*
 * The class declaration for the Player.
 */

#ifndef __DCPU_GAME_SERVER_PLAYER_H
#define __DCPU_GAME_SERVER_PLAYER_H

#include "Actor.h"
#include "../Network/SpatialObject.h"

namespace Server
{
    class Player : public Network::SpatialObject
    {
    private:
        Actor* m_Actor;
    public:
        Player(Network::Controller& controller, std::string identifier) :
            Network::SpatialObject(controller, identifier), m_Actor(NULL) {}
        virtual void ReceiveMessage(Network::Message& message);
    };
}

#endif
