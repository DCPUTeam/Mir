/*
 * The class declaration for the Engine.
 */

#ifndef __DCPU_GAME_ENGINE_H
#define __DCPU_GAME_ENGINE_H

#include <GL/glfw3.h>
#include <boost/noncopyable.hpp>
#include "CachedUniverse.h"
#include "CachedPlayer.h"
#include "Network/Controller.h"

class Engine : private boost::noncopyable
{
private:
    CachedUniverse* m_Universe;
    CachedPlayer* m_Player;
    Network::Controller& m_Controller;

    GLFWwindow m_GLFWWindow;
    bool m_Running;
    bool m_WindowOpen;
public:
    Engine(Network::Controller& controller);
    ~Engine();
    bool IsRunning();
    void Run();
    void Cleanup();
};

#endif
