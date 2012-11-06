/*
 * The class definition for a CachedPlayer.
 */

#include "CachedPlayer.h"

void CachedPlayer::ReceiveMessage(Network::Message& message)
{
    // Handle messages.
    //std::cout << "NOTE: CachedPlayer just received a message!" << std::endl;

    Network::SpatialObject::ReceiveMessage(message);
}