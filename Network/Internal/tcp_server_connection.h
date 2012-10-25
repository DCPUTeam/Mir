/*
 * The class declaration for the network server connection.
 */

#ifndef __DCPU_GAME_NETWORKING_INTERNAL_TCP_CONNECTION_H
#define __DCPU_GAME_NETWORKING_INTERNAL_TCP_CONNECTION_H

namespace Network
{
    class Controller;

    namespace Internal
    {
        class tcp_connection;
    }
}

#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace Network
{
    namespace Internal
    {
        class tcp_connection
            : public boost::enable_shared_from_this<tcp_connection>
        {
        public:
            typedef boost::shared_ptr<tcp_connection> pointer;

            static pointer create(Controller& controller, boost::asio::io_service& io_service);
            tcp::socket& socket();
            void start();
        private:
            tcp_connection(Controller& controller, boost::asio::io_service& io_service);
            void read_header();
            void read_message();
            void handle_header(const boost::system::error_code& error);
            void handle_message(const boost::system::error_code& error);
            void handle_write(const boost::system::error_code& error, size_t bytes_transferred);

            tcp::socket m_Socket;
            std::string m_Message;
            Controller& m_Controller;

            uint16_t m_PacketLength;
            char* m_PacketData;
        };
    }
}

#endif