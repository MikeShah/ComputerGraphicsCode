#include <SDL2/SDL.h>
#include <iostream>

int main(){

    if(SDL_Init(SDL_INIT_VIDEO) !=0){
        std::cout << "sdl video initialized!\n";
    }

    SDL_Window* window;
    window = SDL_CreateWindow("title",0,0,640,480,SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Need to add in glad libraries.
//    std::cout << glGetString(GL_VENDOR) << std::endl;
//    std::cout << glGetString(GL_RENDERER) << std::endl;
//    std::cout << glGetString(GL_VERSION) << std::endl;

    if(nullptr==window){
        std::cout << "Could not create window!?\n";
    }

    int i=0;
    // Artificial loop here to run for a bit,
    // while showing how our SDL Graphics applications loop
    while(i < 1000000){
        SDL_Event e;
        while(SDL_PollEvent(&e) !=0){
            // handle keyboard/mouse/etc.
        }
        std::cout << "i < " << i << std::endl;
        ++i;

        // Updates our window of our OpenGL application
        SDL_GL_SwapWindow(window);
    }


    // Delay our application 5000 milliseconds
    // (5 seconds) so we can see our window.
    //    SDL_Delay(5000);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
