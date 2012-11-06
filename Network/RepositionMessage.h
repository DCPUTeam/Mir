///
/// @brief The class declaration for repositioning messages.
/// @author James Rhodes
///

#ifndef __MIR_NETWORKING_REPOSITION_MESSAGE_H
#define __MIR_NETWORKING_REPOSITION_MESSAGE_H

#include <string>
#include "Message.h"

namespace Network
{
    ///
    /// @brief A message that requests a spatial entity either be positioned, or the current
    ///        position of a spatial entity (depending on who recieves it).
    ///
    class RepositionMessage : public Message
    {
    public:
#ifdef MIR_NETWORK_CONTROLLER_INTERNALS
        RepositionMessage();
#endif
        RepositionMessage(std::string identifier, bool xchanged, double x, bool ychanged, double y, bool zchanged, double z);
        virtual std::string Serialize();
        virtual void Deserialize(std::string data);

        std::string Identifier;
        bool XChanged;
        bool YChanged;
        bool ZChanged;
        double X;
        double Y;
        double Z;
    };
}

#endif
