/*
 * The class declaration for Actors.
 */

#ifndef __DCPU_GAME_ACTOR_H
#define __DCPU_GAME_ACTOR_H

#include "CachedSpatial.h"
#include "Network/IdentifiableObject.h"

class CachedActor : public CachedSpatial, public Network::IdentifiableObject
{
public:
    CachedActor(Network::Controller& controller, std::string identifier)
        : Network::IdentifiableObject(controller, identifier) {}
    virtual void ReceiveMessage(Network::Message& message);
};

#endif
