#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

int main()
{
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        return 1;
    }

    auto* win = SDL_CreateWindow(
        "测试", 1280, 760, 0
    );

    bool quit = false;

    while(!quit)
    {
        SDL_Event e{};

        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
            case SDL_EVENT_QUIT:
                quit = true;
                break;

            default:
                break;
            }
        }

        SDL_Delay(10);
    }

    SDL_DestroyWindow(win);

    SDL_Quit();
    return 0;
}
