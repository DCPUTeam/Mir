/*
 * The class declaration for the client object translation.
 */

#ifndef __DCPU_GAME_CLIENT_OBJECT_TRANSLATION_H
#define __DCPU_GAME_CLIENT_OBJECT_TRANSLATION_H

#include <string>
#include "Network/ObjectTranslation.h"

class ClientObjectTranslation : public Network::ObjectTranslation
{
public:
    virtual Network::IdentifiableObject* CreateByType(Network::Controller& controller, std::string type, std::string identifier);
    virtual std::string GetType(Network::IdentifiableObject* object);
};

#endif