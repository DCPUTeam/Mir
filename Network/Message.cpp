///
/// @brief The implementation of the base message.
/// @author James Rhodes
///

#define MIR_NETWORK_CONTROLLER_INTERNALS

#include <exception>
#include "Message.h"

namespace Network
{
    ///
    /// @brief Constructor used for deserialization.
    /// @internal
    ///
    Message::Message(Source* source)
    {
        this->m_Source = source;
        this->m_Reliable = false;
    }

    ///
    /// @brief Constructor for building a message to be sent.
    ///
    /// @param reliable Whether the message should be sent reliably.
    ///
    Message::Message(bool reliable)
    {
        this->m_Source = NULL;
        this->m_Reliable = reliable;
    }

    ///
    /// @brief Returns the current message.
    ///
    Source* Message::GetSource()
    {
        return this->m_Source;
    }
}
