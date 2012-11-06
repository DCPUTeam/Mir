///
/// @brief The class declaration for Ships.
/// @author James Rhodes
///

#ifndef __MIR_SHIP
#define __MIR_SHIP

#include "Network/SpatialObject.h"

///
/// @brief Represents a cached ship on the client side.
///
class CachedShip : public Network::SpatialObject
{
public:
    CachedShip(Network::Controller& controller, std::string identifier)
        : Network::SpatialObject(controller, identifier) {}
    virtual void ReceiveMessage(Network::Message& message);
};

#endif
