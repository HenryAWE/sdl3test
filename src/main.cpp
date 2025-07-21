#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <iostream>

static void on_draw(SDL_Window* win, SDL_GPUDevice* dev)
{
    SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(dev);
    if(cmdbuf == nullptr)
    {
        SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
        return;
    }

    SDL_GPUTexture* swapchain_tex;
    if(!SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, win, &swapchain_tex, NULL, NULL))
    {
        SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
        return;
    }

    if(swapchain_tex != nullptr)
    {
        SDL_GPUColorTargetInfo color = {0};
        color.texture = swapchain_tex;
        color.clear_color = SDL_FColor{0.0f, 0.5f, 1.0f, 1.0f};
        color.load_op = SDL_GPU_LOADOP_CLEAR;
        color.store_op = SDL_GPU_STOREOP_STORE;

        SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(cmdbuf, &color, 1, NULL);
        if(!render_pass)
            std::cerr << "failed to get render pass" << std::endl;
        SDL_EndGPURenderPass(render_pass);
    }

    SDL_SubmitGPUCommandBuffer(cmdbuf);
}

int main()
{
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        return 1;
    }

    SDL_GPUDevice* dev = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV, false, nullptr
    );
    auto* win = SDL_CreateWindow(
        "测试", 1280, 760, 0
    );

    if(!dev)
        std::cerr << SDL_GetError() << std::endl;
    if(!SDL_ClaimWindowForGPUDevice(dev, win))
        std::cerr << SDL_GetError() << std::endl;
    std::cerr << "GPU driver: " << SDL_GetGPUDeviceDriver(dev) << std::endl;

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

        on_draw(win, dev);

        SDL_Delay(10);
    }

    SDL_DestroyWindow(win);
    SDL_DestroyGPUDevice(dev);
    dev = nullptr;

    SDL_Quit();
    return 0;
}
