///
/// @brief Implements the class that actively monitors gameplay.
/// @author James Rhodes
///

#include <sstream>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <GL/glut.h>
#include "GameplayState.h"
#include "GameplayRenderingEngine.h"
#include "MainMenuState.h"
#include "Network/Messages/PlayerJoinMessage.h"

CachedPlayer* GameplayState::GetPlayer()
{
    Network::RequestState& state = this->m_Engine->GetNetworkController().Request("player");
    if (state.Status == Network::REQUEST_STATUS_AVAILABLE)
        return (CachedPlayer*)state.Reference;
    else
        return NULL;
}

CachedUniverse* GameplayState::GetUniverse()
{
    Network::RequestState& state = this->m_Engine->GetNetworkController().Request("universe");
    if (state.Status == Network::REQUEST_STATUS_AVAILABLE)
        return (CachedUniverse*)state.Reference;
    else
        return NULL;
}

///
/// @brief Validates that the game is still connected to the server.
///
/// @note If this function returns true, you should immediately return
///       from the calling function.
///
/// @return Whether a connection still exists.
///
bool GameplayState::ValidateConnection()
{
    if (!this->m_Engine->HasNetworkController() ||
        !this->m_Engine->GetNetworkController().Connected)
    {
        MainMenuState* menu = new MainMenuState(this->m_Engine);
        menu->ErrorMessage = "Lost connection to server.";
        this->m_Engine->Switch(menu);
        return false;
    }
    
    return true;
}

void GameplayState::Activate()
{
    // Validate that we are connected.
    if (!this->ValidateConnection())
        return;

    // Start an initial reference to get the player and
    // universe.  This won't happen instantly, so there's no
    // point saving the result of these calls.
    this->GetPlayer();
    this->GetUniverse();

    // Start the gameplay rendering engine settings.
    GameplayRenderingEngine::Init();
}

void GameplayState::Deactivate()
{
    // Stop the gameplay rendering engine settings.
    GameplayRenderingEngine::Deinit();
}

void GameplayState::Update()
{
    // Validate that we are still connected.
    if (!this->ValidateConnection())
        return;

    // Update the universe and everything in it.
    if (this->GetUniverse() != NULL && this->GetPlayer() != NULL)
    {
        this->GetUniverse()->Update();
        this->GetPlayer()->Update();
    }

    // Handle player movement.
    if (this->GetPlayer() != NULL)
    {
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_UP))
            this->GetPlayer()->Z += 0.1;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_DOWN))
            this->GetPlayer()->Z -= 0.1;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_LEFT))
            this->GetPlayer()->X += 0.1;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_RIGHT))
            this->GetPlayer()->X -= 0.1;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_PAGE_UP))
            this->GetPlayer()->Y += 0.1;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_PAGE_DOWN))
            this->GetPlayer()->Y -= 0.1;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_Z))
            this->GetPlayer()->CameraRotation += 0.1;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_X))
            this->GetPlayer()->CameraRotation -= 0.1;
        /*if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_Q))
            this->GetPlayer()->FrustrumA += 0.1;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_W))
            this->GetPlayer()->FrustrumA -= 0.1;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_E))
            this->GetPlayer()->FrustrumB += 0.1;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_R))
            this->GetPlayer()->FrustrumB -= 0.1;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_T))
            this->GetPlayer()->FrustrumC += 0.01;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_Y))
            this->GetPlayer()->FrustrumC -= 0.01;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_U))
            this->GetPlayer()->FrustrumD += 0.1;
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_I))
            this->GetPlayer()->FrustrumD -= 0.1;*/
        this->GetPlayer()->Resynchronise();
    }

    // Test player join.
    if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_SPACE))
    {
        Network::PlayerJoinMessage message;
        this->m_Engine->GetNetworkController().SendMessage(message);
    }
}

void GameplayState::Render()
{
    // Validate that we are still connected.
    if (!this->ValidateConnection())
        return;
    
    // Get the window size.
    int width;
    int height;
    glfwGetWindowSize(this->m_Engine->GetWindow(), &width, &height);
    
    // Get status of main objects.
    Network::RequestState& p = this->m_Engine->GetNetworkController().Request("player");
    Network::RequestState& u = this->m_Engine->GetNetworkController().Request("universe");
    std::stringstream player_state, universe_state;
    player_state << "Player state is: " << p.Status;
    universe_state << "Universe state is: " << u.Status;

    // Draw world.
    glColor3f(0.2f, 0, 0);
    if (this->GetPlayer() != NULL && this->GetUniverse() != NULL)
        GameplayRenderingEngine::Render(*this->GetPlayer(), *this->GetUniverse());
    else
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up orthogonal viewing matrix.
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, width, 0, height);

    // Draw overlay.
    glColor3f(1.0f, 1.0f, 1.0f);
    glWindowPos2i(10, height - 20 - 12);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)player_state.str().c_str());
    glWindowPos2i(10, height - 20 - 30);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)universe_state.str().c_str());
    if (this->GetPlayer() != NULL)
    {
        /*std::stringstream fa, fb, fc, fd;
        fa << "Frustrum A:" << this->GetPlayer()->FrustrumA;
        fb << "Frustrum B:" << this->GetPlayer()->FrustrumB;
        fc << "Frustrum C:" << this->GetPlayer()->FrustrumC;
        fd << "Frustrum D:" << this->GetPlayer()->FrustrumD;

        int a = height - 20 - 30 - 24;

        glWindowPos2i(10, a);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)fa.str().c_str());
        a -= 24;

        glWindowPos2i(10, a);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)fb.str().c_str());
        a -= 24;

        glWindowPos2i(10, a);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)fc.str().c_str());
        a -= 24;

        glWindowPos2i(10, a);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)fd.str().c_str());
        a -= 24;*/
    }
}
