///
/// @brief Defines an abstract base class for representing game states.
/// @author James Rhodes
///

#ifndef __MIR_BASE_STATE_H
#define __MIR_BASE_STATE_H

#include "Engine.h"

///
/// @brief An abstract class for representing the game state.
///
class BaseState
{
protected:
    Engine* m_Engine;
public:
    BaseState(Engine* engine)
        : m_Engine(engine) { }
    virtual void Activate() = 0;
    virtual void Deactivate() = 0;
    virtual void Render() = 0;
    virtual void Update() = 0;
};

#endif
