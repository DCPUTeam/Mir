/*
 * The class declaration for the network client.
 */

#ifndef __DCPU_GAME_NETWORKING_INTERNAL_TCP_CLIENT_H
#define __DCPU_GAME_NETWORKING_INTERNAL_TCP_CLIENT_H

namespace Network
{
    class Controller;

    namespace Internal
    {
        class tcp_client;
    }
}

#include <string>
#include <deque>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "tcp_client_connection.h"

using boost::asio::ip::tcp;

namespace Network
{
    namespace Internal
    {
        class tcp_client
        {
        public:
            tcp_client(Controller& controller, boost::asio::io_service& io_service, tcp::resolver::iterator iterator);
            void send(std::string data);
            void close();
        private:
            void start_write(std::string data);
            void start_close();
            void read_header();
            void read_message();
            void handle_header(const boost::system::error_code& error);
            void handle_message(const boost::system::error_code& error);
            void handle_connect(const boost::system::error_code& error);
            void handle_write(const boost::system::error_code& error, size_t bytes_transferred);

            boost::asio::io_service& m_IOService;
            tcp::socket m_Socket;
            std::string m_Message;
            Controller& m_Controller;
            std::deque<std::string> m_DataMessages;

            uint16_t m_PacketLength;
            char* m_PacketData;
        };
    }
}

#endif