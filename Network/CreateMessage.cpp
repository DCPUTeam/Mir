/*
 * The class definitions for a message that creates objects.
 */

#include <exception>
#include "CreateMessage.h"

namespace Network
{
    CreateMessage::CreateMessage()
    {
    }

    CreateMessage::CreateMessage(std::string type, std::string identifier)
    {
        this->Type = type;
        this->Identifier = identifier;
    }

    std::string CreateMessage::Serialize()
    {
        return this->Type + ":" + this->Identifier;
    }

    void CreateMessage::Deserialize(std::string data)
    {
        std::string::size_type at = data.find(":", 0);
        if (at == std::string::npos)
            throw std::exception();
        this->Type = data.substr(0, at);
        this->Identifier = data.substr(at + 1, data.length() - at - 1);
    }
}