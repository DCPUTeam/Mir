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
        tcp_server_connection::pointer tcp_server_connection::create(Controller& controller, boost::asio::io_service& io_service)
        {
            return tcp_server_connection::pointer(new tcp_server_connection(controller, io_service));
        }

        tcp::socket& tcp_server_connection::socket()
        {
            return this->m_Socket;
        }
        
        ///
        /// Writes a message to the client.
        ///
        void tcp_server_connection::broadcast(std::string data)
        {
            //std::cout << "Pushing new message onto deque for sending to client." << std::endl;

            bool write_in_progress = !this->m_DataMessages.empty();

            // Create header information.
            if (data.length() > UINT16_MAX)
            {
                //std::cerr << "ERROR: Packet is larger than UINT16_MAX and can not be sent." << std::endl;
                return;
            }
            uint16_t l = data.length();
            char* lc = (char*)&l;
            std::string header = std::string(lc, sizeof(uint16_t));
            std::string combined = header + data;

            // Push message.
            this->m_DataMessages.push_back(combined);
            if (!write_in_progress)
            {
                boost::asio::async_write(this->m_Socket, boost::asio::buffer(this->m_DataMessages.front()),
                    boost::bind(&tcp_server_connection::handle_write, this, 
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
            }
        }

        ///
        /// Starts reading a message header from the connection.
        ///
        void tcp_server_connection::read_header()
        {
            //std::cout << "Attempting to read " << sizeof(this->m_PacketLength) << " bytes from remote client." << std::endl;

            boost::asio::async_read(this->m_Socket,
                                    boost::asio::buffer(&this->m_PacketLength, sizeof(this->m_PacketLength)),
                                    boost::bind(&tcp_server_connection::handle_header, shared_from_this(),
                                                boost::asio::placeholders::error));
        }

        ///
        /// Starts reading message data from the connection.
        ///
        void tcp_server_connection::read_message()
        {
            //std::cout << "Attempting to read " << this->m_PacketLength << " bytes from remote client." << std::endl;

            boost::asio::async_read(this->m_Socket,
                                    boost::asio::buffer(this->m_PacketData, this->m_PacketLength),
                                    boost::bind(&tcp_server_connection::handle_message, shared_from_this(),
                                                boost::asio::placeholders::error));
        }

        ///
        /// Handles the connection set up, including starting reading of packets.
        ///
        void tcp_server_connection::start()
        {
            //std::cout << "Obtained connection from remote client." << std::endl;

            // Read the next header.
            this->read_header();
        }

        ///
        /// Handles an incoming message header.
        ///
        void tcp_server_connection::handle_header(const boost::system::error_code& error)
        {
            if (error)
            {
                //std::cerr << "Error while reading message data from remote client." << std::endl;
                //std::cerr << error.message() << std::endl;

                // Disconnect the user by not setting up any further
                // asynchronous operations.
                return;
            }

            //std::cout << "Read header from remote client (" << this->m_PacketLength << " bytes to read)." << std::endl;

            // Construct an appropriate data block to hold the data.
            this->m_PacketData = new char[this->m_PacketLength];

            // Read the message body.
            this->read_message();
        }

        ///
        /// Handles incoming message data.
        ///
        void tcp_server_connection::handle_message(const boost::system::error_code& error)
        {
            if (error)
            {
                //std::cerr << "Error while reading message data from remote client." << std::endl;

                // Disconnect the user by not setting up any further
                // asynchronous operations.
                return;
            }

            //std::cout << "Read " << this->m_PacketLength << " bytes from remote client." << std::endl;

            // Make a copy of the length and data pointer (the data isn't freed;
            // that's the controller's responsibility).
            uint16_t length = this->m_PacketLength;
            char* data = this->m_PacketData;

            // Start reading another header (so we can accept them as fast as we can; we don't
            // need to wait for the controller to finish processing a message before getting
            // the next one).
            this->read_header();

            // Tell the controller that we have received a message.
            this->m_Controller.ReceiveData(new Source(this), length,  data);
        }

        tcp_server_connection::tcp_server_connection(Controller& controller, boost::asio::io_service& io_service)
            : m_Socket(io_service), m_Controller(controller)
        {
        }

        ///
        /// Handles after data has been written to the client.
        ///
        void tcp_server_connection::handle_write(const boost::system::error_code& error,
                                          size_t bytes_transferred)
        {
            //std::cout << "Sent " << bytes_transferred << " bytes to remote client." << std::endl;

            if (!error)
            {
                this->m_DataMessages.pop_front();
                if (!this->m_DataMessages.empty())
                {
                    boost::asio::async_write(this->m_Socket, boost::asio::buffer(this->m_DataMessages.front()),
                        boost::bind(&tcp_server_connection::handle_write, this, 
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
                }
            }
            else
                // FIXME: this->start_close();
                return;
        }
    }
}