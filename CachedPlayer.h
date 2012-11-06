///
/// @brief The class declaration for the Player.
/// @author James Rhodes
///

#ifndef __MIR_PLAYER_H
#define __MIR_PLAYER_H

#include "CachedActor.h"
#include "Network/SpatialObject.h"

///
/// @brief Represents a cached player on the client side.
///
class CachedPlayer : public Network::SpatialObject
{
private:
    CachedActor* m_Actor;
public:
    CachedPlayer(Network::Controller& controller, std::string identifier)
        : Network::SpatialObject(controller, identifier), m_Actor(NULL) {}
    virtual void ReceiveMessage(Network::Message& message);
};

#endif
