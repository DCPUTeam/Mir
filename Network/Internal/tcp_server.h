/*
 * The class declaration for the network server.
 */

#ifndef __DCPU_GAME_NETWORKING_INTERNAL_TCP_SERVER_H
#define __DCPU_GAME_NETWORKING_INTERNAL_TCP_SERVER_H

namespace Network
{
    class Controller;

    namespace Internal
    {
        class tcp_server;
    }
}

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "tcp_server_connection.h"

using boost::asio::ip::tcp;

namespace Network
{
    namespace Internal
    {
        class tcp_server
        {
        public:
            tcp_server(Controller& controller, boost::asio::io_service& io_service);
        private:
            void start_accept();
            void handle_accept(tcp_connection::pointer new_connection,
                const boost::system::error_code& error);

            tcp::acceptor m_Acceptor;
            Controller& m_Controller;
        };
    }
}

#endif