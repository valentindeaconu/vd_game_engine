## Description

C++ 3D Open World Game Engine using OpenGL.

## Installation
Note*: Project is developed (respectively tested) under Windows 10 and Ubuntu 20.04.

Dependency Manager requirements:
1. Python
2. cmake >= 3.13
3. (UNIX only) Super user privileges.

Steps for the installation:
1. Download the repository.
2. Open a command line and navigate to the repository root folder.
3. (Windows Only) Select VC++ version on `dpdmgr.py`.
4. Run `python dpdmgr.py`.

Optional* Use any IDE which supports CMake (CLion, Visual Studio, Qt Creator etc.) and import the root folder.

## Libraries used

* [OpenGL](https://www.opengl.org/)
* [GLEW](http://glew.sourceforge.net/)
* [GLFW](https://www.glfw.org/)
* [glm](https://glm.g-truc.net/0.9.9/index.html)
* [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)

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

## Goals

* Average of 50-60 fps
* No frame drops

## Known bugs

* Working only using VisualC++ compiler

## Authors

* **Lucian-Valentin Deaconu**
