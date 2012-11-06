///
/// @brief The class declaration for spatial entities.
/// @author James Rhodes
///

#ifndef __MIR_CACHED_SPATIAL_H
#define __MIR_CACHED_SPATIAL_H

#include "IdentifiableObject.h"

namespace Network
{
    ///
    /// @brief Represents the spatial attributes of an entity.
    ///
    class SpatialObject : public IdentifiableObject
    {
    private:
        double m_PreviousX;
        double m_PreviousY;
        double m_PreviousZ;
    public:
        SpatialObject(Controller& controller, std::string identifier) :
          IdentifiableObject(controller, identifier), X(0), Y(0), Z(0) { }
        double X;
        double Y;
        double Z;
        virtual void Resynchronise();
        virtual void ReceiveMessage(Message& message);
    };
}

#endif
