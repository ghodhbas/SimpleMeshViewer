#include <iostream>
#include <string>
#include <GL/glew.h>
#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include <GL/glut.h>


#define WIDTH 1200
#define HEIGHT 900

using namespace std;



int main()
{
    //make window
    Display display(WIDTH, HEIGHT, "Mesh Viewer");

    //construct the mesh
    Mesh mesh2("./res/sphere.obj");

    //access the shader code that will be sent to the gpu
    Shader shader("./res/basicShader");
    //construct the texture
    Texture texture("./res/rock3.jpg");
    //camera setup
    Camera camera(glm::vec3(0,0,-7),70.0f, (float)WIDTH/(float)HEIGHT, 0.01f,1000.0f); //-7 for zoomout

    //construct the transformation matrix on camera
    Transform transform;

    //FPS COUNTER
    unsigned int startclock = 0;
    unsigned int deltaclock = 0;
    unsigned int currentFPS = 0;
    startclock = SDL_GetTicks();
    unsigned int iteration = 0;
    float counter =0.f;

    while(!display.IsClosed())
    {
        //MEASURE FPS
        deltaclock = SDL_GetTicks() - startclock;
        startclock = SDL_GetTicks();
        if ( deltaclock != 0)
            currentFPS = 1000 / deltaclock;
        if(iteration > 20)
        {
            std::cout << "FPS: " << std::to_string(currentFPS) << std::endl;
            iteration = 0;
        }
        iteration++;


        //make background
        display.Clear(0.0f,0.15f,0.3f,1.0f);

        //grid
        GLfloat fExtent = 20.0f;
        GLfloat fStep = 1.0f;
        GLfloat y = 0.f;
        GLint iLine;
        glBegin(GL_LINES);
        for(iLine = -fExtent; iLine <= fExtent; iLine += fStep)
        {
        glVertex3f(iLine, y, fExtent); // Draw Z lines
        glVertex3f(iLine, y, -fExtent);
        glVertex3f(fExtent, y, iLine);
        glVertex3f(-fExtent, y, iLine);
        }
        glEnd();


        //apply shader
        shader.Bind();
        //apply texture
        texture.Bind();

        //drawcall
        mesh2.Draw();


        //update display and camera
        display.Update(camera);//checks for closed screen or keyboard events

        //aplly transformatio
        shader.Update(transform, camera);
    }
    return 0;
}

