# Concentric Clock
#### v0.2

An unconventional representation of a clock.
It shows rotating concentric rings, each showing one unit of time & date.
[![Screenshot] http://github.com/noerw/concentric-clock/blob/master/screenshot.png]

This concept was inspired by mindbrix' ["Concentrichon"](http://www.concentrichron.com/)

Written as an excercise in C++ and SFML.

## dependencies:
* SFML 2.1

## build
Currently only building on windows is supported natively.
To do so, put the [SFML-library (v2.1)](http://www.sfml-dev.org/download/sfml/2.1/) in the folder `.\SFML-2.1\`.
If you are using mingw32, run `mingw32-make -f make-windows` to start compiling.

Successfully build this on win7x64 using mingw 4.2.0 build by nixMan.
Makefiles for other platforms are appreciated!

## license:
unlicense (public domain).
