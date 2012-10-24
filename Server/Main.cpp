/*
 * The main entry point for the physical simulation server.
 */

#include "Universe.h"
#include "Player.h"
#include "../Network/Controller.h"

using namespace Server;
using namespace Network;

int main(int argc, char** argv)
{
    Controller controller = Controller(ControllerMode::Server, "127.0.0.1", 5055);

    // Create universe and player.
    Universe universe = Universe(controller, "universe");
    Player player = Player(controller, "player");

    // TODO: Give some way of shutting down the server.
    while (controller.ServerRunning)
    {
        controller.Synchronise();
    }
}