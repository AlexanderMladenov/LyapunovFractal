#include <stdio.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <SDL.h>
#include <glm/glm.hpp>

std::string fractString("AABAB");
glm::vec3 FrameBuffer[1024][1024];
std::mt19937 m_Gen(5892753628915);
SDL_Window* m_Window = nullptr;
SDL_Surface* m_Surface = nullptr;

using namespace glm;

inline std::uint32_t ConvertPixel(const glm::vec3& pixel)
{
    int rs = m_Surface->format->Rshift;
    int gs = m_Surface->format->Gshift;
    int bs = m_Surface->format->Bshift;

    std::uint8_t r, g, b;
    if (pixel.r > 1.f)
        r = 255;

    else if (pixel.r <= 0.f)
        r = 0;
    else
        r = pixel.r * 255;

    if (pixel.g > 1.f)
        g = 255;

    else if (pixel.g <= 0.f)
        g = 0;
    else
        g = pixel.g * 255;

    if (pixel.b > 1.f)
        b = 255;

    else if (pixel.b <= 0.f)
        b = 0;
    else
        b = pixel.b * 255;

    return (b << bs) | (g << gs) | (r << rs);
}

void SwapBuffers(const vec3 frameBuf[1024][1024])
{
    for (int y = 0; y < 1024; y++)
    {
        Uint32 *row = (Uint32*)((Uint8*)m_Surface->pixels + y * m_Surface->pitch);
        for (int x = 0; x < 1024; x++)
            row[x] = ConvertPixel(frameBuf[y][x]);
    }
    SDL_UpdateWindowSurface(m_Window);
}

void waitForUserExit()
{
    SDL_Event e;
    bool wantToQuit = false;
    while (!wantToQuit)
    {
        SDL_PollEvent(&e);

        if (e.type == SDL_QUIT)
        {
            wantToQuit = true;
        }
    }
}

float r(int n)
{
    std::uniform_real_distribution<float> distributionA(2, 4);
    std::uniform_real_distribution<float> distributionB(2, 4);

    float a = distributionA(m_Gen);
    float b = distributionB(m_Gen);

    int Sn = n % fractString.size();
    if (fractString[Sn] == 'A')
    {
        return a;
    }
    else
    {
        return b;
    }
    return 0.f;

}

#define ITERATIONS 10000
std::vector<float> precomtuteIterations()
{
    std::vector<float> result;
    result.resize(ITERATIONS);
    result[0] = 0.5f;
    for (int i = 1; i < ITERATIONS; i++)
    {
        result[i] = r(i) * result[i - 1] * ( 1 - result[i - 1]);
    }
    return result;
}

float computeLyapunovExponent(const std::vector<float>& iterations)
{
    float result = 0.f;

    for (int i = 1; i < ITERATIONS; i++)
    {
        auto a = (2 * iterations[i]);
        auto oneminusa = 1 - a;
        auto ri = r(i);

        result += logf(abs(ri * (oneminusa)));
    }

    return result;
}
#undef main
int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING != 0))
    {
        std::cout << "SDL_init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    m_Window = SDL_CreateWindow("Sword", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1024, 1024, SDL_WINDOW_OPENGL);

    if (!m_Window)
    {
        std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return false;
    }

    m_Surface = SDL_GetWindowSurface(m_Window);

    if (!m_Surface)
    {
        std::cout << "SDL_GetWindowSurface failed: " << SDL_GetError() << std::endl;
        return false;
    }

    auto iterations = precomtuteIterations();
    auto lyapunovExp = computeLyapunovExponent(iterations);

    waitForUserExit();
    SwapBuffers(FrameBuffer);
    return 0;
}


















