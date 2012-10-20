/*
 * The class declaration for the Engine.
 */

#ifndef __DCPU_GAME_ENGINE_H
#define __DCPU_GAME_ENGINE_H

#include <GL/glfw3.h>
#include "Universe.h"
#include "Player.h"
#include "Network/Controller.h"

class Engine
{
private:
    Universe* m_Universe;
    Player* m_Player;
    Network::Controller* m_Controller;

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
