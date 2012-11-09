/*
 * The main entry point for the physical simulation server.
 */

#include "Universe.h"
#include "Player.h"
#include "Planet.h"
#include "../Network/Controller.h"
#include "ServerObjectTranslation.h"

using namespace Server;
using namespace Network;

int main(int argc, char** argv)
{
    ServerObjectTranslation translation;
    Controller controller(ControllerMode::Server, translation, "", 5055);

    // Show welcome message.
    std::cout << "Mir Game Server (listening on *:5055)" << std::endl;
    std::cout << "===============================================" << std::endl;

    // Create universe and player.
    Universe universe(controller, "universe");
    Player player(controller, "player");

    // TODO: Give some way of shutting down the server.
    while (controller.ServerRunning)
    {
        controller.Synchronise();
    }
}
