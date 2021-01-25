## Description

C++ 3D Open World Game Engine using OpenGL.

## Installation
Note*: Project is developed (respectively tested) under Windows 10 and Ubuntu 20.04.

Requirements:
1. Python 3
2. cmake >= 3.13

Steps for the installation:
1. Clone the repository.
2. Open a command line and navigate to the repository root folder.
3. Run `python3 update.py`
    * Updater script works as 'plug-and-play' only with VC++2019 on Windows. If you use another compiler, you have to replace `lib-vc2019` from `LIBRARY glfw/*/lib-vc2019/* win` instruction in `archive.json` with one of the following supported by GLFW library: `lib-mingw-w64 lib-vc2012 lib-vc2013 lib-vc2015 lib-vc2017 lib-vc2019`.

## Libraries used

* [OpenGL](https://www.opengl.org/)
* [GLEW](http://glew.sourceforge.net/)
* [GLFW](https://www.glfw.org/)
* [glm](https://glm.g-truc.net/0.9.9/index.html)
* [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
* [boost](https://www.boost.org/)

## Plan-to-use libraries

* [freetype](https://www.freetype.org/)

## Features

* [SOLID](https://en.wikipedia.org/wiki/SOLID) architecture written with [PascalCase](https://wiki.c2.com/?PascalCase) and [Hungarian notation](https://en.wikipedia.org/wiki/Hungarian_notation).
* Quad-tree terrain with dynamic tessellation, multi-texturing, heights, normals, displacement and bump normals. 
* Biome System (up to 32 biomes, with possibility to combine multiple biome)
* Props and Prop generator with possibility to spawn a certain object in a certain biome
* Skybox
* Free Camera and 3rd Person Camera (switchable at run-time)
* Player object with following basic movement: forward, backward, left, right, jumping with gravity and collision with the terrain
* Multiple lights (point lights and directional lights)
* Shadows (Shadow Mapping)
* Support for GUI Textures
* Water with DuDv and Normal maps and Fresnel effect
* Frustum Culling

## Available soon

* Spot lighting
* Day-night time
* Animations
* Animated Objects (such as mobs)
* Particle System
* Weather effects (raining, snowing using particle system)
* Post-rendering effects
* Lens flare effect
* Support for GUI Text
* Frustum Culling for Terrain

## Meta-project related TODOs
* Successfully built on UNIX systems
* Add support for updater on UNIX

## Goals

* Average of 50-60 fps
* No frame drops

## Known bugs

* Working only using Visual C++ compiler

## Authors

* **Lucian-Valentin Deaconu**
