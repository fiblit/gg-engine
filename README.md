# Author
Dalton Hildreth

# Resources:
I used [Glitter](http://polytonic.github.io/Glitter/) to get the boilerplate of OpenGL/GLFW working.
I didn't use Bullet, but I used every other library from it.

I used http://learnopengl.com to, well, learn OpenGL, so some of his code is in mine to do rendering.



# Plan
At some point there will be the following:
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

# Build
To Init:
```
git --recursive clone https://github.com/fiblit/gg-engine.git
```

To Build:
```
cd Build
# UNIX Makefile
# Although, this might work anywhere.
cmake ..

# Mac OSX
# cmake -G "Xcode" ..

# Microsoft Windows
# cmake -G "Visual Studio 14" ..
# cmake -G "Visual Studio 14 Win64" ..
```
