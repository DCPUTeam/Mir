///
/// @brief The class declaration for the Player.
/// @author James Rhodes
///

#ifndef __MIR_PLAYER_H
#define __MIR_PLAYER_H

#include "CachedActor.h"
#include "Network/SpatialObject.h"
#include "Network/UpdatableObject.h"
#include "RenderableObject.h"

///
/// @brief Represents a cached player on the client side.
///
class CachedPlayer : public Network::SpatialObject, public Network::UpdatableObject, public RenderableObject
{
private:
    CachedActor* m_Actor;
    Network::SpatialObject* m_Focus;
public:
    CachedPlayer(Network::Controller& controller, std::string identifier)
        : Network::SpatialObject(controller, identifier), m_Actor(NULL),
          LookAtX(10), LookAtY(10), LookAtZ(10), CameraDistance(3.7416), CameraRotation(45) {}
    virtual void ReceiveMessage(Network::Message& message);
    virtual void Update();
    virtual void Render();

    double LookAtX;
    double LookAtY;
    double LookAtZ;
    double CameraDistance;
    double CameraRotation;
};

#endif
