#include "display.h"
#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include "transform.h"


/* Constructor and init of display*/
Display::Display(int width, int height,const std::string& title)
{
    //init windowing
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);


    //create window and opengl context
    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    m_glContext = SDL_GL_CreateContext(m_window);


    GLenum status = glewInit();
    //test init
    if( status != GLEW_OK)
    {
        std::cerr << "Glew Failed to Initialize" << std::endl;
    }



    m_isClosed = false;

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

}

/*Display Destructor*/
Display::~Display()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

/*Check of display is closed*/
bool Display::IsClosed()
{
    return m_isClosed;
}

/*fill screen with specified color*/
void Display::Clear(float r, float g, float b, float a)
{
     glClearColor(r,g,b,a);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


/*UPDATE*/
void Display::Update( Camera& camera)
{
    SDL_GL_SwapWindow(m_window);

    SDL_SetRelativeMouseMode(SDL_TRUE);
    // make sure that we tell the os that we are recieving and responding
    SDL_Event e;

    while( SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        //close window
        case SDL_QUIT:
            m_isClosed = true;
            break;

        //keyboard pressed
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
            case SDLK_a: camera.setPosition(camera.getPostion() - glm::normalize(glm::cross(camera.getForward(), camera.getUpward())) * 0.3f); break;
            case SDLK_d: camera.setPosition(camera.getPostion() + glm::normalize(glm::cross(camera.getForward(), camera.getUpward())) * 0.3f); break;
            case SDLK_w:  camera.setPosition(camera.getPostion() + 0.3f * camera.getForward()); break;
            case SDLK_s:  camera.setPosition(camera.getPostion() - 0.3f * camera.getForward()); break;
            }
             break;

        //controle canera with mouse
        case SDL_MOUSEMOTION:
            float xoff;
            float yoff;

            xoff = e.motion.xrel;
            yoff = -e.motion.yrel;
            //yrot
            xoff *= sensitivity;
            yoff *= sensitivity;
            //limot pitch and yaw
            yaw   += xoff;
            pitch += yoff;
            if(pitch > 89.0f)
              pitch =  89.0f;
            if(pitch < -89.0f)
              pitch = -89.0f;
            //update front vector of camera
            glm::vec3 front;
            front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
            front.y = sin(glm::radians(pitch));
            front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
            camera.setForward(glm::normalize(front));


        }


    }

}
