/*
 * The class declaration for a message that indicates a player joined.
 */

#ifndef __DCPU_GAME_NETWORKING_PLAYER_JOIN_MESSAGE_H
#define __DCPU_GAME_NETWORKING_PLAYER_JOIN_MESSAGE_H

#include <string>
#include "Message.h"

namespace Network
{
    class PlayerJoinMessage : public Message
    {
    public:
        PlayerJoinMessage();
        virtual std::string Serialize();
        virtual void Deserialize(std::string data);
    };
}

#endif