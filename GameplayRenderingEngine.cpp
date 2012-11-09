/*
 * The class definition for the RenderingEngine.
 */

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <GL/glut.h>
extern "C"
{
#include <GL/GLU.h>
}
#include "GameplayRenderingEngine.h"

#ifdef OLD

/*
 * Example code taken from http://content.gpwiki.org/index.php/GLFW:Tutorials:Basics#An_Example
 * just so we know we can draw things to the screen.
 */

float rotate_y = 0,
      rotate_z = 0;
const float rotations_per_tick = .2f;

void Draw_Square(float red, float green, float blue)
{
    // Draws a square with a gradient color at coordinates 0, 10
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    {
        glNormal3f(0.0, 0.0, 1.0);
        glColor3f(red, green, blue);
        glTexCoord2d(0.05, 0.05);
        glVertex2i(1, 11);
        glColor3f(red * .8, green * .8, blue * .8);
        glTexCoord2d(0.05, 0);
        glVertex2i(-1, 11);
        glColor3f(red * .5, green * .5, blue * .5);
        glTexCoord2d(0, 0);
        glVertex2i(-1, 9);
        glColor3f(red * .8, green * .8, blue * .8);
        glTexCoord2d(0, 0.05);
        glVertex2i(1, 9);
    }
    glDisable(GL_TEXTURE_2D);
    glPopAttrib();
    glEnd();
}

void Draw(void)
{
    // reset view matrix
    glLoadIdentity();
    // move view back a bit
    glTranslatef(0, 0, -30);
    // apply the current rotation
    glRotatef(rotate_y, 0, 1, 0);
    glRotatef(rotate_z, 0, 0, 1);
    // by repeatedly rotating the view matrix during drawing, the
    // squares end up in a circle
    int i = 0, squares = 15;
    float red = 0, blue = 1;
    for (; i < squares; ++i)
    {
        glRotatef(360.0 / squares, 0, 0, 1);
        // colors change for each square
        red += 1.0f / 12;
        blue -= 1.0f / 12;
        Draw_Square(red, .6f, blue);
    }
}

void GameplayRenderingEngine::Render(CachedPlayer& player, CachedUniverse& universe)
{
    // set the projection matrix to a normal frustum with a max depth of 50
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect_ratio = ((float)600) / 800;
    glFrustum(.5, -.5, -.5 * aspect_ratio, .5 * aspect_ratio, 1, 50);
    glMatrixMode(GL_MODELVIEW);

    // the time of the previous frame
    static double old_time = glfwGetTime();
    // calculate time elapsed, and the amount by which stuff rotates
    double current_time = glfwGetTime(),
           delta_rotate = (current_time - old_time) * rotations_per_tick * 360;
    old_time = current_time;
    // z axis always rotates
    rotate_z = (float)player.Z;
    //rotate_z += delta_rotate;

    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw the figure
    Draw();
}

#endif

void GameplayRenderingEngine::Init()
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
}

void GameplayRenderingEngine::Deinit()
{
}

void GameplayRenderingEngine::Render(CachedPlayer& player, CachedUniverse& universe)
{
    // Set up the projection matrix.
    glViewport(0, 0, (GLsizei)800, (GLsizei)600);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2, 2, -2, 2, 1.5, 50);
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the model viewing matrix.
    glLoadIdentity();
    gluLookAt(
        player.X + cos(player.CameraRotation / 180.f * (2 * 3.14f)) * 5.3,
        player.Y + sqrt(pow(player.CameraDistance, 2) * 2),
        player.Z + sin(player.CameraRotation / 180.f * (2 * 3.14f)) * 5.3,
        player.X,
        player.Y,
        player.Z,
        0,
        1,
        0);

    // Push matrix to maintain it while drawing things.
    glPushMatrix();

    // Draw universe.
    //universe.Render();

    // Draw player.
    //player.Render();
    //glTranslatef(-player.X, -player.Y, -player.Z);
    //glRotatef(3.14f / 4.f, 0, 0, 1);
    //glRotatef(3.14f / 4.f, 0, 1, 0);
    std::cout << "Rendering at " << player.X << ", " << player.Y << ", " << player.Z << "." << std::endl;

    glColor3f(0, 1, 0);
    glPushMatrix();
    glTranslatef(player.X, player.Y, player.Z);
    glutSolidCube(0.5);
    glPopMatrix();

    glColor3f(1, 0, 0);
    for (int x = -10; x < 10; x++)
        for (int z = -10; z < 10; z++)
        {
            glPushMatrix();
            //glRotatef(player.RotTest, 0, 1, 0);
            glTranslatef(x, 0, z);
            glutSolidCube(0.5);
            glPopMatrix();
        }

    // Finished rendering, pop matrix.
    glPopMatrix();
}