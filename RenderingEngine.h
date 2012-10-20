/*
 * The class declaration for the RenderingEngine.
 */

#ifndef __DCPU_GAME_RENDERING_ENGINE_H
#define __DCPU_GAME_RENDERING_ENGINE_H

#include "Universe.h"
#include "Player.h"

class RenderingEngine
{
public:
    static void Render(Player& player, Universe& universe);
};

#endif