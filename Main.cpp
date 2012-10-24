/*
 * The main entry point for the physical simulation client.
 */

#include "Engine.h"
#include "Network/Controller.h"

using namespace Network;

int main(int argc, char** argv)
{
    Controller controller = Controller(ControllerMode::Client, "127.0.0.1", 5055);
    Engine engine = Engine(controller);

    while (engine.IsRunning())
    {
        engine.Run();
    }

    engine.Cleanup();
}