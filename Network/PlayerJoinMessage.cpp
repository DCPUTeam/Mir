/*
 * The class definitions for a message that a player joined.
 */

#include <exception>
#include "PlayerJoinMessage.h"

namespace Network
{
    PlayerJoinMessage::PlayerJoinMessage()
    {
    }

    std::string PlayerJoinMessage::Serialize()
    {
        return "";
    }

    void PlayerJoinMessage::Deserialize(std::string data)
    {
    }
}