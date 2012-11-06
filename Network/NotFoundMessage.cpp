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
    NotFoundMessage::NotFoundMessage()
    {
    }

    ///
    /// @brief Constructs a response that the object with the given identifier doesn't exist.
    ///
    NotFoundMessage::NotFoundMessage(std::string identifier)
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
}
