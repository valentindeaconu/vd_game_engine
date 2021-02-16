## Description

C++ 3D Open World Game Engine using OpenGL.

## Installation
Note*: Project is developed (respectively tested) under Windows 10 and Ubuntu 20.04.

Requirements:
1. python >= 3
2. cmake >= 3.13
3. C++20 (VC++ >= 2012 or GCC >= 9)

Steps for the installation:
1. Clone the repository.
2. Open a command line and navigate to the repository root folder.
3. Run `python3 update.py` (this process may take a while)
    * On Windows you have to select proper library for your compiler from `LIBRARY glfw/*/<compiler library>/* /` instruction in `archive.json`. Choose one of the following supported by GLFW library: `lib-mingw-w64 lib-vc2012 lib-vc2013 lib-vc2015 lib-vc2017 lib-vc2019`.

## Libraries used

* [OpenGL](https://www.opengl.org/)
* [GLEW](http://glew.sourceforge.net/)
* [GLFW](https://www.glfw.org/)
* [glm](https://glm.g-truc.net/0.9.9/index.html)
* [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
* [boost](https://www.boost.org/)

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

## Project Board

Available on [Trello](https://trello.com/b/lx9Ny7u6/vdge)

## Goals

* Average of 50-60 fps
* No frame drops

## Authors

* **Lucian-Valentin Deaconu**
