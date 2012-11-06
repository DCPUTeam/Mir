///
/// @brief Defines the class that actively handles the main menu.
/// @author James Rhodes
///

#ifndef __MIR_MAIN_MENU_STATE_H
#define __MIR_MAIN_MENU_STATE_H

#include "Engine.h"
#include "BaseState.h"

///
/// @brief Monitors and updates the main menu.
///
class MainMenuState : public BaseState
{
private:
    bool m_Blink;
    std::string m_Status;
    int m_RenderSteps;

public:
    MainMenuState(Engine* engine)
        : BaseState(engine), ErrorMessage("") { }
    virtual void Activate();
    virtual void Deactivate();
    virtual void Render();
    virtual void Update();

    std::string ErrorMessage;

    // Private variables, but must be exposed for C handlers.
    std::string m_KeyboardBuffer;
    typedef enum
    {
        STATE_INPUT,
        STATE_RESOLVING,
        STATE_CONNECTING,
        STATE_SWITCHING
    } MainMenuStateEnum;
    MainMenuStateEnum m_State;
};

#endif
