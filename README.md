## Description

C++ 3D Open World Game Engine using OpenGL.

## Installation

1. Download the repository.
2. Create 2 folders, "include" and "lib".
3. Inside "lib", create 2 folders "win" and "unix".
4. Download the binaries according to your operating system and put them into the according folder.

Optional* Use any IDE which supports CMake (CLion, Visual Studio, Qt Creator etc.) and import the root folder.

Also:
* UNIX GNU GCC and MinGW (posix build) cannot start the application (gray screen). 
* Use Visual C on Windows.

## Libraries used

* [OpenGL](https://www.opengl.org/)
* [GLEW](http://glew.sourceforge.net/)
* [GLFW](https://www.glfw.org/)
* [freetype](https://www.freetype.org/) - not used yet
* [glm](https://glm.g-truc.net/0.9.9/index.html)
* [OpenCV](https://opencv.org/) - not used anymore
* [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)
* [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)

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
* Artifact downloader
* Successfully built on UNIX systems

## Goals

* Average of 50-60 fps
* No frame drops

## Authors

* **Lucian-Valentin Deaconu**
