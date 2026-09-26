#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Editor/Editor.h"

static const char* WINDOW_TITLE = "BigBoxEngine"; //temporary values, overwritten by engine.json
static const int   WINDOW_WIDTH  = 1280;
static const int   WINDOW_HEIGHT = 720;
static const char* GLSL_VERSION  = "#version 460";

int main(int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Error en SDL_Init: %s", SDL_GetError());
        return -1;
    }
    SDL_Log("SDL inicializado");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (window == nullptr)
    {
        SDL_Log("Error creando la ventana: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr)
    {
        SDL_Log("Error creando el contexto OpenGL: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        SDL_Log("Error inicializando GLAD");
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    SDL_Log("OpenGL %s (%s)", (const char*)glGetString(GL_VERSION),
                              (const char*)glGetString(GL_RENDERER));

    int pixelW = 0, pixelH = 0;
    SDL_GetWindowSizeInPixels(window, &pixelW, &pixelH);
    glViewport(0, 0, pixelW, pixelH);

    Editor editor;
    if (!editor.Init(window, glContext, GLSL_VERSION))
    {
        SDL_Log("Error inicializando el editor");
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            editor.ProcessEvent(event);

            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                if (event.window.windowID == SDL_GetWindowID(window))
                    running = false;
                break;
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                glViewport(0, 0, event.window.data1, event.window.data2);
                break;
            default:
                break;
            }
        }

        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        editor.Update();
        if (editor.WantsToQuit())
			running = false; //we call editor.WantsToQuit() after Update() because the user may have clicked "Quit" in the menu, doing it this way allows the editor to process the event and show the confirmation dialog before quitting

        glClearColor(0.11f, 0.20f, 0.20f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        editor.Render();
        SDL_GL_SwapWindow(window);
    }

    editor.Shutdown();
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
