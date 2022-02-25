
# pge-app

Simple implementation of an application relying on the [PixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine) to perform the rendering. This project comes with a basic event handling system, some basic menu which can register actions and a default `game` structure which can be extended to handle various processes.

It is meant as a simple way to build an application without having to reinvent the weel for every project. Most of the behaviors can be customized in order to handle more complex behaviors (resources loading, step function, pause system, etc.)

# Installation

- Clone the repo: `git clone git@github.com:Knoblauchpilze/pge-app.git`.
- Clone dependencies:
    * [core_utils](https://github.com/Knoblauchpilze/core_utils)
    * [maths_utils](https://github.com/Knoblauchpilze/maths_utils)
- Go to the project's directory `cd ~/path/to/the/repo`.
- Compile: `make r`.

Don't forget to add `/usr/local/lib` to your `LD_LIBRARY_PATH` to be able to load shared libraries at runtime. This is handled automatically when using the `make r` target (which internally uses the [run.sh](https://github.com/Knoblauchpilze/pge-app/blob/master/data/run.sh) script).

# Profiling

A convenience script is provided in order to profile the app. This comes from [this](https://stackoverflow.com/questions/375913/how-can-i-profile-c-code-running-on-linux) topic. This requires a few things to be installed on the system:
* GIMP
* valgrind
* [gprof2dot](https://github.com/jrfonseca/gprof2dot)

The output image is a png that is opened with GIMP and can give ideas about what is slowing down the application.

The profiling can be triggered with the following command:
```bash
make profile
```

# Usage

The application is structured around a base [DefaultApp](https://github.com/KnoblauchPilze/pge-app/blob/master/src/DefaultApp.hh) which can be customized to include more complex behaviors.

## Layers

The rendering is dividied into four layers:
* the debug layer is meant to handle every debug display and can be hidden for increased performance.
* the ui layer receives all the UI information such as menus. Can also be hidden if needed.
* the rendering layer receives any resource that compose the main content of the application.
* the decal layer is meant to receive any GPU accelerated resource.

The ordering of the layer matters as it will describe how elements are overlaid. The order is as follows:
* decal layer
* non-decal graphical resource layer
* ui layer
* debug layer

## Game

The application provides a base [Game](https://github.com/KnoblauchPilze/pge-app/blob/master/src/game/Game.hh) class which can be used to wrap the application's data into a structure that can communicate easily with the application. Some general methods have been extracted to provide hooks that are used by the default application to make the game evolve.

While this class is called `Game` it can also receive some other type of data.
