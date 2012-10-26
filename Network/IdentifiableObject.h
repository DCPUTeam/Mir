/*
 * The class declaration for identifiable objects.
 */

#ifndef __DCPU_GAME_NETWORKING_IDENTIFIABLE_OBJECT_H
#define __DCPU_GAME_NETWORKING_IDENTIFIABLE_OBJECT_H

namespace Network
{
    class IdentifiableObject;
}

#include <string>
#include <boost/noncopyable.hpp>
#include "Message.h"
#include "Controller.h"

namespace Network
{
    class IdentifiableObject : private boost::noncopyable
    {
    private:
        Controller& m_Controller;
        std::string m_Identifier;
    protected:
        IdentifiableObject(Controller& controller, std::string identifier);
        ~IdentifiableObject();
        Controller& GetController();
        void SendMessage(Message& message);
    public:
        std::string GetIdentifier();
        virtual void ReceiveMessage(Message& message) = 0;
    };
}

#endif
