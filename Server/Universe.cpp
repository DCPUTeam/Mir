/*
 * The class definition for a Universe.
 */

#include "Universe.h"

namespace Server
{
    void Universe::ReceiveMessage(Message& message)
    {
        // Handle messages.
        std::cout << "NOTE: Universe just received a message!" << std::endl;
    }
}