///
/// @brief The class declaration for celestial bodies.
/// @author James Rhodes
///

#ifndef __MIR_CACHED_CELESTIAL_BODY_H
#define __MIR_CACHED_CELESTIAL_BODY_H

#include "Network/SpatialObject.h"
#include "Network/UpdatableObject.h"
#include "RenderableObject.h"

///
/// @brief Represents a cached celestial body on the client side.
///
class CachedCelestialBody : public Network::SpatialObject, public Network::UpdatableObject, public RenderableObject
{
public:
    CachedCelestialBody(Network::Controller& controller, std::string identifier)
        : Network::SpatialObject(controller, identifier) {}
    virtual void ReceiveMessage(Network::Message& message) = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
};

#endif
