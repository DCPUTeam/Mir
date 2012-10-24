/*
 * The class declaration for the Player.
 */

#ifndef __DCPU_GAME_PLAYER_H
#define __DCPU_GAME_PLAYER_H

#include "CachedSpatial.h"
#include "CachedActor.h"

class CachedPlayer : public CachedSpatial
{
private:
    CachedActor* m_Actor;
public:
    CachedPlayer() :
      m_Actor(NULL) {}
};

#endif
