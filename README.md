## Description

C++ 3D Open World Game Engine using OpenGL.

## Installation
1. Download the repository.
2. Open a command line and navigate to the repository root folder.
3. Execute `python dpdmgr.py`.

Optional* Use any IDE which supports CMake (CLion, Visual Studio, Qt Creator etc.) and import the root folder.

Also:
* Use VC++ on Windows (mingw is not working)
* Select VC++ version on dpdmgr before running it

## Libraries used

* [OpenGL](https://www.opengl.org/)
* [GLEW](http://glew.sourceforge.net/)
* [GLFW](https://www.glfw.org/)
* [glm](https://glm.g-truc.net/0.9.9/index.html)
* [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)
* [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)

* [freetype](https://www.freetype.org/)
* [OpenCV](https://opencv.org/)

## Features

* SOLID architecture
* Terrain with heights and multiple textures, each texture defining a different biome
* Biome System (up to 32 biomes, with possibility to combine multiple biome)
* Static Objects
* Random Object Generator with possibility to spawn a certain object in a certain biome
* Skydome
* Free Camera and 3rd Person Camera (actually using only 3rd Person Camera, but the switch will be available soon.
* Player object with following basic movement: forward, backward, left, right, jumping with gravity and collision with the terrain
* Multiple lights (only point lights for now, spot and directional will be added soon)

## Available soon

* Shadows
* Day-night time
* Water
* Animations
* Animated Objects (such as mobs)
* Particle System
* Weather effects (raining, snowing using particle system)
* Post-rendering effects
* Lens flare effect

## Meta-project related TODOs
* Artifact downloader (for now, available only for windows)
* Successfully built on UNIX systems

## Goals

* Average of 50-60 fps
* No frame drops

## Authors

* **Lucian-Valentin Deaconu**
