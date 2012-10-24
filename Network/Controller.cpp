/*
 * The class definition for the network controller.
 */

#include <cassert>
#include "Controller.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#undef SendMessage
#endif

namespace Network 
{
    Controller::Controller(ControllerMode::Type mode, std::string address, int port)
        : m_Mode(mode), m_ConnectionAddress(address), m_ConnectionPort(port), ServerRunning(true)
    {
        // Initialize the local object mappings.
        m_LocalObjects = std::map<std::string, IdentifiableObject*>();

        // Make the connection.
        if (mode == ControllerMode::Server)
        {
            int sockfd;

            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            assert(sockfd >= 0);
            memset(
        }
    }

    void Controller::SendMessage(IdentifiableObject& object, Message& message)
    {
        std::string id = object.GetIdentifier();
        std::string data = message.Serialize();
    }
    
    void Controller::Synchronise()
    {
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