/*
 * The class declaration for a message that creates objects.
 */

#ifndef __DCPU_GAME_NETWORKING_CREATE_MESSAGE_H
#define __DCPU_GAME_NETWORKING_CREATE_MESSAGE_H

#include <string>
#include "../ObjectMessage.h"

namespace Network
{
    class CreateMessage : public ObjectMessage
    {
    public:
#ifdef MIR_NETWORK_CONTROLLER_INTERNALS
        CreateMessage(Source* source);
#endif
        CreateMessage(std::string type, std::string identifier);
        virtual std::string Serialize();
        virtual void Deserialize(std::string data);
        virtual int GetHashCode();

        std::string Type;
    };
}

#endif
