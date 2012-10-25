/*
 * The class definition for the network server.
 */

#include "tcp_server.h"

namespace Network
{
    namespace Internal
    {
        tcp_server::tcp_server(Controller& controller, boost::asio::io_service& io_service)
            : m_Acceptor(io_service, tcp::endpoint(tcp::v4(), 5055)), m_Controller(controller)
        {
            this->start_accept();
        }

        void tcp_server::start_accept()
        {
            tcp_connection::pointer new_connection =
                tcp_connection::create(this->m_Controller, this->m_Acceptor.get_io_service());

            this->m_Acceptor.async_accept(new_connection->socket(),
                boost::bind(&tcp_server::handle_accept, this, new_connection,
                    boost::asio::placeholders::error));
        }

        void tcp_server::handle_accept(tcp_connection::pointer new_connection,
            const boost::system::error_code& error)
        {
            if (!error)
                new_connection->start();

            this->start_accept();
        }
    }
}