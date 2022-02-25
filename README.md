
# cellify

Implementation of a very simple open-world cell landscape with resources and agents. The world is a grid with discrete coordinates so as to ease a lot the concerns about motion and paths.

# Installation

- Clone the repo: `git clone git@github.com:Knoblauchpilze/cellify.git`.
- Clone dependencies:
    * [core_utils](https://github.com/Knoblauchpilze/core_utils)
    * [maths_utils](https://github.com/Knoblauchpilze/maths_utils)
- Go to the project's directory `cd ~/path/to/the/repo`.
- Compile: `make r`.

Don't forget to add `/usr/local/lib` to your `LD_LIBRARY_PATH` to be able to load shared libraries at runtime. This is handled automatically when using the `make r` target (which internally uses the [run.sh](https://github.com/Knoblauchpilze/cellify/blob/master/data/run.sh) script).
