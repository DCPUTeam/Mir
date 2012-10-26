/*
 * The class definition for the server object translation.
 */

#include <exception>
#include "ServerObjectTranslation.h"
#include "../Network/Controller.h"
#include "Actor.h"
#include "Player.h"
#include "Ship.h"
#include "Universe.h"

namespace Server
{
    Network::IdentifiableObject* ServerObjectTranslation::CreateByType(Network::Controller& controller, std::string type, std::string identifier)
    {
        if (type == "Actor")
            return new Actor(controller, identifier);
        else if (type == "Player")
            return new Player(controller, identifier);
        else if (type == "Ship")
            return new Ship(controller, identifier);
        else if (type == "Universe")
            return new Universe(controller, identifier);
        else
            throw std::exception();
    }

    std::string ServerObjectTranslation::GetType(Network::IdentifiableObject* object)
    {
        if (typeid(*object) == typeid(Actor))
            return "Actor";
        else if (typeid(*object) == typeid(Player))
            return "Player";
        else if (typeid(*object) == typeid(Ship))
            return "Ship";
        else if (typeid(*object) == typeid(Universe))
            return "Universe";
        else
            throw std::exception();
    }
}
