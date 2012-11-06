/*
 * The class definition for the network client.
 */

#define MIR_NETWORK_CONTROLLER_INTERNALS

#include <stdint.h>
#include "tcp_client.h"
#include "../Controller.h"
#include <boost/bind.hpp>

namespace Network
{
    namespace Internal
    {
        tcp_client::tcp_client(Controller& controller, boost::asio::io_service& io_service, tcp::resolver::iterator iterator)
            : m_IOService(io_service), m_Socket(io_service), m_Controller(controller)
        {
            boost::asio::async_connect(this->m_Socket, iterator,
                boost::bind(&tcp_client::handle_connect, this,
                    boost::asio::placeholders::error));
        }
        
        ///
        /// Pushes a message to be sent during the next cycle.
        ///
        void tcp_client::send(std::string data)
        {
            this->m_IOService.post(boost::bind(&tcp_client::start_write, this, data));
        }
        
        ///
        /// Closes the connection at the next cycle.
        ///
        void tcp_client::close()
        {
            this->m_IOService.post(boost::bind(&tcp_client::start_close, this));
        }

        ///
        /// Starts writing data to the server.
        ///
        void tcp_client::start_write(std::string data)
        {
            //std::cout << "Pushing new message onto deque for sending to server." << std::endl;

            bool write_in_progress = !this->m_DataMessages.empty();

            // Create header information.
            if (data.length() > UINT16_MAX)
            {
                this->m_Controller.Warning("Packet is larger than UINT16_MAX and can not be sent.");
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
                    boost::bind(&tcp_client::handle_write, this, 
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
            }
        }

        ///
        /// Starts closing the connection.
        ///
        void tcp_client::start_close()
        {
            this->m_Socket.close();
        }

        ///
        /// Starts reading a message header from the connection.
        ///
        void tcp_client::read_header()
        {
            //std::cout << "Attempting to read " << sizeof(this->m_PacketLength) << " bytes from remote server." << std::endl;

            boost::asio::async_read(this->m_Socket,
                                    boost::asio::buffer(&this->m_PacketLength, sizeof(this->m_PacketLength)),
                                    boost::bind(&tcp_client::handle_header, this,
                                                boost::asio::placeholders::error));
        }

        ///
        /// Starts reading message data from the connection.
        ///
        void tcp_client::read_message()
        {
            //std::cout << "Attempting to read " << this->m_PacketLength << " bytes from remote server." << std::endl;

            boost::asio::async_read(this->m_Socket,
                                    boost::asio::buffer(this->m_PacketData, this->m_PacketLength),
                                    boost::bind(&tcp_client::handle_message, this,
                                                boost::asio::placeholders::error));
        }

        ///
        /// Handles connection to the server.
        ///
        void tcp_client::handle_connect(const boost::system::error_code& error)
        {
            //std::cout << "Obtained connection to remote server." << std::endl;
            this->m_Controller.SetConnectionStatus(false, true);

            // Read the next header.
            this->read_header();
        }

        ///
        /// Handles after data has been written to the server.
        ///
        void tcp_client::handle_write(const boost::system::error_code& error, size_t bytes_transferred)
        {
            //std::cout << "Sent " << bytes_transferred << " bytes to remote server." << std::endl;

            if (!error)
            {
                this->m_DataMessages.pop_front();
                if (!this->m_DataMessages.empty())
                {
                    boost::asio::async_write(this->m_Socket, boost::asio::buffer(this->m_DataMessages.front()),
                        boost::bind(&tcp_client::handle_write, this, 
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
                }
            }
            else
                this->start_close();
        }
        
        ///
        /// Handles an incoming message header.
        ///
        void tcp_client::handle_header(const boost::system::error_code& error)
        {
            if (error)
            {
                //std::cerr << "Error while reading message data from remote server." << std::endl;
                this->m_Controller.Error(error.message());
                this->m_Controller.SetConnectionStatus(false, false);

                // Disconnect the user by not setting up any further
                // asynchronous operations.
                return;
            }

            //std::cout << "Read header from remote server (" << this->m_PacketLength << " bytes to read)." << std::endl;

            // Construct an appropriate data block to hold the data.
            this->m_PacketData = new char[this->m_PacketLength];

            // Read the message body.
            this->read_message();
        }

        ///
        /// Handles incoming message data.
        ///
        void tcp_client::handle_message(const boost::system::error_code& error)
        {
            if (error)
            {
                //std::cerr << "Error while reading message data from remote server." << std::endl;
                this->m_Controller.Error(error.message());
                this->m_Controller.SetConnectionStatus(false, false);

                // Disconnect the user by not setting up any further
                // asynchronous operations.
                return;
            }

            //std::cout << "Read " << this->m_PacketLength << " bytes from remote server." << std::endl;

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
    }
}