/*
 * The class declaration for a message that creates objects.
 */

#ifndef __DCPU_GAME_NETWORKING_CREATE_MESSAGE_H
#define __DCPU_GAME_NETWORKING_CREATE_MESSAGE_H

#include <string>
#include "Message.h"

namespace Network
{
    class CreateMessage : public Message
    {
    public:
#ifdef MIR_NETWORK_CONTROLLER_INTERNALS
        CreateMessage();
#endif
        CreateMessage(std::string type, std::string identifier);
        virtual std::string Serialize();
        virtual void Deserialize(std::string data);

        std::string Type;
        std::string Identifier;
    };
}

#endif
