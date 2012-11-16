/*
 * The class declaration for the Engine.
 */

#include <cassert>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "Engine.h"
#include "Network/Controller.h"
#include <Messages/Declarations.h>
#include "TextureManager.h"

Engine::Engine()
    : m_Controller(NULL), m_CurrentState(NULL)
{
    this->m_Running = true;
    this->m_WindowOpen = false;
    this->m_Translation = new ClientObjectTranslation();

    // Initailize GLFW.
    glfwInit();

    // Open an 800x600 that matches the parameters as closely as possible.
    this->m_GLFWWindow = glfwCreateWindow(800, 600, GLFW_WINDOWED, "Mir", 0);
    this->m_WindowOpen = true;
    glfwMakeContextCurrent(this->m_GLFWWindow);

    // Initialize GLEW.
    std::cout << "Minimum Required: OpenGL 1.4" << std::endl;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "Result: FAIL!" << std::endl;
        std::cerr << "Unable to load GLEW to inspect OpenGL support." << std::endl;
        std::cerr << "Ensure the GLEW libraries are installed and available for use." << std::endl;
        exit(1);
    }
    else
    {
        // Display OpenGL support status.
        std::cout << "OpenGL 1.1: " << (GLEW_VERSION_1_1 ? "Available" : "MISSING") << std::endl;
        std::cout << "OpenGL 1.2: " << (GLEW_VERSION_1_2 ? "Available" : "MISSING") << std::endl;
        std::cout << "OpenGL 1.3: " << (GLEW_VERSION_1_3 ? "Available" : "MISSING") << std::endl;
        std::cout << "OpenGL 1.4: " << (GLEW_VERSION_1_4 ? "Available" : "MISSING") << std::endl;
        std::cout << "OpenGL 1.5: " << (GLEW_VERSION_1_5 ? "Available" : "MISSING") << std::endl;
        std::cout << "OpenGL 2.0: " << (GLEW_VERSION_2_0 ? "Available" : "MISSING") << std::endl;
        std::cout << "OpenGL 2.1: " << (GLEW_VERSION_2_1 ? "Available" : "MISSING") << std::endl;
        std::cout << "OpenGL 3.0: " << (GLEW_VERSION_3_0 ? "Available" : "MISSING") << std::endl;
        std::cout << "OpenGL 3.1: " << (GLEW_VERSION_3_1 ? "Available" : "MISSING") << std::endl;
        std::cout << "OpenGL 3.2: " << (GLEW_VERSION_3_2 ? "Available" : "MISSING") << std::endl;
        std::cout << "OpenGL 3.3: " << (GLEW_VERSION_3_3 ? "Available" : "MISSING") << std::endl;
        if (GLEW_VERSION_1_4)
            std::cout << "Result: OK!" << std::endl;
        else
        {
            std::cout << "Result: FAIL!" << std::endl;
            std::cerr << "Upgrade your OpenGL drivers." << std::endl;
            exit(1);
        }
    }

    // Load textures!
    TextureManager::LoadTexture("starfield", "Resources/Textures/Starfield.png");
    TextureManager::LoadTexture("planet", "Resources/Textures/Planet.png");
}

Engine::~Engine()
{
    this->m_CurrentState->Deactivate();
    delete this->m_Translation;
    delete this->m_CurrentState;
    glfwTerminate();
}

void Engine::Connect(std::string address, int port)
{
    if (this->m_Controller != NULL)
        this->m_Controller->Close();
    this->m_Controller = new Network::Controller(Network::ControllerMode::Client, *this->m_Translation, address, port);
}

void Engine::Disconnect()
{
    this->m_Controller->Close();
    this->m_Controller = NULL;
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
    if (this->m_CurrentState != NULL)
    {
        this->m_CurrentState->Update();
        this->m_CurrentState->Render();
    }
    glfwSwapBuffers(this->m_GLFWWindow);

    // If the user has pressed the escape key, close the program.
    // In future, this close handler will be much more advanced!
    if (glfwGetKey(this->m_GLFWWindow, GLFW_KEY_ESC) ||
            glfwGetWindowParam(this->m_GLFWWindow, GLFW_CLOSE_REQUESTED))
    {
        // Exit.  The window will be closed by Engine::Cleanup.
        this->m_Running = false;
    }

    // Synchronise the network.
    if (this->m_Controller != NULL)
        this->m_Controller->Synchronise();
}

GLFWwindow& Engine::GetWindow()
{
    // Return the currently open window.
    return this->m_GLFWWindow;
}

bool Engine::HasNetworkController()
{
    // Return whether we are connected to a network.
    return this->m_Controller != NULL;
}

Network::Controller& Engine::GetNetworkController()
{
    // Return the current network controller.
    assert(this->HasNetworkController());
    return *this->m_Controller;
}

void Engine::Cleanup()
{
    glfwDestroyWindow(this->m_GLFWWindow);
}

void Engine::Switch(BaseState* state)
{
    assert(state != NULL);
    if (this->m_CurrentState != NULL)
    {
        this->m_CurrentState->Deactivate();
        delete this->m_CurrentState;
    }
    this->m_CurrentState = state;
    this->m_CurrentState->Activate();
}
