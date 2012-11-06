/*
 * The class definition for the network server.
 */

#define MIR_NETWORK_CONTROLLER_INTERNALS

#include "tcp_server.h"
#include "../Controller.h"

namespace Network
{
    namespace Internal
    {
        tcp_server::tcp_server(Controller& controller, boost::asio::io_service& io_service, int port)
            : m_Acceptor(io_service, tcp::endpoint(tcp::v4(), port)), m_Controller(controller)
        {
            this->start_accept();
            this->m_Controller.SetConnectionStatus(false, true);
        }
        
        void tcp_server::broadcast(std::string data)
        {
            for (std::list<tcp_server_connection::pointer>::const_iterator i = this->m_Clients.begin();
                i != this->m_Clients.end(); i++)
            {
                (*i)->broadcast(data);
            }
        }

        void tcp_server::start_accept()
        {
            tcp_server_connection::pointer new_connection =
                tcp_server_connection::create(this->m_Controller, this->m_Acceptor.get_io_service());

            this->m_Acceptor.async_accept(new_connection->socket(),
                                          boost::bind(&tcp_server::handle_accept, this, new_connection,
                                                  boost::asio::placeholders::error));
        }

        void tcp_server::handle_accept(tcp_server_connection::pointer new_connection,
                                       const boost::system::error_code& error)
        {
            if (!error)
            {
                new_connection->start();
                this->m_Clients.insert(this->m_Clients.end(),
                    new_connection);
            }

            this->start_accept();
        }
    }
}