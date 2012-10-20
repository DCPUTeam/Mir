/*
 * The class declaration for a Universe.
 */

#ifndef __DCPU_GAME_UNIVERSE_H
#define __DCPU_GAME_UNIVERSE_H

#include <list>
#include "Actor.h"
#include "Ship.h"
#include "Network/Controller.h"

class Universe
{
private:
    std::list<Actor*> m_Actors;
    std::list<Ship*> m_Ships; 
    Network::Controller* m_Controller;

public:
    Universe() { }
};

#endif
