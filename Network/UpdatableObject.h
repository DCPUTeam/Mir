///
/// @brief The class declaration for updatable entities.
/// @author James Rhodes
///

#ifndef __MIR_UPDATABLE_OBJECT_H
#define __MIR_UPDATABLE_OBJECT_H

#include "IdentifiableObject.h"

namespace Network
{
    ///
    /// @brief Represents the updatable attributes of an entity.
    ///
    class UpdatableObject
    {
    public:
        virtual void Update() = 0;
    };
}

#endif
