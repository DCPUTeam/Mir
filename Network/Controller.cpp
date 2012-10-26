/*
 * The class definition for the network controller.
 */

#include <exception>
#include <cassert>
#include <sstream>
#include "Controller.h"
#include "CreateMessage.h"
#include "PlayerJoinMessage.h"

namespace Network
{
    Controller::Controller(ControllerMode::Type mode, ObjectTranslation& translation, std::string address, int port)
        : m_Mode(mode), m_ConnectionAddress(address), m_ConnectionPort(port), ServerRunning(true), m_ObjectTranslation(translation)
    {
        // Initialize the local object mappings.
        this->m_LocalObjects = new std::map<std::string, IdentifiableObject*>();

        // Make the connection.
        this->m_IOService = new boost::asio::io_service();
        if (this->m_Mode == ControllerMode::Server)
            this->m_TCPServer = new Internal::tcp_server(*this, *this->m_IOService, port);
        else
        {
            // Resolve the address parameter.
            tcp::resolver resolver(*this->m_IOService);
            std::stringstream port_str;
            port_str << port;
            tcp::resolver::query query(address, port_str.str()); //, boost::asio::ip::resolver_query_base::flags::all_matching);
            tcp::resolver::iterator iterator = resolver.resolve(query);
            this->m_TCPClient = new Internal::tcp_client(*this, *this->m_IOService, iterator);
        }
    }
    
    Controller::~Controller()
    {
        // FIXME: Free memory inside the map first.
        delete this->m_LocalObjects;
    }

    void Controller::SendMessage(IdentifiableObject& object, Message& message)
    {
        this->SendMessage(object.GetIdentifier(), message);
    }

    void Controller::SendMessage(Message& message)
    {
        this->SendMessage("", message);
    }

    void Controller::SendMessage(std::string id, Message& message)
    {
        std::string data = message.Serialize();

        // Construct serialized representation.
        std::string serialized = std::string(typeid(message).name()) + "\1" + id + "\1" + data;

        // Send message to proper target(s).
        if (this->m_Mode == ControllerMode::Server)
        {
            // Broadcast message to all connected clients.
            this->m_TCPServer->broadcast(serialized);
        }
        else if (this->m_Mode == ControllerMode::Client)
        {
            // Send message to server.
            this->m_TCPClient->send(serialized);
        }
    }

    void Controller::ReceiveData(size_t length, char* data)
    {
        // Convert the raw data into an std::string.
        std::string message(data, length);

        // Find the first ':'.
        std::string::size_type at = message.find('\1');
        if (at == std::string::npos)
            return; // Invalid message.

        // Pull out the message type.
        std::string message_type = message.substr(0, at);
        message = message.substr(at + 1);

        // Find the second ':'.
        at = message.find('\1');
        if (at == std::string::npos)
            return; // Invalid message.

        // Pull out the owner ID.
        std::string message_owner = message.substr(0, at);
        message = message.substr(at + 1);

        // The rest of the message is the message data.  Reconstruct
        // based on the type.
        if (message_type == typeid(CreateMessage).name())
        {
            // Check whether it's valid for this mode.
            if (this->m_Mode != ControllerMode::Client)
                return;

            // This is a special creation message; the owner ID
            // is currently unused, but we need to create the new
            // object, depending on it's type.  First however, we need
            // to create a new CreateMessage and deserialize.
            CreateMessage create;
            create.Deserialize(message);

            // Create the new object.  The object is automatically registered
            // by it's constructor.
            this->m_ObjectTranslation.CreateByType(*this, create.Type, create.Identifier);
        }
        else if (message_type == typeid(PlayerJoinMessage).name())
        {
            // Check whether it's valid for this mode.
            if (this->m_Mode != ControllerMode::Server)
                return;

            // Deserialize player join message.
            PlayerJoinMessage player_join;
            player_join.Deserialize(message);
            this->ReceiveMessage(player_join);
        }
    }

    void Controller::ReceiveMessage(Network::Message& message)
    {
        if (typeid(message) == typeid(PlayerJoinMessage))
        {
            // Here we need to transmit the state of the universe to the client
            // the message came from.
            // TODO: Add source tracking into Message class and a send to one
            // target option for tcp_server.
            std::cout << "Server received a message of type " << typeid(message).name() << "!" << std::endl;

            CreateMessage create;
            create.Identifier = "blah1234567";
            create.Type = "Actor";
            this->SendMessage(create);
        }
        else
        {
            std::cout << "Client received a message of type " << typeid(message).name() << "!" << std::endl;
        }
        
        std::cerr << "FIXME Controller::ReceiveMessage!" << std::endl;
    }

    void Controller::Synchronise()
    {
        this->m_IOService->poll_one();
    }

    void Controller::Register(IdentifiableObject& object)
    {
        this->m_LocalObjects->insert(
            std::pair<std::string, IdentifiableObject*>(object.GetIdentifier(), &object)
        );
    }

    void Controller::Unregister(IdentifiableObject& object)
    {
        this->m_LocalObjects->erase(object.GetIdentifier());
    }

    IdentifiableObject* Controller::Find(std::string identifier)
    {
        for (std::map<std::string, IdentifiableObject*>::const_iterator i = this->m_LocalObjects->begin();
                i != this->m_LocalObjects->end(); i++)
        {
            if (i->first == identifier)
                return i->second;
        }

        return NULL;
    }
}