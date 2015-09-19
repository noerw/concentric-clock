# Concentric Clock

An unconventional representation of a clock.
It shows rotating concentric rings, each showing one unit of time & date.
[Screenshot!](http://github.com/noerw/concentric-clock/blob/master/screenshot.png)

This concept was inspired by mindbrix' ["Concentrichon"](http://www.concentrichron.com/)

Written as an excercise in C++ and SFML.

## dependencies:
* SFML 2.1

## build it!
Currently only building on windows is supported natively.
To do so

* put the [SFML-library (v2.1)](http://www.sfml-dev.org/download/sfml/2.1/) in the folder `.\SFML-2.1\`
* [optional] put the SFML `.dll` files in the repos root directory `.\`, for easier binary execution, as the lib is dynamically linked.
* If you are using mingw32, run `mingw32-make -f make-windows` to start compiling

Successfully build this way on win7x64 using mingw 4.2.0.
Makefiles for other platforms are appreciated!

## license:
unlicense (public domain).
