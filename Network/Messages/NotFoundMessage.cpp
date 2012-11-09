///
/// @brief The implementation of the message that indicates an object doesn't exist.
/// @author James Rhodes
///

#define MIR_NETWORK_CONTROLLER_INTERNALS

#include <exception>
#include "NotFoundMessage.h"

namespace Network
{
    ///
    /// @brief Empty constructor used for deserialization.
    /// @internal
    ///
    NotFoundMessage::NotFoundMessage(Source* source)
        : ObjectMessage(source)
    {
    }

    ///
    /// @brief Constructs a response that the object with the given identifier doesn't exist.
    ///
    NotFoundMessage::NotFoundMessage(std::string identifier)
        : ObjectMessage(true)
    {
        this->Identifier = identifier;
    }

    ///
    /// @brief Serializes the not found response message.
    /// @internal
    ///
    std::string NotFoundMessage::Serialize()
    {
        return this->Identifier;
    }

    ///
    /// @brief Deserializes a not found response message.
    /// @internal
    ///
    void NotFoundMessage::Deserialize(std::string data)
    {
        this->Identifier = data;
    }

    int NotFoundMessage::GetHashCode()
    {
        int i = 0;
        for (size_t j = 0; j < std::string("NotFoundMessage").length(); j++)
            i += i * "NotFoundMessage"[i];
        for (size_t j = 0; j < this->Identifier.length(); j++)
            i += i * this->Identifier[i];
        return i;
    }
}
