/*
 * The class definition for a Ship.
 */

#include "Ship.h"

namespace Server
{
    void Ship::ReceiveMessage(Network::Message& message)
    {
        // Handle messages.
        std::cout << "NOTE: Ship just received a message!" << std::endl;
    }
}