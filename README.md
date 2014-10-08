LyapunovFractal
===============

What is this?
=============
This is a small multithreaded application that renders a Lyapunov Fractal. You can read more about the fractals [HERE](http://en.wikipedia.org/wiki/Lyapunov_fractal)
- Add command line arguments for iterations count, range and resolution. Currently the default fractal is "AABAB", in [2,4]x[2,4] with 750 iteration

Current Features
=============
- Screenshot [HERE](http://i.imgur.com/p6vVDg2.png)
- Multithreaded rendering.
- Some usage of SoA in order to befit SSE2/AVX vector instructioins in loops
- Render time of a 1024x1024 with 750 iterations per pixel is less than 4 sec on 8 threads.
- Fractal string can be passed as a command line argument in the form of, for example: -AABAABB.
- Lyapunov Fractal! Freshly rendered for you! No one?

Current issues/bugs/TODO list
=============
(Upcoming soon)
- Currently the threads and regions are created by hardware_concurrency() which may cause issues if the threads are not 2, 4 or 8;
- Make coloring user picked
- Add a timed SwapBuffers to show output.

How to build
=============
Requirements:
- MS Visual Studio 2013
- [SDL2 2.0.3](https://www.libsdl.org/index.php) (included in libs)
- [GLM 0.9.5.4](http://glm.g-truc.net/0.9.5/index.html) (included in libs)

External dependencies are GLM and SDL2 which are included in libs folder.
Open the solution and build* the project.
When the build finishes copy the SDL2.dll from the libs/SDL/libs/x64 to the folder x64/Release.

* There are 2 Release modes SSE2 and AVX one. If the application crashes in the avx mode, try the SSE2 one, it should work just fine.

Licence Information
=============
MIT Licence except for external dependencies, which use their own licensing.