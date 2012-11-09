/*
 * The class definition for a CachedShip.
 */

#include "CachedShip.h"

void CachedShip::ReceiveMessage(Network::Message& message)
{
    // Handle messages.
    std::cout << "NOTE: CachedShip just received a message!" << std::endl;
}

void CachedShip::Update()
{
}

void CachedShip::Render()
{
}