/*
 * The class declaration for the server object translation.
 */

#ifndef __DCPU_GAME_SERVER_SERVER_OBJECT_TRANSLATION_H
#define __DCPU_GAME_SERVER_SERVER_OBJECT_TRANSLATION_H

#include <string>
#include "../Network/ObjectTranslation.h"

namespace Server
{
    class ServerObjectTranslation : public Network::ObjectTranslation
    {
    public:
        virtual Network::IdentifiableObject* CreateByType(Network::Controller& controller, std::string type, std::string identifier);
        virtual std::string GetType(Network::IdentifiableObject* object);
    };
}

#endif