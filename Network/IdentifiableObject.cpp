/*
 * The class definition for identifiable objects.
 */

#include "IdentifiableObject.h"

namespace Network
{
    IdentifiableObject::IdentifiableObject(Controller& controller, std::string identifier)
        : m_Controller(controller), m_Identifier(identifier)
    {
        this->m_Controller.Register(*this);
    }

    IdentifiableObject::~IdentifiableObject()
    {
        this->m_Controller.Unregister(*this);
    }

    Controller& IdentifiableObject::GetController()
    {
        return this->m_Controller;
    }

    std::string IdentifiableObject::GetIdentifier()
    {
        return this->m_Identifier;
    }

    void IdentifiableObject::SendMessage(Message& message)
    {
        this->m_Controller.SendMessage(*this, message);
    }

    void IdentifiableObject::Resynchronise()
    {
        // By default we resynchronise nothing since we don't
        // know the structure of the class...
    }
}