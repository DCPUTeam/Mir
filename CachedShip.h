///
/// @brief The class declaration for Ships.
/// @author James Rhodes
///

#ifndef __MIR_SHIP
#define __MIR_SHIP

#include "Network/SpatialObject.h"
#include "Network/UpdatableObject.h"
#include "RenderableObject.h"

///
/// @brief Represents a cached ship on the client side.
///
class CachedShip : public Network::SpatialObject, public Network::UpdatableObject, public RenderableObject
{
public:
    CachedShip(Network::Controller& controller, std::string identifier)
        : Network::SpatialObject(controller, identifier) {}
    virtual void ReceiveMessage(Network::Message& message);
    virtual void Update();
    virtual void Render();
};

#endif
