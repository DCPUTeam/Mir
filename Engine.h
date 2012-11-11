///
/// @brief The class declaration for the Engine.
/// @author James Rhodes
///

#ifndef __MIR_ENGINE_H
#define __MIR_ENGINE_H

class Engine;

#include <GL/glfw3.h>
#include <boost/noncopyable.hpp>
#include "Network/Controller.h"
#include "ClientObjectTranslation.h"
#include "BaseState.h"

///
/// @brief The main game engine.
/// 
class Engine : private boost::noncopyable
{
private:
    Network::Controller* m_Controller;
    ClientObjectTranslation* m_Translation;
    BaseState* m_CurrentState;

    GLFWwindow m_GLFWWindow;
    bool m_Running;
    bool m_WindowOpen;
public:
    Engine();
    ~Engine();
    bool IsRunning();
    void Run();
    void Cleanup();
    void Connect(std::string address, int port);
    void Disconnect();
    void Switch(BaseState* state);
    GLFWwindow& GetWindow();
    bool HasNetworkController();
    Network::Controller& GetNetworkController();
};

#endif
