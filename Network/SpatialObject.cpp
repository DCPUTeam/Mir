///
/// @brief The implementation for spatial entities.
/// @author James Rhodes
///

#include "SpatialObject.h"
#include "RepositionMessage.h"

namespace Network
{
    ///
    /// @brief Resynchronizes this entity over the network.
    ///
    void SpatialObject::Resynchronise()
    {
        if (this->m_PreviousX == this->X &&
            this->m_PreviousY == this->Y &&
            this->m_PreviousZ == this->Z)
            return;

        RepositionMessage message(
            this->GetIdentifier(),
            this->m_PreviousX != this->X,
            this->X,
            this->m_PreviousY != this->Y,
            this->Y,
            this->m_PreviousZ != this->Z,
            this->Z
            );
        this->SendMessage(message);

        this->m_PreviousX = this->X;
        this->m_PreviousY = this->Y;
        this->m_PreviousZ = this->Z;

        IdentifiableObject::Resynchronise();
    }

    ///
    /// @brief Handles receiving a message from the network.
    ///
    void SpatialObject::ReceiveMessage(Network::Message& message)
    {
        if (typeid(message) == typeid(RepositionMessage))
        {
            // TODO: If the controller is currently a server controller, verify the
            // player is permitted to send this message.

            // Apply the message.
            RepositionMessage& repos = (RepositionMessage&)message;
            if (repos.XChanged && this->m_PreviousX == this->X)
                this->m_PreviousX = this->X = repos.X;
            if (repos.YChanged && this->m_PreviousY == this->Y)
                this->m_PreviousY = this->Y = repos.Y;
            if (repos.ZChanged && this->m_PreviousZ == this->Z)
                this->m_PreviousZ = this->Z = repos.Z;

            // If we are a server, send the message to all clients.
            if (this->GetController().IsServer())
                this->SendMessage(repos);
        }
    }
}