/*
 * The class definition for a CachedUniverse.
 */

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <GL/glut.h>
#include "CachedUniverse.h"
#include <Messages/Declarations.h>
#include "Network/Controller.h"

CachedUniverse::CachedUniverse(Network::Controller& controller, std::string identifier)
    : Network::IdentifiableObject(controller, identifier), m_SphereGenerated(false) 
{
    // If this is a server, no need to do any client stuff so return.
    if (this->GetController().IsServer())
        return;
    
    // First try to initiate a request to get the player (this is also done in the
    // gameplay state).
    CachedPlayer* player = this->GetPlayer();
    this->m_SendLocalizedRequests = false;
    
    // If we already have the player, then we know their position and we can request
    // all of the objects around them.
    if (player != NULL)
    {
        Network::GetAllLocalObjectsMessage message("", player->X, player->Y, player->Z);
        this->SendMessage(message);
        this->m_SendLocalizedRequests = true;
        std::cout << "Requesting a list of names to request." << std::endl;
    }
}

CachedPlayer* CachedUniverse::GetPlayer()
{
    Network::RequestState& state = this->GetController().Request("player");
    if (state.Status == Network::REQUEST_STATUS_AVAILABLE)
        return (CachedPlayer*)state.Reference;
    else
        return NULL;
}

void CachedUniverse::ReceiveMessage(Network::Message& message)
{
    if (Network::Messages::GetType(message) == Network::Messages::ID_CREATEMESSAGE)
    {
        Network::CreateMessage& create = (Network::CreateMessage&)message;
        Network::RequestState& request = this->m_Controller->Request(create.Identifier);
        if (request.Status != Network::REQUEST_STATUS_AVAILABLE)
            return; // No idea what went wrong here...
        if (create.Type == "Ship")
            this->m_CachedShips.insert(this->m_CachedShips.end(), (CachedShip*)request.Reference);
        else if (create.Type == "Actor")
            this->m_CachedActors.insert(this->m_CachedActors.end(), (CachedActor*)request.Reference);
        else if (create.Type == "Planet" /* || ... */)
            this->m_CachedCelestialBodies.insert(this->m_CachedCelestialBodies.end(), (CachedCelestialBody*)request.Reference);
        else
        {
            std::cout << "Universe not taking reference to '" << create.Type << "'." << std::endl;
        }
    }
    else if (Network::Messages::GetType(message) == Network::Messages::ID_GETALLLOCALOBJECTSMESSAGE && this->GetController().IsServer())
    {
        std::cout << "Server universe asked to provide all of the objects near the player!" << std::endl;
        
        // FIXME: Send only the objects near the position.
        for (std::list<CachedCelestialBody*>::iterator i = this->m_CachedCelestialBodies.begin();
            i != this->m_CachedCelestialBodies.end(); i++)
        {
            Network::LocalObjectNameMessage msg = Network::LocalObjectNameMessage((*i)->GetIdentifier());
            this->SendMessage(msg);
        }
        for (std::list<CachedActor*>::iterator i = this->m_CachedActors.begin();
            i != this->m_CachedActors.end(); i++)
        {
            Network::LocalObjectNameMessage msg = Network::LocalObjectNameMessage((*i)->GetIdentifier());
            this->SendMessage(msg);
        }        
        for (std::list<CachedShip*>::iterator i = this->m_CachedShips.begin();
            i != this->m_CachedShips.end(); i++)
        {
            Network::LocalObjectNameMessage msg = Network::LocalObjectNameMessage((*i)->GetIdentifier());
            this->SendMessage(msg);
        }
    }
    else if (Network::Messages::GetType(message) == Network::Messages::ID_LOCALOBJECTNAMEMESSAGE && !this->GetController().IsServer())
    {
        // We've been told the name of something and now we need to actually request it.
        Network::LocalObjectNameMessage& local = (Network::LocalObjectNameMessage&)message;
        this->GetController().Request(local.Identifier);
    }
    else
        std::cerr << "Cached universe silently dropping message of type '" << Network::Messages::GetType(message) << "'." << std::endl;
}

void CachedUniverse::Update()
{
    // If we have not yet got a list of local objects, do that now.
    if (!this->m_SendLocalizedRequests)
    {
        CachedPlayer* player = this->GetPlayer();
        if (player != NULL)
        {
            Network::GetAllLocalObjectsMessage message("", player->X, player->Y, player->Z);
            this->SendMessage(message);
            this->m_SendLocalizedRequests = true;
            std::cout << "Requesting a list of names to request." << std::endl;
        }
    }
    
    for (std::list<CachedCelestialBody*>::iterator i = this->m_CachedCelestialBodies.begin();
        i != this->m_CachedCelestialBodies.end(); i++)
        (*i)->Update();
    for (std::list<CachedActor*>::iterator i = this->m_CachedActors.begin();
        i != this->m_CachedActors.end(); i++)
        (*i)->Update();
    for (std::list<CachedShip*>::iterator i = this->m_CachedShips.begin();
        i != this->m_CachedShips.end(); i++)
        (*i)->Update();
}

void CachedUniverse::Render()
{
    // Render the universe.
    if (!this->m_SphereGenerated)
    {
        GLUquadricObj* sphere = NULL;
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
        gluQuadricTexture(sphere, true);
        gluQuadricNormals(sphere, GLU_SMOOTH);
        this->m_SphereID = glGenLists(1);
        glNewList(this->m_SphereID, GL_COMPILE);
        gluSphere(sphere, 1.0, 20, 20);
        glEndList();
        gluDeleteQuadric(sphere);
    }
    glCallList(this->m_SphereID);

    // Render entities.
    for (std::list<CachedCelestialBody*>::iterator i = this->m_CachedCelestialBodies.begin();
        i != this->m_CachedCelestialBodies.end(); i++)
        (*i)->Render();
    for (std::list<CachedActor*>::iterator i = this->m_CachedActors.begin();
        i != this->m_CachedActors.end(); i++)
        (*i)->Render();
    for (std::list<CachedShip*>::iterator i = this->m_CachedShips.begin();
        i != this->m_CachedShips.end(); i++)
        (*i)->Render();
}
