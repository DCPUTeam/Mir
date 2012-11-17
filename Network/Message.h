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
        Source m_Source;
        bool m_Reliable;
        bool m_GlobalHandled;
    public:
        Message(Source source);
        Message(bool reliable, bool global);
        virtual ~Message() { };
        virtual std::string Serialize() = 0;
        virtual void Deserialize(std::string data) = 0;
        virtual int GetHashCode() = 0;
        Source GetSource();
        bool IsReliable();
        bool IsGlobal();
    };
}

#endif