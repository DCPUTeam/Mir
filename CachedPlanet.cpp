///
/// @brief The implementation of cached planets.
/// @author James Rhodes
///

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <GL/glut.h>
#include "CachedPlanet.h"

void CachedPlanet::ReceiveMessage(Network::Message& message)
{
    Network::SpatialObject::ReceiveMessage(message);
}

void CachedPlanet::Update()
{
}

void CachedPlanet::Render()
{
    glColor3f(1, 1, 1);
    //glScalef(0.2, 0.2, 0.2);
    //glutSolidCube(1.0);
    //glTranslatef(0.0, 1.75, 0.0);
    //glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(20.0, 8, 8);
}