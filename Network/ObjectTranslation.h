/*
 * The class declaration for an object translation API.
 */

#ifndef __DCPU_GAME_NETWORKING_OBJECT_TRANSLATION_H
#define __DCPU_GAME_NETWORKING_OBJECT_TRANSLATION_H

namespace Network
{
    class ObjectTranslation;
}

#include <string>
#include "IdentifiableObject.h"

namespace Network
{
    class ObjectTranslation
    {
    public:
        virtual IdentifiableObject* CreateByType(Controller& controller, std::string type, std::string identifier) = 0;
        virtual std::string GetType(IdentifiableObject* object) = 0;
    };
}

#endif