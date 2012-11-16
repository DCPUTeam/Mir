/*
 * The class definition for a CachedUniverse.
 */

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <GL/glut.h>
#include "CachedUniverse.h"
#include <Messages/Declarations.h>
#include "Network/Controller.h"

void CachedUniverse::ReceiveMessage(Network::Message& message)
{
    // Handle messages.
    if (typeid(message) == typeid(Network::CreateMessage))
    {
        /*Network::CreateMessage& create = (Network::CreateMessage&)message;
        Network::RequestState& request = this->m_Controller->Request(create.Identifier);
        if (request.Status != Network::REQUEST_STATUS_AVAILABLE)
            return; // No idea what went wrong here...
        if (create.Type == "Ship")
            this->m_CachedShips.insert(this->m_CachedShips.end(), (CachedShip*)request.Reference);
        else if (create.Type == "Actor")
            this->m_CachedActors.insert(this->m_CachedActors.end(), (CachedActor*)request.Reference);
        else if (create.Type == "Planet" / || ... *)
            this->m_CachedCelestialBodies.insert(this->m_CachedCelestialBodies.end(), (CachedCelestialBody*)request.Reference);
        else
        {
            std::cout << "Universe not taking reference to '" << create.Type << "'." << std::endl;
        }*/
    }
    std::cout << "NOTE: CachedUniverse just received a message!" << std::endl;
}

void CachedUniverse::Update()
{
    /*for (std::list<CachedCelestialBody*>::iterator i = this->m_CachedCelestialBodies.begin();
        i != this->m_CachedCelestialBodies.end(); i++)
        (*i)->Update();
    for (std::list<CachedActor*>::iterator i = this->m_CachedActors.begin();
        i != this->m_CachedActors.end(); i++)
        (*i)->Update();
    for (std::list<CachedShip*>::iterator i = this->m_CachedShips.begin();
        i != this->m_CachedShips.end(); i++)
        (*i)->Update();*/
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
    /*for (std::list<CachedCelestialBody*>::iterator i = this->m_CachedCelestialBodies.begin();
        i != this->m_CachedCelestialBodies.end(); i++)
        (*i)->Render();
    for (std::list<CachedActor*>::iterator i = this->m_CachedActors.begin();
        i != this->m_CachedActors.end(); i++)
        (*i)->Render();
    for (std::list<CachedShip*>::iterator i = this->m_CachedShips.begin();
        i != this->m_CachedShips.end(); i++)
        (*i)->Render();*/
}
