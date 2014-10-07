LyapunovFractal
===============

What is this?
=============
This is a small multithreaded application that renders a Lyapunov Fractal. You can read more about the fractals [HERE](http://en.wikipedia.org/wiki/Lyapunov_fractal)

Current Features
=============
- Render time of a 1024x1024 with 750 iterations per pixel is less than 4 sec on 8 threads;
- Lyapunov Fractal! Freshly rendered for you! No one?

Current issues/bugs/TODO list
=============
(Upcoming soon)
- Currently the threads and regions are created by hardware_concurrency() which may cause races if the threads are not 4 or 8;
- Add command line arguments for Lyapunov string, iterations count, range and resolution. Currently the default hardcoded ones are "AABAB", in [2,4]x[2,4] with 750 
  iterations on a 1024x1024 resolution.
- Make coloring user picked
- Add a timed SwapBuffers to show output.

How to build
=============
Requirements:
- MS Visual Studio 2013
- [SDL2 2.0.3](https://www.libsdl.org/index.php) (included in libs)
- [GLM 0.9.5.4](http://glm.g-truc.net/0.9.5/index.html) (included in libs)

External dependencies are GLM and SDL2 which are included in libs folder. 
You should just open the solution file and run in release mode.

Licence Information
=============
MIT Licence except for external dependencies, which use their own licensing.