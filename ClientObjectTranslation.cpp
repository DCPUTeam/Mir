/*
 * The class definition for the client object translation.
 */

#include <exception>
#include "ClientObjectTranslation.h"
#include "Network/Controller.h"
#include "CachedActor.h"
#include "CachedPlayer.h"
#include "CachedShip.h"
#include "CachedUniverse.h"

Network::IdentifiableObject* ClientObjectTranslation::CreateByType(Network::Controller& controller, std::string type, std::string identifier)
{
    if (type == "Actor")
        return new CachedActor(controller, identifier);
    else if (type == "Player")
        return new CachedPlayer(controller, identifier);
    else if (type == "Ship")
        return new CachedShip(controller, identifier);
    else if (type == "Universe")
        return new CachedUniverse(controller, identifier);
    else
        throw std::exception();
}

std::string ClientObjectTranslation::GetType(Network::IdentifiableObject* object)
{
    if (typeid(*object) == typeid(CachedActor))
        return "Actor";
    else if (typeid(*object) == typeid(CachedPlayer))
        return "Player";
    else if (typeid(*object) == typeid(CachedShip))
        return "Ship";
    else if (typeid(*object) == typeid(CachedUniverse))
        return "Universe";
    else
        throw std::exception();
}
