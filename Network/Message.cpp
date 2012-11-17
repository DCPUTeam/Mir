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
    Message::Message(Network::Source source)
    {
        this->m_Source = source;
        this->m_Reliable = false;
        this->m_GlobalHandled = false;
    }

    ///
    /// @brief Constructor for building a message to be sent.
    ///
    /// @param reliable Whether the message should be sent reliably.
    ///
    Message::Message(bool reliable, bool global)
    {
        this->m_Source = Source();
        this->m_Reliable = reliable;
        this->m_GlobalHandled = global;
    }

    ///
    /// @brief Returns the current message.
    ///
    Source Message::GetSource()
    {
        return this->m_Source;
    }
    
    ///
    /// @brief Returns whether the current message should be reliably transferred.
    ///
    bool Message::IsReliable()
    {
        return this->m_Reliable;
    }
    
    ///
    /// @brief Returns whether the current message should be handled globally.
    ///
    bool Message::IsGlobal()
    {
        return this->m_GlobalHandled;
    }
}
