///
/// @brief The class declaration for repositioning messages.
/// @author James Rhodes
///

#ifndef __MIR_NETWORKING_REPOSITION_MESSAGE_H
#define __MIR_NETWORKING_REPOSITION_MESSAGE_H

#include <string>
#include "ObjectMessage.h"

namespace Network
{
    ///
    /// @brief A message that requests a spatial entity either be positioned, or the current
    ///        position of a spatial entity (depending on who recieves it).
    ///
    class RepositionMessage : public ObjectMessage
    {
    public:
#ifdef MIR_NETWORK_CONTROLLER_INTERNALS
        RepositionMessage(Source* source);
#endif
        RepositionMessage(std::string identifier, bool xchanged, double x, bool ychanged, double y, bool zchanged, double z);
        virtual std::string Serialize();
        virtual void Deserialize(std::string data);
        virtual int GetHashCode();

        bool XChanged;
        bool YChanged;
        bool ZChanged;
        double X;
        double Y;
        double Z;
    };
}

#endif
