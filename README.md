# About

Turncoat is a 3D game engine being made for programmers putting on their
creative coats (or vice versa). It will promote emergent gameplay, procedural
content, and grand simulation game designs, as well as simlar genres. It's a
fun pet project of mine that I've used to learn graphics with over time, as
well. Hopefully, you can enjoy using -- or collaborating on! -- Turncoat.

# Goals

While this project existed for a while with a singular goal, *learn stuff*,
over time I have listed down what I see as the intended capabilities of this
game engine and library. Many of these are inspired by the games I like making
and playing (and perhaps tiny niches I see in the game engine space).

* Learning for me
* "Programmers are creative too"
* Open Source
* Modularity / Moddability
* Ease
    * Easy for Engine Developers
    * Easy for Game Developers
* Lightweight: always easy & low or no external dependencies
* Technically impressive (largely such that I learn a lot)
    * Modern
        * (Physically/AI/Multi-agent) based (Animation/Rendering)
    * FAST
        * When you *must* choose, choose less time over less memory
    * Pretty
    * Parallel
        * Incorporate a Vulkan backend
* Optimizations for
    * Simulation-based games
    * Massive procedural content generation
        * (e.g. voxelized worlds or generating anything)
    * This is all to make pretty, *emergent* games even as just a programmer.
* 3D for
    * Simulation
    * RTS
    * Management
    * RPG
    * Action
    * Often Real-time
    * Often Eagle's Eye or First-Person
* Networking: All of these genres (and the ideas I want to make elsewhere or
    with this someday necessarily require it)
* Gradual Support as I work into creating systems for
    * 2D
    * Spatial Audio
    * M/A/VR

# Architecture Plan (Old)
At some point there will be the following components, perhaps rearranged when
I read more about game engine architecture, or as I decide something is better
while working on it (incremental agility):

* Manager/interface module 
    * timer
    * Memory Unit
        * pool
* Render/lighting module
    * scene\_graph
    * light
    * mesh
    * texture
    * shader
    * model
    * primitive mesh
        * line/cylinder
        * cubic/quad
        * ellipsoid/ellipse
        * triangle/prism
        * k-DOP
* Physics/simulation module
    * BVH
    * collider
    * particle
    * integrator
* AI/planning module
    * Agent
    * PRM
    * Graph
    * Global (may not actually be a module)
        * A\*
        * RRT\*
    * Local (may not actually be a module)
        * Boids
        * TTC
        * VBO
        * ORCA
* Math module 
    * glm
* OS I/O module 
    * parser for any internal format (levels/scenes/etc.)
    * image loader -- stb\_image
    * mesh loader -- assimp
    * writer (for any internal format/log)
* UI module 
  * Terminal/CLI input
  * Keymapper
  * GUI
* Test module (testing / debug-vis / debug-only-code / etc.)
    * unit test suite
    * visual debug overlay
    * debug only
    * benchmarking / logger

# Resources:
I used http://learnopengl.com to, well, learn OpenGL, so some of my code is
based on his to do rendering.

# Build
Turncoat is intended to be open to all systems: it's cross-platform. However...
the build system's first step (fetching library dependencies) is not windows
compatible. I am working to fix this. I usually develop on Linux so Windows can
be behind often. I'm also looking into the most painless way to handle these
dependencies in a cross-platform manner.

## Linux/OS X
In the terminal perform the following to go from nothing to a running build:
```
$ git clone https://github.com/fiblit/gg-engine.git
$ cd gg-engine
$ . lib/init.sh
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./bin/gg-engine
```
If you run into issues, it may be because you don't have bash or CMake. On
Linux, do this to get them:
```
$ sudo apt-get install cmake
$ sudo apt-get install bash
```
