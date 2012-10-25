/*
 * The class declaration for the network controller.
 */

#ifndef __DCPU_GAME_NETWORKING_CONTROLLER_H
#define __DCPU_GAME_NETWORKING_CONTROLLER_H

namespace Network
{
    class Controller;
}

#include <map>
#include <string>
#include "IdentifiableObject.h"
#include "Internal/tcp_server_connection.h"
#include "Internal/tcp_server.h"

namespace Network 
{
    namespace ControllerMode
    {
        enum Type
        {
            Server,
            Client
        };
    }

    class Controller
    {
    private:
        std::map<std::string, IdentifiableObject*> m_LocalObjects;
        ControllerMode::Type m_Mode;
        std::string m_ConnectionAddress;
        boost::asio::io_service* m_IOService;
        Internal::tcp_server* m_TCPServer;
        int m_ConnectionPort;
    public:
        bool ServerRunning;

        Controller(ControllerMode::Type mode, std::string address, int port);
        void SendMessage(IdentifiableObject& object, Message& message);
        void ReceiveData(size_t length, char* data);
        void Synchronise();
        void Register(IdentifiableObject& object);
        void Unregister(IdentifiableObject& object);
        IdentifiableObject* Find(std::string identifier);
    };
}

#endif
