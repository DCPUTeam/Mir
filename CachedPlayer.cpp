/*
 * The class definition for a CachedPlayer.
 */

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <GL/glut.h>
#include "CachedPlayer.h"

void CachedPlayer::ReceiveMessage(Network::Message& message)
{
    // Handle messages.
    //std::cout << "NOTE: CachedPlayer just received a message!" << std::endl;

    Network::SpatialObject::ReceiveMessage(message);
}

void CachedPlayer::Update()
{
}

void CachedPlayer::Render()
{
    glColor3f(1, 1, 1);
    //glScalef(0.2, 0.2, 0.2);
    //glutSolidCube(1.0);
    //glTranslatef(0.0, 1.75, 0.0);
    //glScalef(0.5, 0.5, 0.5);
    glutSolidCube(1.0);
}