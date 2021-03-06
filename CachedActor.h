///
/// @brief The class declaration for Actors.
/// @author James Rhodes
///

#ifndef __MIR_ACTOR_H
#define __MIR_ACTOR_H

#include "Network/SpatialObject.h"
#include "Network/UpdatableObject.h"
#include "RenderableObject.h"

///
/// @brief Represents a cached actor on the client side.
///
class CachedActor : public Network::SpatialObject, public Network::UpdatableObject, public RenderableObject
{
public:
    CachedActor(Network::Controller& controller, std::string identifier)
        : Network::SpatialObject(controller, identifier) {}
    virtual void ReceiveMessage(Network::Message& message);
    virtual void Update();
    virtual void Render();
};

#endif
