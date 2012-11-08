///
/// @brief The class declaration for the message that requests objects.
/// @author James Rhodes
///

#ifndef __MIR_NETWORKING_CREATE_MESSAGE_H
#define __MIR_NETWORKING_CREATE_MESSAGE_H

#include <string>
#include "ObjectMessage.h"

namespace Network
{
    ///
    /// @brief A message that requests an object from a remote computer.
    ///
    class RequestMessage : public ObjectMessage
    {
    public:
#ifdef MIR_NETWORK_CONTROLLER_INTERNALS
        RequestMessage(Source* source);
#endif
        RequestMessage(std::string identifier);
        virtual std::string Serialize();
        virtual void Deserialize(std::string data);
        virtual int GetHashCode();
    };
}

#endif
