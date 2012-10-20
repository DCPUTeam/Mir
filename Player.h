/*
 * The class declaration for the Player.
 */

#ifndef __DCPU_GAME_PLAYER_H
#define __DCPU_GAME_PLAYER_H

#include "Spatial.h"
#include "Actor.h"

class Player : public Spatial
{
private:
    Actor* m_Actor;
public:
    Player() :
      m_Actor(NULL) {}
};

#endif
