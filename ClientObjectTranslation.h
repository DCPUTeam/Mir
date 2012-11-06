///
/// @brief The class declaration for the client object translation.
/// @author James Rhodes
///

#ifndef __MIR_CLIENT_OBJECT_TRANSLATION_H
#define __MIR_CLIENT_OBJECT_TRANSLATION_H

#include <string>
#include "Network/ObjectTranslation.h"

///
/// @brief Provides functionality from translating network IDs into objects in memory and visa-versa.
///
class ClientObjectTranslation : public Network::ObjectTranslation
{
public:
    virtual Network::IdentifiableObject* CreateByType(Network::Controller& controller, std::string type, std::string identifier);
    virtual std::string GetType(Network::IdentifiableObject* object);
};

#endif