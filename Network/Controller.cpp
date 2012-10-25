/*
 * The class definition for the network controller.
 */

#include <cassert>
#include "Controller.h"

namespace Network 
{
    Controller::Controller(ControllerMode::Type mode, std::string address, int port)
        : m_Mode(mode), m_ConnectionAddress(address), m_ConnectionPort(port), ServerRunning(true)
    {
        // Initialize the local object mappings.
        this->m_LocalObjects = std::map<std::string, IdentifiableObject*>();

        // Make the connection.
        if (this->m_Mode == ControllerMode::Server)
        {
            this->m_IOService = new boost::asio::io_service();
            this->m_TCPServer = new Internal::tcp_server(*this, *this->m_IOService);
        }
    }

    void Controller::SendMessage(IdentifiableObject& object, Message& message)
    {
        std::string id = object.GetIdentifier();
        std::string data = message.Serialize();

        if (this->m_Mode == ControllerMode::Server)
        {
            // Broadcast message to all connected clients.

        }
    }
    
    void Controller::ReceiveData(size_t length, char* data)
    {
        // Ask the message class to deserialize the raw data.

    }

    void Controller::Synchronise()
    {
        if (this->m_Mode == ControllerMode::Server)
        {
            this->m_IOService->poll_one();
        }
    }
    
    void Controller::Register(IdentifiableObject& object)
    {
        this->m_LocalObjects.insert(
            std::pair<std::string, IdentifiableObject*>(object.GetIdentifier(), &object)
            );
    }
   
    void Controller::Unregister(IdentifiableObject& object)
    {
        this->m_LocalObjects.erase(object.GetIdentifier());
    }
    
    IdentifiableObject* Controller::Find(std::string identifier)
    {
        for (std::map<std::string, IdentifiableObject*>::const_iterator i = this->m_LocalObjects.begin();
            i != this->m_LocalObjects.end(); i++)
        {
            if (i->first == identifier)
                return i->second;
        }

        return NULL;
    }
}