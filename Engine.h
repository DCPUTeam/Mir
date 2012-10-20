/*
 * The class declaration for the Engine.
 */

#ifndef __DCPU_GAME_ENGINE_H
#define __DCPU_GAME_ENGINE_H

#include <GL/glfw3.h>
#include "Universe.h"

class Engine
{
private:
    Universe* m_Universe;
    GLFWwindow m_GLFWWindow;
    bool m_Running;
    bool m_WindowOpen;
public:
    Engine();
    ~Engine();
    bool IsRunning();
    void Run();
    void Cleanup();
};

#endif