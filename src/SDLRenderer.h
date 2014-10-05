#ifndef __SDL_RENDERER_H
#define __SDL_RENDERER_H

//class SDLRenderer
//{
//public:
//    SDLRenderer(int widht, int height)
//    {
//        m_Window = SDL_CreateWindow("Sword", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
//            (fullscreen == true) ? SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL : SDL_WINDOW_OPENGL);
//
//        if (!m_Window)
//        {
//            std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
//            return false;
//        }
//
//        m_Surface = SDL_GetWindowSurface(m_Window);
//
//        if (!m_Surface)
//        {
//            std::cout << "SDL_GetWindowSurface failed: " << SDL_GetError() << std::endl;
//            return false;
//        }
//
//    };
//    
//    SDL_Window* m_Window = nullptr;
//    SDL_Renderer* m_Renderer = nullptr;
//};
#endif //__SDL_RENDERER_


        //}

        //inline unsigned Renderer::convertTo8bit_sRGB(float x)
        //{
        //    const float a = 0.055f;

        //    // sRGB transform:
        //    if (x <= 0.0031308f)
        //        x = x * 12.02f;
        //    else
        //        x = (1.0f + a) * powf(x, 1.0f / 2.4f) - a;
        //    return (int)floor((x * 255.0f) + 0.5f);
        //}

        //inline std::uint32_t Renderer::ConvertPixel(const glm::vec3& pixel)
        //{
        //    int rs = m_Surface->format->Rshift;
        //    int gs = m_Surface->format->Gshift;
        //    int bs = m_Surface->format->Bshift;

        //    std::uint8_t r, g, b;
        //    if (pixel.r > 1.f)
        //        r = 255;

        //    else if (pixel.r <= 0.f)
        //        r = 0;
        //    else
        //        r = m_srgbCache[int(pixel.r * 4096.0f)];

        //    if (pixel.g > 1.f)
        //        g = 255;

        //    else if (pixel.g <= 0.f)
        //        g = 0;
        //    else
        //        g = m_srgbCache[int(pixel.g * 4096.0f)];

        //    if (pixel.b > 1.f)
        //        b = 255;

        //    else if (pixel.b <= 0.f)
        //        b = 0;
        //    else
        //        b = m_srgbCache[int(pixel.b * 4096.0f)];

        //    return (b << bs) | (g << gs) | (r << rs);
        //}

        //void Renderer::SwapBuffers(const std::vector<std::vector<vec3>>& frameBuf)
        //{
        //    for (int y = 0; y < FRAME_HEIGHT; y++)
        //    {
        //        Uint32 *row = (Uint32*)((Uint8*)m_Surface->pixels + y * m_Surface->pitch);
        //        for (int x = 0; x < FRAME_WIDTH; x++)
        //            row[x] = ConvertPixel(frameBuf[y][x]);
        //    }
        //    SDL_UpdateWindowSurface(m_Window);
        //}H