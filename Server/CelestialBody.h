///
/// @brief The class declaration for celestial bodies.
/// @author James Rhodes
///

#ifndef __MIR_CELESTIAL_BODY_H
#define __MIR_CELESTIAL_BODY_H

#include "../Network/SpatialObject.h"
#include "../Network/UpdatableObject.h"

namespace Server
{
    ///
    /// @brief Represents a celestial body on the client side.
    ///
    class CelestialBody : public Network::SpatialObject, public Network::UpdatableObject
    {
    public:
        CelestialBody(Network::Controller& controller, std::string identifier)
            : Network::SpatialObject(controller, identifier) {}
        virtual void ReceiveMessage(Network::Message& message) = 0;
        virtual void Update() = 0;
    };
}

#endif