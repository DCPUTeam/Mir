/*
 * The class declaration for the Engine.
 */

#include <GL/glfw3.h>
#include "Engine.h"
#include "RenderingEngine.h"

Engine::Engine()
{
    this->m_Running = true;
    this->m_WindowOpen = false;
    this->m_Universe = new Universe();

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
        this->m_GLFWWindow = glfwCreateWindow(800, 600, GLFW_WINDOWED, "DCPU-16 Game Simulation", NULL);
        this->m_WindowOpen = true;
    }

    // Render the game.
    glfwMakeContextCurrent(this->m_GLFWWindow);
    RenderingEngine::Render(*this->m_Universe);
    glfwSwapBuffers(this->m_GLFWWindow);

    // Check to see if the window is still open.
    this->m_WindowOpen = !glfwGetWindowParam(this->m_GLFWWindow, GLFW_CLOSE_REQUESTED);

    // If the user has pressed the escape key, close the program.
    // In future, this close handler will be much more advanced!
    if (glfwGetKey(this->m_GLFWWindow, GLFW_KEY_ESC))
    {
        // Exit.  The window will be closed by Engine::Cleanup.
        this->m_Running = false;
    }
}

void Engine::Cleanup()
{
    glfwDestroyWindow(this->m_GLFWWindow);
}