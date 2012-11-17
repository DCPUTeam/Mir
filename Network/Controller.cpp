/*
 * The class definition for the network controller.
 */

#define MIR_NETWORK_CONTROLLER_INTERNALS

#include <exception>
#include <cassert>
#include <sstream>
#include "Controller.h"
#include <Messages/Declarations.h>

namespace Network
{
    Controller::Controller(ControllerMode::Type mode, ObjectTranslation& translation, std::string address, int port)
        : m_Mode(mode), m_ConnectionAddress(address), m_ConnectionPort(port), ServerRunning(true),
          m_ObjectTranslation(translation), Connected(false), Connecting(true), m_GlobalHandler(NULL)
    {
        // Initialize the local object or request cache mappings.
        if (this->m_Mode == ControllerMode::Server)
        {
            this->m_LocalObjects = new std::map<std::string, IdentifiableObject*>();
            this->m_RequestCache = new std::map<std::string, RequestState*>();
        }
        else
        {
            this->m_LocalObjects = NULL;
            this->m_RequestCache = new std::map<std::string, RequestState*>();
        }

        // Make the connection.
        this->m_IOService = new boost::asio::io_service();
        if (this->m_Mode == ControllerMode::Server)
            this->m_TCPServer = new Internal::tcp_server(*this, *this->m_IOService, port);
        else
        {
            // Resolve the address parameter.
            try
            {
                tcp::resolver resolver(*this->m_IOService);
                std::stringstream port_str;
                port_str << port;
                std::cout << "Resolving " << address << ":" << port_str.str() << "." << std::endl;
                tcp::resolver::query query(address, port_str.str());
                tcp::resolver::iterator iterator = resolver.resolve(query);
                this->m_TCPClient = new Internal::tcp_client(*this, *this->m_IOService, iterator);
            }
            catch (...)
            {
                this->Error("Unable to resolve the hostname / address.");
                this->SetConnectionStatus(false, false);
            }
        }
    }
    
    Controller::~Controller()
    {
        // FIXME: Free memory inside the map first.
        delete this->m_RequestCache;
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
        std::string name = Messages::GetType(message);

        // Construct serialized representation.
        std::string serialized = std::string(name) + "\1" + id + "\1" + data;

        // Send message to proper target(s).
        if (this->m_Mode == ControllerMode::Server)
        {
            // Broadcast message to all connected clients.
            std::list<Source> clients = this->m_TCPServer->clients();
            for (Internal::tcp_server::client_iterator i = clients.begin(); i != clients.end(); i++)
            {
                if (*i != message.GetSource())
                    this->m_TCPServer->broadcast(*i, serialized);
            }
        }
        else if (this->m_Mode == ControllerMode::Client)
        {
            // Send message to server.
            this->m_TCPClient->send(serialized);
        }
    }

    void Controller::ReceiveData(Source source, size_t length, char* data)
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
        
        // Deserialize based on the type.
        ObjectMessage* result = Messages::DeserializeByType(source, message_type, message);
        if (result == NULL)
        {
            std::cerr << "UNKNOWN MESSAGE TYPE RECEIVED '" << message_type << "'." << std::endl;
            return;
        }
        
        // If the result has a non-blank identifier, search for that object and send
        // the message to it.
        if (result->Identifier == "" || result->IsGlobal())
            this->ReceiveMessage(*result);
        else
        {
            // This message needs to be sent to an object.
            if (this->m_LocalObjects != NULL)
            {
                for (std::map<std::string, IdentifiableObject*>::const_iterator i = this->m_LocalObjects->begin();
                        i != this->m_LocalObjects->end(); i++)
                {
                    if (i->second == NULL)
                        continue;
                    if (i->first == result->Identifier)
                    {
                        i->second->ReceiveMessage(*result);
                        delete result;
                        return;
                    }
                }
            }

            for (std::map<std::string, RequestState*>::const_iterator i = this->m_RequestCache->begin();
                    i != this->m_RequestCache->end(); i++)
            {
                if (i->second == NULL)
                    continue;
                if (i->first == result->Identifier)
                    if (i->second->Status == REQUEST_STATUS_AVAILABLE)
                    {
                        i->second->Reference->ReceiveMessage(*result);
                        delete result;
                        return;
                    }
            }
            
            // FIXME: Use a proper logging system.
            std::cerr << "Silently dropping message of type '" << Messages::GetType(*result) << "'." << std::endl;
        }
        
        // Delete the message from memory.
        delete result;
    }

    void Controller::ReceiveMessage(Network::Message& message)
    {
        // Handle global messages.
        if (Messages::GetType(message) == Messages::ID_REQUESTMESSAGE)
        {
            // Check whether it's valid for this mode.
            if (this->m_Mode != ControllerMode::Server)
                return;

            // Attempt to find the specified object in our local object storage.
            for (std::map<std::string, IdentifiableObject*>::const_iterator i = this->m_LocalObjects->begin();
                    i != this->m_LocalObjects->end(); i++)
            {
                if (i->first == ((RequestMessage&)message).Identifier)
                {
                    // We have found the matching object.  Send back a
                    // CreateMessage that informs the client that the
                    // object does exist.
                    CreateMessage create(
                        this->m_ObjectTranslation.GetType(i->second),
                        i->first
                        );
                    this->SendMessage(create);
                    return;
                }
            }

            // The specified object does not exist.  Tell the client
            // that the object isn't in existance.
            NotFoundMessage notfound(
                ((RequestMessage&)message).Identifier
                );
            this->SendMessage(notfound);
        }
        else if (Messages::GetType(message) == Messages::ID_CREATEMESSAGE)
        {
            // Check whether it's valid for this mode.
            if (this->m_Mode != ControllerMode::Client)
                return;

            // Handle the creation of the object.
            CreateMessage& create = (CreateMessage&)message;
            this->m_ObjectTranslation.CreateByType(*this, create.Type, create.Identifier);
            
            // Also notify the global handler.
            if (this->m_GlobalHandler != NULL)
                this->m_GlobalHandler->ReceiveMessage(message);
        }
        else if (Messages::GetType(message) == Messages::ID_NOTFOUNDMESSAGE)
        {
            // Check whether it's valid for this mode.
            if (this->m_Mode != ControllerMode::Client)
                return;

            // Mark the request as failed in the request cache.
            for (std::map<std::string, RequestState*>::const_iterator i = this->m_RequestCache->begin();
                    i != this->m_RequestCache->end(); i++)
            {
                if (i->second == NULL)
                    continue;
                if (i->first == ((NotFoundMessage&)message).Identifier)
                    i->second->Status = REQUEST_STATUS_ERROR;
            }
        }
        else
        {
            // Pass off to the global handler to manage.
            if (this->m_GlobalHandler != NULL)
                this->m_GlobalHandler->ReceiveMessage(message);
        }
    }

    ///
    /// @brief Sets the global handling object which should recieve global incoming messages.
    ///
    void Controller::SetGlobalHandler(IdentifiableObject* handler)
    {
        this->m_GlobalHandler = handler;
    }
    
    ///
    /// @brief Returns the global handling object.
    ///
    IdentifiableObject* Controller::GetGlobalHandler()
    {
        return this->m_GlobalHandler;
    }
        
    ///
    /// @brief Performs a single synchronisation step of the network.
    ///
    void Controller::Synchronise()
    {
        // Poll the network for the next event.
        int max = 0;
        if (this->m_Mode == ControllerMode::Client)
            max = 4;
        else if (this->m_Mode == ControllerMode::Server)
            max = 10000000; //20 /* client sending */ * 4 /* messages per step */ * 20 /* broadcasting to 20 clients */;
        int i = 0;
        while (this->m_IOService->poll_one() != 0 && i++ < max)
            /* Network messages are processed... */;

        // Check outstanding requests.
        if (this->m_Mode == ControllerMode::Client)
        {
            for (std::map<std::string, RequestState*>::const_iterator i = this->m_RequestCache->begin();
                    i != this->m_RequestCache->end(); i++)
            {
                if (i->second == NULL)
                    continue;
                if (i->second->Status == REQUEST_STATUS_PENDING)
                {
                    // Send out a request for this object.
                    RequestMessage request(i->first);
                    this->SendMessage(request);
                    i->second->Status = REQUEST_STATUS_SENT;
                }
            }
        }
    }

    ///
    /// @brief Registers an identifiable object with the local storage.
    ///
    void Controller::Register(IdentifiableObject& object)
    {
        if (this->m_Mode == ControllerMode::Server)
        {
            // As a server, we keep it in our local objects.
            this->m_LocalObjects->insert(
                std::pair<std::string, IdentifiableObject*>(object.GetIdentifier(), &object)
            );
        }
        else
        {
            // As a client, it was derived from a request in the request cache.
            for (std::map<std::string, RequestState*>::const_iterator i = this->m_RequestCache->begin();
                    i != this->m_RequestCache->end(); i++)
            {
                if (i->second == NULL)
                    continue;
                if (i->first == object.GetIdentifier())
                {
                    i->second->Status = REQUEST_STATUS_AVAILABLE;
                    i->second->Reference = &object;
                }
            }
        }
    }

    ///
    /// @brief Unregisters an identifiable object from local storage.
    ///
    void Controller::Unregister(IdentifiableObject& object)
    {
        if (this->m_Mode == ControllerMode::Server)
        {
            this->m_LocalObjects->erase(object.GetIdentifier());
        }
    }

    ///
    /// @brief Request an object asynchronously from the network.
    ///
    /// @param identifier The unique identifier of the object.
    /// @return The request state representing the state of the object.
    ///
    RequestState& Controller::Request(std::string identifier)
    {
        // First check to see if the object is already in the request
        // cache.
        for (std::map<std::string, RequestState*>::const_iterator i = this->m_RequestCache->begin();
                i != this->m_RequestCache->end(); i++)
        {
            if (i->first == identifier)
                return *(i->second);
        }

        // It might also be a local object not stored in the request
        // cache.  If so, add it to the request cache and return.
        if (this->m_LocalObjects != NULL)
        {
            for (std::map<std::string, IdentifiableObject*>::const_iterator i = this->m_LocalObjects->begin();
                    i != this->m_LocalObjects->end(); i++)
            {
                if (i->first == identifier)
                {
                    // It is in the local storage, but not the request
                    // cache.  We fake a request with the result immediately
                    // set to REQUEST_STATUS_AVAILABLE.
                    RequestState* state = new RequestState();
                    state->Status = REQUEST_STATUS_AVAILABLE;
                    state->Reference = i->second;
                    this->m_RequestCache->insert(
                        this->m_RequestCache->begin(),
                        std::pair<std::string, RequestState*>(identifier, state)
                        );
                    return *state;
                }
            }
        }

        // The object is not in the request cache; let's create a new
        // request state.
        RequestState* state = new RequestState();
        state->Status = REQUEST_STATUS_PENDING;
        state->Reference = NULL;
        this->m_RequestCache->insert(
            this->m_RequestCache->begin(),
            std::pair<std::string, RequestState*>(identifier, state)
            );
        return *state;
    }

    ///
    /// @brief Returns a list of all known objects in the system.
    ///
    std::list<IdentifiableObject*> Controller::GetKnownObjects()
    {
        return std::list<IdentifiableObject*>();
    }
    
    ///
    /// @brief Returns whether the controller is currently a server.
    ///
    bool Controller::IsServer()
    {
        return this->m_Mode == ControllerMode::Server;
    }

    void Controller::Close()
    {
        // Disconnect the server / client from each other.
        if (this->m_Mode == ControllerMode::Client)
        {
            //std::cout << "Now closing TCP client..." << std::endl;
            this->m_TCPClient->close();
            this->m_IOService->poll_one();
        }

        // Close the connection and delete ourselves.
        this->m_IOService->stop();
        delete this;
    }

    void Controller::SetConnectionStatus(bool connecting, bool connected)
    {
        this->Connecting = connecting;
        this->Connected = connected;
    }
    
    void Controller::Error(std::string err)
    {
        this->m_Errors.push(err);
    }
    
    void Controller::Warning(std::string err)
    {
        this->m_Warnings.push(err);
    }

    bool Controller::HasError()
    {
        return this->m_Errors.size() > 0;
    }

    bool Controller::HasWarning()
    {
        return this->m_Warnings.size() > 0;
    }

    std::string Controller::NextError()
    {
        std::string result = this->m_Errors.front();
        this->m_Errors.pop();
        return result;
    }

    std::string Controller::NextWarning()
    {
        std::string result = this->m_Warnings.front();
        this->m_Warnings.pop();
        return result;
    }
}
