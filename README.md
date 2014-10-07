LyapunovFractal
===============

What is this?
=============
This is a small multithreaded application that renders a Lyapunov Fractal. You can read more about the fractals [HERE](http://en.wikipedia.org/wiki/Lyapunov_fractal)


Current Features
=============
- Rendertime of a 1024x1024 with 750 iterations per pixel is less than 4 sec on 8 threads;
- Lyapunov Fractal! Freshly rendered for you! No one?

Current issues/bugs/TODO list
=============
(Upcoming soon)
- Currently the threads and regions are created by hardware_concurrency() which may cause races if the threads are not 4 or 8;
- Add command line arguments for Lyapunov string, iterations count, range and resolution. Currently the default hardcoded ones are "AABAB", in [2,4]x[2,4] with 750 
  iterations on a 1024x1024 resolution.
- Make coloring user picked
- Add a timed SwapBuffers to show output.