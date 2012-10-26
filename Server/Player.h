/*
 * The class declaration for the Player.
 */

#ifndef __DCPU_GAME_SERVER_PLAYER_H
#define __DCPU_GAME_SERVER_PLAYER_H

#include "Spatial.h"
#include "Actor.h"
#include "../Network/IdentifiableObject.h"

namespace Server
{
    class Player : public Spatial, public Network::IdentifiableObject
    {
    private:
        Actor* m_Actor;
    public:
        Player(Network::Controller& controller, std::string identifier) :
            IdentifiableObject(controller, identifier), m_Actor(NULL) {}
        virtual void ReceiveMessage(Network::Message& message);
    };
}

#endif
