/*
 * The main entry point for the physical simulation client.
 */

#include <GL/glut.h>
#include "Engine.h"
#include "Network/Controller.h"
#include "ClientObjectTranslation.h"
#include "MainMenuState.h"

using namespace Network;

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    Engine engine;
    engine.Switch(new MainMenuState(&engine));

    while (engine.IsRunning())
    {
        engine.Run();
    }

    engine.Cleanup();
}