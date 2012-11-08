/*
 * The class definitions for a message that a player joined.
 */

#define MIR_NETWORK_CONTROLLER_INTERNALS

#include <exception>
#include "PlayerJoinMessage.h"

namespace Network
{
    PlayerJoinMessage::PlayerJoinMessage()
        : Message(true)
    {
    }

    PlayerJoinMessage::PlayerJoinMessage(Source* source)
        : Message(source)
    {
    }

    std::string PlayerJoinMessage::Serialize()
    {
        return "";
    }

    void PlayerJoinMessage::Deserialize(std::string data)
    {
    }

    int PlayerJoinMessage::GetHashCode()
    {
        int i = 0;
        for (size_t j = 0; j < std::string("PlayerJoinMessage").length(); j++)
            i += i * "PlayerJoinMessage"[i];
        return i;
    }
}