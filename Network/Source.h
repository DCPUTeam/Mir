///
/// @brief The declaration of a class that represents a message source.
/// @author James Rhodes
///

#ifndef __DCPU_GAME_NETWORKING_SOURCE_H
#define __DCPU_GAME_NETWORKING_SOURCE_H

#include "Internal/tcp_server_connection.h"

namespace Network
{
    class Source
    {
    public:
        Source() { };
        Source(Internal::tcp_server_connection* p)
            : Client(p) { };
        Source(Internal::tcp_server_connection::pointer p)
            : Client(p) { };
        Internal::tcp_server_connection::pointer Client;
        inline bool operator==(const Source& other) const
        {
            return (other.Client == this->Client);
        }
        inline bool operator!=(const Source& other) const
        {
            return !(other == *this);
        }
    };
}

#endif