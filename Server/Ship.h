/*
 * The class declaration for a Ship.
 */

#ifndef __DCPU_GAME_SERVER_SHIP
#define __DCPU_GAME_SERVER_SHIP

#include "Spatial.h"
#include "../Network/IdentifiableObject.h"

using namespace Network;

namespace Server
{
    class Ship : public Spatial, public IdentifiableObject
    {
    };
}

#endif
