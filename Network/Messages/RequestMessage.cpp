///
/// @brief The implementation of the message that requests objects.
/// @author James Rhodes
///

#define MIR_NETWORK_CONTROLLER_INTERNALS

#include <exception>
#include "RequestMessage.h"

namespace Network
{
    ///
    /// @brief Empty constructor used for deserialization.
    /// @internal
    ///
    RequestMessage::RequestMessage(Source* source)
        : ObjectMessage(source)
    {
    }

    ///
    /// @brief Constructs a request for the object with the given identifier.
    ///
    RequestMessage::RequestMessage(std::string identifier)
        : ObjectMessage(true)
    {
        this->Identifier = identifier;
    }

    ///
    /// @brief Serializes the request message.
    /// @internal
    ///
    std::string RequestMessage::Serialize()
    {
        return this->Identifier;
    }

    ///
    /// @brief Deserializes a request message.
    /// @internal
    ///
    void RequestMessage::Deserialize(std::string data)
    {
        this->Identifier = data;
    }

    int RequestMessage::GetHashCode()
    {
        int i = 0;
        for (size_t j = 0; j < std::string("RequestMessage").length(); j++)
            i += i * "RequestMessage"[i];
        for (size_t j = 0; j < this->Identifier.length(); j++)
            i += i * this->Identifier[i];
        return i;
    }
}
