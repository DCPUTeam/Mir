/*
 * The class definition for a CachedUniverse.
 */

#include "CachedUniverse.h"

void CachedUniverse::ReceiveMessage(Network::Message& message)
{
    // Handle messages.
    std::cout << "NOTE: CachedUniverse just received a message!" << std::endl;
}