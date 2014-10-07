#include <stdio.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <SDL.h>
#include <glm/glm.hpp>

const int FRAME_RES = 1024;
std::string fractString("AABAB");
glm::vec3 FrameBuffer[FRAME_RES][FRAME_RES];
std::mt19937 m_Gen(9874651);
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

void SwapBuffers(const vec3 frameBuf[FRAME_RES][FRAME_RES])
{
    for (int y = 0; y < FRAME_RES; y++)
    {
        Uint32 *row = (Uint32*)((Uint8*)m_Surface->pixels + y * m_Surface->pitch);
        for (int x = 0; x < FRAME_RES; x++)
            row[x] = ConvertPixel(frameBuf[y][x]);
    }
    SDL_UpdateWindowSurface(m_Window);
}

void waitForUserExit()
{
    SDL_Event e;
    bool wantExit = false;
    while (!wantExit)
    {
        SDL_PollEvent(&e);

        if (e.type == SDL_QUIT)
        {
            wantExit = true;
        }
    }
}

float r(int n, const vec2& chosenPoint)
{

    float a = chosenPoint.x;
    float b = chosenPoint.y;

    int Sn = n % fractString.size();
    auto x = fractString[Sn];
    if (x == 'A')
    {
        return a;
    }
    else
    {
        return b;
    }
}

template <typename T = float>
auto clamp(T x, T a, T b) -> T
{
    return x > a ? a : x < b ? b : x;
}

#define ITERATIONS 1000
std::vector<float> precomtuteIterations(const vec2& cPoint)
{
    std::vector<float> result;
    result.resize(ITERATIONS);
    result[0] = 0.5f;

    for (int i = 1; i < ITERATIONS; i++)
    {
        result[i] = r(i - 1, cPoint) * result[i - 1] * (1 - result[i - 1]);
    }

    return result;
}

float computeLyapunovExponent(const std::vector<float>& iterations, const vec2& cPoint)
{
    float result = 0.f;

    for (int i = 1; i < ITERATIONS; i++)
    {
        auto a = (2 * iterations[i]);
        auto ri = r(i, cPoint);
        result += logf(abs(ri * (1 - a)));
    }

    return (result / (float)ITERATIONS);
}

void renderFractalPixel(int x, int y)
{
    float a = (float)x / (float)(FRAME_RES);
    a = 2.f + a * 2.f;

    float b = (float)y / (float)(FRAME_RES);
    b = 2.f + b * 2.f;

    auto point = vec2(a, b);
    auto iterations = precomtuteIterations(point);
    auto lyapunovExp = computeLyapunovExponent(iterations, point);
    if (lyapunovExp < 0)
    {
        FrameBuffer[x][y] = vec3(abs(lyapunovExp), abs(lyapunovExp), 0);
    }
    else if (lyapunovExp == 0.f)
    {
        FrameBuffer[x][y] = vec3(1, 1, 0);
    }
    else
    {
        FrameBuffer[x][y] = vec3(0, lyapunovExp * 0.5, lyapunovExp);
    }
}

void renderFractalRegion(int xFrom, int xTo)
{
    for (auto x = xFrom; x < xTo; x++)
    {
        for (auto y = 0; y < FRAME_RES; y++)
        {
            renderFractalPixel(x, y);
        }
        SwapBuffers(FrameBuffer);
    }
    return;
}

std::vector<std::thread> threads;
void DoFractalThreaded()
{
    unsigned int numThreads = std::thread::hardware_concurrency();
    int delta = FRAME_RES / numThreads;
    for (int i = 0; i < numThreads; i++)
    {
        auto fun = std::bind(&renderFractalRegion, delta * i, delta * i + delta);
        threads.emplace_back(fun);
    }
}
#undef main
int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING != 0))
    {
        std::cout << "SDL_init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    m_Window = SDL_CreateWindow("LyapunovFract", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        FRAME_RES, FRAME_RES, SDL_WINDOW_OPENGL);

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

    DoFractalThreaded();
    
    /*for (int y = FRAME_RES; y > 0; y--)
    {
        for (int x = 0; x < FRAME_RES; x++)
        {
            renderFractalPixel(x, y);
        }
        SwapBuffers(FrameBuffer);
    }*/
    for (auto&& th : threads)
    {
        th.join();
    }
    SwapBuffers(FrameBuffer);
    waitForUserExit();
    return 0;
}
