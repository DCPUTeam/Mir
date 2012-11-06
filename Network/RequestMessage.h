///
/// @brief The class declaration for the message that requests objects.
/// @author James Rhodes
///

#ifndef __MIR_NETWORKING_CREATE_MESSAGE_H
#define __MIR_NETWORKING_CREATE_MESSAGE_H

#include <string>
#include "Message.h"

namespace Network
{
    ///
    /// @brief A message that requests an object from a remote computer.
    ///
    class RequestMessage : public Message
    {
    public:
#ifdef MIR_NETWORK_CONTROLLER_INTERNALS
        RequestMessage();
#endif
        RequestMessage(std::string identifier);
        virtual std::string Serialize();
        virtual void Deserialize(std::string data);

        std::string Identifier;
    };
}

#endif
