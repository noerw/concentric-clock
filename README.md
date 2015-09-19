# Concentric Clock

An unconventional representation of a clock.
It shows rotating concentric rings, each showing one unit of time & date.
[Screenshot!](http://github.com/noerw/concentric-clock/blob/master/screenshot.png)

This concept was inspired by mindbrix' ["Concentrichron"](http://www.concentrichron.com/).

Written as an excercise in C++ and SFML.

## dependencies:
* SFML 2.1

## build it!

### Debian / Ubuntu (tested on ubuntu 14.04 x64)
* install dependencies once:
```
sudo apt-get update
sudo apt-get install libsfml-dev libsfml2-dbg
```
* compile:
```
make -f make-linux
```
* run: `./bin/concentric-clock_dbg`

Note, that the linux build contains heavy artifacts and aliasing in the graphics. I'm looking for the source of this issue.

### Windows (tested on win7 x64)
* put the [SFML-library (v2.1)](http://www.sfml-dev.org/download/sfml/2.1/) libraries in the folder `.\lib\`
* put the SFML include files in the folder `.\include\SFML\`
* [optional] put the SFML `.dll` files in the repos `.\bin` directory for easier binary execution, as the lib is dynamically linked.
* If you are using mingw32, run `mingw32-make -f make-windows` to start compiling

Successfully build this way on win7x64 using mingw 4.2.0.

## license:
unlicense (public domain).
