/*
 * The class definitions for a message that creates objects.
 */

#define MIR_NETWORK_CONTROLLER_INTERNALS

#include <exception>
#include "CreateMessage.h"

namespace Network
{
    CreateMessage::CreateMessage(Source* source)
        : ObjectMessage(source)
    {
    }

    CreateMessage::CreateMessage(std::string type, std::string identifier)
        : ObjectMessage(true)
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

    int CreateMessage::GetHashCode()
    {
        int i = 0;
        for (size_t j = 0; j < std::string("CreateMessage").length(); j++)
            i += i * "CreateMessage"[i];
        for (size_t j = 0; j < this->Type.length(); j++)
            i += i * this->Type[i];
        for (size_t j = 0; j < this->Identifier.length(); j++)
            i += i * this->Identifier[i];
        return i;
    }
}
