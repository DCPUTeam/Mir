/*
 * The class definition for a CachedActor.
 */

#include "CachedActor.h"

void CachedActor::ReceiveMessage(Network::Message& message)
{
    // Handle messages.
    std::cout << "NOTE: CachedActor just received a message!" << std::endl;
}

void CachedActor::Update()
{
}

void CachedActor::Render()
{
}