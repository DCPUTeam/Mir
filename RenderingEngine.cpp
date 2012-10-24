/*
 * The class definition for the RenderingEngine.
 */

#include <GL/glfw3.h>
#include "RenderingEngine.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/GL.h>
#include <GL/glu.h>

/*
 * Example code taken from http://content.gpwiki.org/index.php/GLFW:Tutorials:Basics#An_Example
 * just so we know we can draw things to the screen.
 */

float rotate_y = 0,
      rotate_z = 0;
const float rotations_per_tick = .2;

void Draw_Square(float red, float green, float blue)
{
    // Draws a square with a gradient color at coordinates 0, 10
    glBegin(GL_QUADS);
    {
        glColor3f(red, green, blue);
        glVertex2i(1, 11);
        glColor3f(red * .8, green * .8, blue * .8);
        glVertex2i(-1, 11);
        glColor3f(red * .5, green * .5, blue * .5);
        glVertex2i(-1, 9);
        glColor3f(red * .8, green * .8, blue * .8);
        glVertex2i(1, 9);
    }
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
    for (; i < squares; ++i){
        glRotatef(360.0/squares, 0, 0, 1);
        // colors change for each square
        red += 1.0/12;
        blue -= 1.0/12;
        Draw_Square(red, .6, blue);
    }
}

void RenderingEngine::Render(CachedPlayer& player, CachedUniverse& universe)
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
    rotate_z += delta_rotate;
 
    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw the figure
    Draw();
}