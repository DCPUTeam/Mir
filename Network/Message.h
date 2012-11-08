///
/// @brief The declaration of the base message.
/// @author James Rhodes
///

#ifndef __DCPU_GAME_NETWORKING_MESSAGE_H
#define __DCPU_GAME_NETWORKING_MESSAGE_H

#include <string>
#include "Source.h"

namespace Network
{
    class Message
    {
    private:
        Source* m_Source;
        bool m_Reliable;
    public:
        Message(Source* source);
        Message(bool reliable);
        virtual std::string Serialize() = 0;
        virtual void Deserialize(std::string data) = 0;
        virtual int GetHashCode() = 0;
        Source* GetSource();
    };
}

#endif