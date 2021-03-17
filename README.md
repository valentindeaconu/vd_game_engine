## Description

C++ 3D Open World Game Engine using OpenGL.

## Installation
Note*: Project is developed (respectively tested) under Windows 10 and Ubuntu 20.04 LTS. 

Requirements:
1. cmake >= 3.13
2. C++20 (VC++ >= 2012 or GCC >= 9)
3. python >= 3.8
4. (UNIX) xorg-dev libglu1-mesa-dev build-essential libXmu-dev libXi-dev libgl-dev

Steps for the installation:
1. Clone the repository.
2. Open a command line and navigate to the repository root folder.
3. Run `./build.py --update` (this process may take a while)
    * On Windows you have to select proper library for your compiler from `LIBRARY glfw/*/<compiler library>/* /` instruction in `archive.json`. Choose one of the following supported by GLFW library: `lib-mingw-w64 lib-vc2012 lib-vc2013 lib-vc2015 lib-vc2017 lib-vc2019`.

## Libraries used

* [OpenGL](https://www.opengl.org/)
* [GLEW](http://glew.sourceforge.net/)
* [GLFW](https://www.glfw.org/)
* [glm](https://glm.g-truc.net/0.9.9/index.html)
* [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
* [boost](https://www.boost.org/)
* [FreeType](https://www.freetype.org/)

## Features

* [SOLID](https://en.wikipedia.org/wiki/SOLID) architecture written with [PascalCase](https://wiki.c2.com/?PascalCase) and [Hungarian notation](https://en.wikipedia.org/wiki/Hungarian_notation).
* Quad-tree terrain with dynamic tessellation, multi-texturing, heights, normals, displacement and bump normals. 
* Biome System (up to 32 biomes, with possibility to combine multiple biome)
* Props and Prop generator with possibility to spawn a certain object in a certain biome and multiple level of details
* Skybox
* Free Camera and 3rd Person Camera (switchable at run-time)
* Player object with following basic movement: forward, backward, left, right, jumping with gravity and collision with the terrain
* Multiple lights (point lights and directional lights)
* Shadows (Shadow Mapping)
* Support for GUI Textures and Texts
* Water with DuDv and Normal maps and Fresnel effect
* Frustum Culling
* Wireframe Mode
* Post-Rendering Effects (Depth of Field effect)
* Day-night cycle with an amazing [texture pack](https://assetstore.unity.com/packages/2d/textures-materials/sky/10-skyboxes-pack-day-night-32236#description)
* Lens Flare with Occlusion Culling

## Project Board

Available on [Trello](https://trello.com/b/lx9Ny7u6/vdge).

## Goals

* Average of 50-60 fps
* No frame drops

## Authors

* **Lucian-Valentin Deaconu**

## Thanks to

* [ThinMatrix](https://www.youtube.com/watch?v=VS8wlS9hF8E&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP) - tutorials
* [OreonEngine](https://github.com/fynnfluegge/oreon-engine) - terrain, some resources
* [LearnOpenGL](https://learnopengl.com/) - tutorials