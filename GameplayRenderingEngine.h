/*
 * The class declaration for the RenderingEngine.
 */

#ifndef __MIR_GAMEPLAY_RENDERING_ENGINE_H
#define __MIR_GAMEPLAY_RENDERING_ENGINE_H

#include "CachedUniverse.h"
#include "CachedPlayer.h"

class GameplayRenderingEngine
{
public:
    static void Render(CachedPlayer& player, CachedUniverse& universe);
};

#endif