///
/// @brief Implements the class that actively handles the main menu.
/// @author James Rhodes
///

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <GL/glut.h>
#include "MainMenuState.h"
#include "GameplayState.h"

// Hacky workarounds for C... :(
static MainMenuState* CurrentMainMenu;

static void CharHandler(GLFWwindow window, int chr)
{
    if (CurrentMainMenu->m_State == MainMenuState::STATE_INPUT)
        CurrentMainMenu->m_KeyboardBuffer += (char)chr;
}

static void KeyHandler(GLFWwindow window, int key, int pressed)
{
    if (CurrentMainMenu->m_State == MainMenuState::STATE_INPUT)
        if (key == GLFW_KEY_BACKSPACE && pressed)
            CurrentMainMenu->m_KeyboardBuffer = CurrentMainMenu->m_KeyboardBuffer.substr(0, std::max<int>(CurrentMainMenu->m_KeyboardBuffer.length() - 1, 0));
}

void MainMenuState::Activate()
{
    this->m_KeyboardBuffer = "";
    this->m_Status = "";
    this->m_State = STATE_INPUT;

    CurrentMainMenu = this;
    glfwSetInputMode(this->m_Engine->GetWindow(), GLFW_CURSOR_MODE, true);
    glfwSetInputMode(this->m_Engine->GetWindow(), GLFW_STICKY_KEYS, false);
    glfwSetInputMode(this->m_Engine->GetWindow(), GLFW_SYSTEM_KEYS, true);
    glfwSetInputMode(this->m_Engine->GetWindow(), GLFW_KEY_REPEAT, true);
    glfwSetCharCallback(CharHandler);
    glfwSetKeyCallback(KeyHandler);
}

void MainMenuState::Deactivate()
{
    CurrentMainMenu = NULL;
    glfwSetCharCallback(NULL);
    glfwSetKeyCallback(NULL);
}

void MainMenuState::Update()
{
    // Handle enter key press.
    if (this->m_State == STATE_INPUT)
    {
        if (glfwGetKey(this->m_Engine->GetWindow(), GLFW_KEY_ENTER) && this->m_KeyboardBuffer.length() > 0)
        {
            // Tell the engine to connect.
            this->m_KeyboardBuffer = "";
            this->m_Status = "Resolving hostname...";
            this->m_State = STATE_RESOLVING;
        }
    }
    else if (this->m_State == STATE_RESOLVING)
    {
        this->m_Engine->Connect(this->m_KeyboardBuffer, 5055);
        this->m_Status = "Connecting to server...";
        this->m_State = STATE_CONNECTING;
    }
    else if (this->m_State == STATE_CONNECTING)
    {
        // Check with the network controller to see if the
        // connection is still in progress or whether there was an
        // error.
        if (this->m_Engine->HasNetworkController())
        {
            if (this->m_Engine->GetNetworkController().Connecting)
            {
                // Wait until the status changes...
            }
            else if (this->m_Engine->GetNetworkController().Connected)
                this->m_State = STATE_SWITCHING;
            else if (this->m_Engine->GetNetworkController().HasError())
            {
                this->ErrorMessage = this->m_Engine->GetNetworkController().NextError();
                this->m_Engine->Disconnect();
                this->m_State = STATE_INPUT;
            }
            else
            {
                this->ErrorMessage = "Unknown state during network connection.";
                this->m_Engine->Disconnect();
                this->m_State = STATE_INPUT;
            }
        }
        else
        {
            this->ErrorMessage = "No network controller available during connection.";
            this->m_State = STATE_INPUT;
        }
    }
    else if (this->m_State == STATE_SWITCHING)
    {
        // Switch to the gameplay state.  We will automatically come back
        // to the main menu if the server state is actually invalid.
        GameplayState* gameplay = new GameplayState(this->m_Engine);
        this->m_Engine->Switch(gameplay);
    }
}

void MainMenuState::Render()
{
    // Get the window size.
    int width;
    int height;
    glfwGetWindowSize(this->m_Engine->GetWindow(), &width, &height);

    // Set up orthogonal viewing matrix.
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, width, 0, height);
    
    // Increment render steps.
    this->m_RenderSteps++;
    if (this->m_RenderSteps > 60)
        this->m_RenderSteps -= 60;

    // Initialize text variables.
    std::string instruction = "Type the address of the server...";
    std::string input = this->m_KeyboardBuffer + ((this->m_Blink && this->m_State != STATE_CONNECTING) ? "_" : " ");
    if (this->m_RenderSteps % 60 == 0)
        this->m_Blink = !this->m_Blink;

    // Rotate the "Connecting to server..." status.
    if (this->m_RenderSteps % 30 == 0)
    {
        if (this->m_Status == "Connecting to server")
            this->m_Status = "Connecting to server.";
        else if (this->m_Status == "Connecting to server.")
            this->m_Status = "Connecting to server..";
        else if (this->m_Status == "Connecting to server..")
            this->m_Status = "Connecting to server...";
        else if (this->m_Status == "Connecting to server...")
            this->m_Status = "Connecting to server....";
        else if (this->m_Status == "Connecting to server....")
            this->m_Status = "Connecting to server.....";
        else if (this->m_Status == "Connecting to server.....")
            this->m_Status = "Connecting to server";
    }

    // Clear buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glWindowPos2i(10, height - 20 - 12);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)instruction.c_str());
    glWindowPos2i(10, height - 20 - 30);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)input.c_str());
    if (this->m_State == STATE_CONNECTING)
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        glWindowPos2i(10, height - 20 - 60);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)this->m_Status.c_str());
    }
    else if (this->m_State == STATE_INPUT)
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glWindowPos2i(10, height - 20 - 60);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)this->ErrorMessage.c_str());
    }
}
