/*
 * The class definition for an Actor.
 */

#include "Actor.h"

namespace Server
{
    void Actor::ReceiveMessage(Network::Message& message)
    {
        // Handle messages.
        std::cout << "NOTE: Actor just received a message!" << std::endl;
    }
}