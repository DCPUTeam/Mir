///
/// @brief The class declaration for the message that indicates an object doesn't exist.
/// @author James Rhodes
///

#ifndef __MIR_NETWORKING_NOT_FOUND_MESSAGE_H
#define __MIR_NETWORKING_NOT_FOUND_MESSAGE_H

#include <string>
#include "Message.h"

namespace Network
{
    ///
    /// @brief A message that indicates a remote computer doesn't have the requested object.
    ///
    class NotFoundMessage : public Message
    {
    public:
#ifdef MIR_NETWORK_CONTROLLER_INTERNALS
        NotFoundMessage();
#endif
        NotFoundMessage(std::string identifier);
        virtual std::string Serialize();
        virtual void Deserialize(std::string data);

        std::string Identifier;
    };
}

#endif
