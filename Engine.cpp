/*
 * The class declaration for the Engine.
 */

#include <cassert>
#include <GL/glfw3.h>
#include "Engine.h"
#include "RenderingEngine.h"
#include "Network/PlayerJoinMessage.h"

Engine::Engine(Network::Controller& controller)
    : m_Controller(controller)
{
    this->m_Running = true;
    this->m_WindowOpen = false;
    this->m_Universe = NULL; //(CachedUniverse*)this->m_Controller.Find("universe");
    this->m_Player = NULL; //(CachedPlayer*)this->m_Controller.Find("player");
    //assert(this->m_Universe != NULL);
    //assert(this->m_Player != NULL);

    glfwInit();
}

Engine::~Engine()
{
    glfwTerminate();
}

bool Engine::IsRunning()
{
    return this->m_Running;
}

void Engine::Run()
{
    // If there is no window open, we must first open it!
    if (!this->m_WindowOpen)
    {
        // Open an 800x600 that matches the parameters as closely as possible.
        this->m_GLFWWindow = glfwCreateWindow(800, 600, GLFW_WINDOWED, "Mir", 0);
        this->m_WindowOpen = true;
        glfwMakeContextCurrent(this->m_GLFWWindow);
    }

    // Render the game.
    glfwPollEvents();
    if (this->m_Player != NULL && this->m_Universe != NULL)
        RenderingEngine::Render(*this->m_Player, *this->m_Universe);
    glfwSwapBuffers(this->m_GLFWWindow);

    // Handle player movement.
    if (this->m_Player != NULL)
    {
        if (glfwGetKey(this->m_GLFWWindow, GLFW_KEY_UP))
            this->m_Player->Z += 1;
        if (glfwGetKey(this->m_GLFWWindow, GLFW_KEY_DOWN))
            this->m_Player->Z -= 1;
        if (glfwGetKey(this->m_GLFWWindow, GLFW_KEY_LEFT))
            this->m_Player->X += 1;
        if (glfwGetKey(this->m_GLFWWindow, GLFW_KEY_RIGHT))
            this->m_Player->X -= 1;
    }

    // Test player join.
    if (glfwGetKey(this->m_GLFWWindow, GLFW_KEY_SPACE))
    {
        Network::PlayerJoinMessage message;
        this->m_Controller.SendMessage(message);
    }

    // If the user has pressed the escape key, close the program.
    // In future, this close handler will be much more advanced!
    if (glfwGetKey(this->m_GLFWWindow, GLFW_KEY_ESC) ||
            glfwGetWindowParam(this->m_GLFWWindow, GLFW_CLOSE_REQUESTED))
    {
        // Exit.  The window will be closed by Engine::Cleanup.
        this->m_Running = false;
    }

    // Synchronise the network.
    this->m_Controller.Synchronise();
}

void Engine::Cleanup()
{
    glfwDestroyWindow(this->m_GLFWWindow);
}
