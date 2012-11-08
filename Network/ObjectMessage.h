///
/// @brief The declaration of a message for an object.
/// @author James Rhodes
///

#ifndef __DCPU_GAME_NETWORKING_OBJECT_MESSAGE_H
#define __DCPU_GAME_NETWORKING_OBJECT_MESSAGE_H

#include <string>
#include "Message.h"

namespace Network
{
    class ObjectMessage : public Message
    {
    public:
        ObjectMessage(Source* source)
            : Message(source) { }
        ObjectMessage(bool reliable)
            : Message(reliable) { }
        virtual std::string Serialize() = 0;
        virtual void Deserialize(std::string data) = 0;
        virtual int GetHashCode() = 0;

        std::string Identifier;
    };
}

#endif