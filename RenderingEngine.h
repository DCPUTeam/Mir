/*
 * The class declaration for the RenderingEngine.
 */

#ifndef __DCPU_GAME_RENDERING_ENGINE_H
#define __DCPU_GAME_RENDERING_ENGINE_H

#include "Universe.h"

class RenderingEngine
{
public:
    static void Render(Universe& universe);
};

#endif