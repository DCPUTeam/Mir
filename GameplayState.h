///
/// @brief Defines the class that actively monitors gameplay.
/// @author James Rhodes
///

#ifndef __MIR_GAMEPLAY_STATE_H
#define __MIR_GAMEPLAY_STATE_H

#include "Engine.h"
#include "BaseState.h"
#include "CachedUniverse.h"
#include "CachedPlayer.h"

///
/// @brief Monitors and updates gameplay.
///
class GameplayState : public BaseState
{
private:
    CachedUniverse* GetUniverse();
    CachedPlayer* GetPlayer();
    bool ValidateConnection();
public:
    GameplayState(Engine* engine)
        : BaseState(engine) { }
    virtual void Activate();
    virtual void Deactivate();
    virtual void Render();
    virtual void Update();
};

#endif
