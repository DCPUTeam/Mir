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
#include "Internal/tcp_client_connection.h"
#include "Internal/tcp_client.h"
#include "ObjectTranslation.h"
#include "Message.h"

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
        Internal::tcp_client* m_TCPClient;
        ObjectTranslation& m_ObjectTranslation;
        int m_ConnectionPort;

        void SendMessage(std::string id, Message& message);
    public:
        bool ServerRunning;

        Controller(ControllerMode::Type mode, ObjectTranslation& translation, std::string address, int port);
        void SendMessage(Message& message);
        void SendMessage(IdentifiableObject& object, Message& message);
        void ReceiveData(size_t length, char* data);
        void ReceiveMessage(Message& message);
        void Synchronise();
        void Register(IdentifiableObject& object);
        void Unregister(IdentifiableObject& object);
        IdentifiableObject* Find(std::string identifier);
    };
}

#endif
