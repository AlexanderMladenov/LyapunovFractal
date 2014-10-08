#include <string>
#include <vector>
#include <random>
#include <array>
#include <math.h>
#include <sstream>
#include <iostream>
#include <thread>
#include <SDL.h>
#include <glm/glm.hpp>

static const char* const VERSION = "1.0.1\0";

#define FRAME_RES 512
#define ITERATIONS 2000
std::string fractString("AABAB");
glm::vec3 FrameBuffer[FRAME_RES][FRAME_RES];
SDL_Window* m_Window = nullptr;
SDL_Surface* m_Surface = nullptr;

template <typename T = float>
auto clamp(T x, T a, T b) -> T
{
    return x <= a ? a : x >= b ? b : x;
}

inline std::uint32_t ConvertPixel(const glm::vec3& pixel)
{
    int rs = m_Surface->format->Rshift;
    int gs = m_Surface->format->Gshift;
    int bs = m_Surface->format->Bshift;

    std::uint8_t r, g, b;
    r = ::clamp(pixel.r, 0.f, 1.f) * 255;
    g = ::clamp(pixel.g, 0.f, 1.f) * 255;
    b = ::clamp(pixel.b, 0.f, 1.f) * 255;

    return (b << bs) | (g << gs) | (r << rs);
}

using namespace glm;

void SwapBuffers(const vec3 frameBuf[FRAME_RES][FRAME_RES])
{
    for (int y = 0; y < FRAME_RES; y++)
    {
        Uint32 *row = (Uint32*) ((Uint8*) m_Surface->pixels + y * m_Surface->pitch);
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
            exit(0);
        }
    }
}

float r(int n, const vec2& point)
{
    auto& a = point.x;
    auto& b = point.y;
    int Sn = n % fractString.size();
    auto& x = fractString[Sn];
    return x == 'A' ? a : b;
}

float computeLyapunovExponent(const vec2& point)
{
    auto& p = point;

    float iter[ITERATIONS];
    iter[0] = 0.5f;

    for (int i = 1; i < ITERATIONS; i++)
    {
        iter[i] = r(i - 1, p) * iter[i - 1] * (1 - iter[i - 1]);
    }

    float result = 0.f;
    float ri[ITERATIONS];
    for (auto i = 1; i < ITERATIONS; i++)
    {
        ri[i] = abs(r(i, p));
    }

    float oneMinus2iterations[ITERATIONS];
    for (auto i = 1; i < ITERATIONS; i++)
    {
        oneMinus2iterations[i] = abs((1 - (2 * iter[i])));
    }

    for (int i = 1; i < ITERATIONS; i++)
    {
        result += logf(ri[i] * oneMinus2iterations[i]);
    }

    return (result / (float) ITERATIONS);
}

void renderFractalPixel(int x, int y)
{
    float a = (float) x / (float) (FRAME_RES);
    a = 2.f + a * 2.f;

    float b = (float) y / (float) (FRAME_RES);
    b = 2.f + b * 2.f;
    
    const vec2 p(a, b);
    auto lyapunovExp = computeLyapunovExponent(p);
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

typedef  std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;
template <typename T = std::chrono::seconds>
inline long long timePast(const TimePoint& start, const TimePoint& end)
{
    return std::chrono::duration_cast<T>(end - start).count();
}

#undef main
int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        fractString = argv[1];
    }

    std::cout << "Lyapunov Fractal Renderer. Version: " << VERSION << std::endl;
    auto begin = std::chrono::high_resolution_clock::now();
    if (SDL_Init(SDL_INIT_EVERYTHING != 0))
    {
        std::cout << "SDL_init failed: " << SDL_GetError() << std::endl;
        waitForUserExit();
        return -1;
    }

    m_Window = SDL_CreateWindow("LyapunovFract", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                FRAME_RES, FRAME_RES, SDL_WINDOW_OPENGL);

    if (!m_Window)
    {
        std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        waitForUserExit();
        return -2;
    }

    m_Surface = SDL_GetWindowSurface(m_Window);

    if (!m_Surface)
    {
        std::cout << "SDL_GetWindowSurface failed: " << SDL_GetError() << std::endl;
        waitForUserExit();
        return -3;
    }
    SwapBuffers(FrameBuffer);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto timeSec = timePast(begin, end);
    auto timeMili = timePast<std::chrono::milliseconds>(begin, end);
    std::cout << "Seting up SDL took: " << timeSec << " sec " << timeMili - (timeSec * 1000) << " milisec" << std::endl;

    std::cout << "Starting rendering with: " << std::thread::hardware_concurrency() << " threads." << std::endl;
    begin = std::chrono::high_resolution_clock::now();

    DoFractalThreaded();

    for (auto&& th : threads)
    {
        th.join();
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Rendering finished!" << std::endl;
    timeSec = timePast(begin, end);
    timeMili = timePast<std::chrono::milliseconds>(begin, end);

    std::stringstream ss;
    ss << "Time taken to render Lyapunov Fractal: " << timeSec << " sec " << timeMili - (timeSec * 1000) << " milisec";
    std::cout << "Time taken to render Lyapunov Fractal: " << timeSec << " sec " << timeMili - (timeSec * 1000) << " milisec" << std::endl;
    std::cout << "Fractal string used to generate image: " << fractString << ". With " << ITERATIONS << " iterations per pixel" << std:: endl;

    SDL_SetWindowTitle(m_Window, ss.str().c_str());
    SwapBuffers(FrameBuffer);
    waitForUserExit();
    return 0;
}
