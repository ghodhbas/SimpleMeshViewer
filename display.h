#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <string>
#include "transform.h"
#include "camera.h"


class Display
{
public:
    Display(int witdth, int height, const std::string& title);

    void Clear(float r, float g, float b, float a);
    void Update(Camera& camera);
    bool IsClosed();

    virtual ~Display();

    float pitch = 0.f;
    float yaw = 0.f;
    float sensitivity = 0.1f;

protected:

private:
    Display(const Display& other) {}
    void operator=(const Display& other) {}

    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    bool m_isClosed;
};

#endif // DISPLAY_H
