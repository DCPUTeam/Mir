///
/// @brief The class declaration for a cached planet.
/// @author James Rhodes
///

#ifndef __MIR_CACHED_PLANET_H
#define __MIR_CACHED_PLANET_H

#include "CachedCelestialBody.h"

///
/// @brief Represents a cached planet on the client side.
///
class CachedPlanet : public CachedCelestialBody
{
public:
    CachedPlanet(Network::Controller& controller, std::string identifier)
        : CachedCelestialBody(controller, identifier) {}
    virtual void ReceiveMessage(Network::Message& message);
    virtual void Update();
    virtual void Render();
};

#endif
