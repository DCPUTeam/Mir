/*
 * The class declaration for serializable messages.
 */

#ifndef __DCPU_GAME_NETWORKING_MESSAGE_H
#define __DCPU_GAME_NETWORKING_MESSAGE_H

#include <string>

namespace Network
{
    class Message
    {
    public:
        virtual std::string Serialize() = 0;
        virtual void Deserialize(std::string data) = 0;
    };
}

#endif