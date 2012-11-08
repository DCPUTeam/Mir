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
#include <queue>
#include <boost/noncopyable.hpp>
#include "IdentifiableObject.h"
#include "Internal/tcp_server_connection.h"
#include "Internal/tcp_server.h"
#include "Internal/tcp_client_connection.h"
#include "Internal/tcp_client.h"
#include "ObjectTranslation.h"
#include "Message.h"
#include "Source.h"

namespace Network
{
    namespace ControllerMode
    {
        ///
        /// @brief The current mode of the network controller.
        ///
        enum Type
        {
            Server,
            Client
        };
    }
    
    ///
    /// @brief An enumeration representing the possible request statuses.
    ///
    typedef enum
    {
        REQUEST_STATUS_PENDING,
        REQUEST_STATUS_SENT,
        REQUEST_STATUS_AVAILABLE,
        REQUEST_STATUS_ERROR
    } RequestStatus;

    ///
    /// @brief A class representing a request for a remote object.
    ///
    /// Since the initial transfer of an object's information from the remote
    /// server to the client will be an asynchronous operation, we need a reference
    /// that indicates when the object is available for use.
    ///
    class RequestState
    {
    public:
        ///
        /// @brief The current status of the object.
        ///
        RequestStatus Status;
        ///
        /// @brief The reference to the object, or NULL if it is not yet available.
        ///
        IdentifiableObject* Reference;
    };

    ///
    /// @brief The network controller class.
    ///
    class Controller : private boost::noncopyable
    {
    private:
        std::map<std::string, IdentifiableObject*>* m_LocalObjects;
        std::map<std::string, RequestState*>* m_RequestCache;
        std::queue<std::string> m_Errors;
        std::queue<std::string> m_Warnings;
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
        bool Connecting;
        bool Connected;

        Controller(ControllerMode::Type mode, ObjectTranslation& translation, std::string address, int port);
        ~Controller();
        void SendMessage(Message& message);
        void SendMessage(IdentifiableObject& object, Message& message);
        void ReceiveData(Source* source, size_t length, char* data);
        void ReceiveMessage(Message& message);
        void Synchronise();
        void Register(IdentifiableObject& object);
        void Unregister(IdentifiableObject& object);
        void Close();
        bool IsServer();

        RequestState& Request(std::string identifier);

#ifdef MIR_NETWORK_CONTROLLER_INTERNALS
        void SetConnectionStatus(bool connecting, bool connected);
        void Error(std::string err);
        void Warning(std::string warn);
#endif

        bool HasError();
        bool HasWarning();
        std::string NextError();
        std::string NextWarning();
    };
}

#endif
