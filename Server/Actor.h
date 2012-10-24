/*
 * The class declaration for Actors.
 */

#ifndef __DCPU_GAME_SERVER_ACTOR_H
#define __DCPU_GAME_SERVER_ACTOR_H

#include "Spatial.h"
#include "../Network/IdentifiableObject.h"

using namespace Network;

namespace Server
{
    class Actor : public Spatial, public IdentifiableObject
    {
 
    };
}
#endif
