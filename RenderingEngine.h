/*
 * The class declaration for the RenderingEngine.
 */

#ifndef __DCPU_GAME_RENDERING_ENGINE_H
#define __DCPU_GAME_RENDERING_ENGINE_H

#include "CachedUniverse.h"
#include "CachedPlayer.h"

class RenderingEngine
{
public:
    static void Render(CachedPlayer& player, CachedUniverse& universe);
};

#endif