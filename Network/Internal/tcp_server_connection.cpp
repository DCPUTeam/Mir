/*
 * The class definition for the network server connection.
 */

#include "tcp_server_connection.h"
#include "../Controller.h"
#include "../Message.h"

namespace Network
{
    namespace Internal
    {
        tcp_connection::pointer tcp_connection::create(Controller& controller, boost::asio::io_service& io_service)
        {
            return tcp_connection::pointer(new tcp_connection(controller, io_service));
        }

        tcp::socket& tcp_connection::socket()
        {
            return this->m_Socket;
        }
        
        ///
        /// Starts reading a message header from the connection.
        ///
        void tcp_connection::read_header()
        {
            std::cout << "Attempting to read " << sizeof(this->m_PacketLength) << " bytes from remote client." << std::endl;

            boost::asio::async_read(this->m_Socket,
                boost::asio::buffer(&this->m_PacketLength, sizeof(this->m_PacketLength)),
                boost::bind(&tcp_connection::handle_header, shared_from_this(),
                    boost::asio::placeholders::error));
        }
        
        ///
        /// Starts reading message data from the connection.
        ///
        void tcp_connection::read_message()
        {
            std::cout << "Attempting to read " << this->m_PacketLength << " bytes from remote client." << std::endl;

            boost::asio::async_read(this->m_Socket,
                boost::asio::buffer(this->m_PacketData, this->m_PacketLength),
                boost::bind(&tcp_connection::handle_message, shared_from_this(),
                    boost::asio::placeholders::error));
        }

        ///
        /// Handles the connection set up, including starting reading of packets.
        ///
        void tcp_connection::start()
        {
            std::cout << "Obtained connection from remote client." << std::endl;

            // Read the next header.
            this->read_header();
        }

        ///
        /// Handles an incoming message header.
        ///
        void tcp_connection::handle_header(const boost::system::error_code& error)
        {
            if (error)
            {
                std::cerr << "Error while reading message data from remote client." << std::endl;
                std::cerr << error.message() << std::endl;

                // Disconnect the user by not setting up any further
                // asynchronous operations.
                return;
            }

            std::cout << "Read header from remote client (" << this->m_PacketLength << " bytes to read)." << std::endl;

            // Construct an appropriate data block to hold the data.
            this->m_PacketData = new char[this->m_PacketLength];

            // Read the message body.
            this->read_message();
        }

        ///
        /// Handles incoming message data.
        ///
        void tcp_connection::handle_message(const boost::system::error_code& error)
        {
            if (error)
            {
                std::cerr << "Error while reading message data from remote client." << std::endl;
                
                // Disconnect the user by not setting up any further
                // asynchronous operations.
                return;
            }

            std::cout << "Read " << this->m_PacketLength << " bytes from remote client." << std::endl;

            // Make a copy of the length and data pointer (the data isn't freed;
            // that's the controller's responsibility).
            uint16_t length = this->m_PacketLength;
            char* data = this->m_PacketData;

            // Start reading another header (so we can accept them as fast as we can; we don't
            // need to wait for the controller to finish processing a message before getting
            // the next one).
            this->read_header();

            // Tell the controller that we have received a message.
            this->m_Controller.ReceiveData(length,  data);
        }

        tcp_connection::tcp_connection(Controller& controller, boost::asio::io_service& io_service)
            : m_Socket(io_service), m_Controller(controller)
        {
        }

        ///
        /// Handles writing of data (unused).
        ///
        void tcp_connection::handle_write(const boost::system::error_code& error,
            size_t bytes_transferred)
        {
        }
    }
}