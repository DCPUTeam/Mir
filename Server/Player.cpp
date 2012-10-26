/*
 * The class definition for the Player.
 */

#include "Player.h"

namespace Server
{
    void Player::ReceiveMessage(Network::Message& message)
    {
        // Handle messages.
        std::cout << "NOTE: Player just received a message!" << std::endl;
    }
}