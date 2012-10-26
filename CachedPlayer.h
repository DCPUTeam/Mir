/*
 * The class declaration for the Player.
 */

#ifndef __DCPU_GAME_PLAYER_H
#define __DCPU_GAME_PLAYER_H

#include "CachedSpatial.h"
#include "CachedActor.h"

class CachedPlayer : public CachedSpatial, public Network::IdentifiableObject
{
private:
    CachedActor* m_Actor;
public:
    CachedPlayer(Network::Controller& controller, std::string identifier)
        : Network::IdentifiableObject(controller, identifier), m_Actor(NULL) {}
    virtual void ReceiveMessage(Network::Message& message);
};

#endif
