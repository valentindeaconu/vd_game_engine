cmake_minimum_required(VERSION 3.16)
project(vd_game_engine)

set(CMAKE_CXX_STANDARD 20)

include_directories(src/)
include_directories(src/engine/component)
include_directories(src/engine/component/impl)
include_directories(src/engine/config)
include_directories(src/engine/core)
include_directories(src/engine/core/impl)
include_directories(src/engine/foundation)
include_directories(src/engine/foundation/imgloader)
include_directories(src/engine/foundation/imgloader/model)
include_directories(src/engine/foundation/math)
include_directories(src/engine/foundation/objloader)
include_directories(src/engine/glmodel/buffer)
include_directories(src/engine/kernel)
include_directories(src/engine/logger)
include_directories(src/engine/model)
include_directories(src/engine/object)
include_directories(src/engine/shader)
include_directories(src/modules/player)
include_directories(src/modules/sky)
include_directories(src/modules/sobj)
include_directories(src/modules/terrain)

add_executable(vd_game_engine
        src/engine/component/impl/EntityRenderer.cpp
        src/engine/component/impl/EntityRenderer.hpp
        src/engine/component/RenderConfig.cpp
        src/engine/component/RenderConfig.hpp
        src/engine/component/Renderer.cpp
        src/engine/component/Renderer.hpp
        src/engine/config/ConfigurationFile.cpp
        src/engine/config/ConfigurationFile.hpp
        src/engine/config/EngineConfig.cpp
        src/engine/config/EngineConfig.hpp
        src/engine/core/impl/EntityCamera.cpp
        src/engine/core/impl/EntityCamera.hpp
        src/engine/core/impl/FreeCamera.cpp
        src/engine/core/impl/FreeCamera.hpp
        src/engine/core/Camera.cpp
        src/engine/core/Camera.hpp
        src/engine/core/InputHandler.cpp
        src/engine/core/InputHandler.hpp
        src/engine/core/Window.cpp
        src/engine/core/Window.hpp
        src/engine/foundation/imgloader/model/Image.cpp
        src/engine/foundation/imgloader/model/Image.hpp
        src/engine/foundation/imgloader/IIMGLoader.cpp
        src/engine/foundation/imgloader/IIMGLoader.hpp
        src/engine/foundation/imgloader/IMGLoader.cpp
        src/engine/foundation/imgloader/IMGLoader.hpp
        src/engine/foundation/math/Transform.cpp
        src/engine/foundation/math/Transform.hpp
        src/engine/foundation/objloader/OBJLoader.cpp
        src/engine/foundation/objloader/OBJLoader.hpp
        src/engine/foundation/GL.hpp
        src/engine/glmodel/buffer/Buffer.hpp
        src/engine/glmodel/buffer/MeshBuffer.cpp
        src/engine/glmodel/buffer/MeshBuffer.hpp
        src/engine/kernel/EngineBlock.cpp
        src/engine/kernel/EngineBlock.hpp
        src/engine/kernel/EngineWorker.cpp
        src/engine/kernel/EngineWorker.hpp
        src/engine/logger/Logger.cpp
        src/engine/logger/Logger.hpp
        src/engine/model/Light.cpp
        src/engine/model/Light.hpp
        src/engine/model/Material.hpp
        src/engine/model/Mesh.hpp
        src/engine/model/Texture.cpp
        src/engine/model/Texture.hpp
        src/engine/model/Vertex.hpp
        src/engine/object/Entity.cpp
        src/engine/object/Entity.hpp
        src/engine/shader/GLSLWrapper.cpp
        src/engine/shader/GLSLWrapper.hpp
        src/engine/shader/Shader.cpp
        src/engine/shader/Shader.hpp
        src/modules/player/Player.cpp
        src/modules/player/Player.hpp
        src/modules/player/PlayerShader.cpp
        src/modules/player/PlayerShader.hpp
        src/modules/sky/Sky.cpp
        src/modules/sky/Sky.hpp
        src/modules/sky/SkyShader.cpp
        src/modules/sky/SkyShader.hpp
        src/modules/sobj/StaticObject.cpp
        src/modules/sobj/StaticObject.hpp
        src/modules/sobj/StaticObjectPlacer.cpp
        src/modules/sobj/StaticObjectPlacer.hpp
        src/modules/sobj/StaticObjectRenderer.cpp
        src/modules/sobj/StaticObjectRenderer.hpp
        src/modules/sobj/StaticObjectShader.cpp
        src/modules/sobj/StaticObjectShader.hpp
        src/modules/terrain/Terrain.cpp
        src/modules/terrain/Terrain.hpp
        src/modules/terrain/TerrainConfig.cpp
        src/modules/terrain/TerrainConfig.hpp
        src/modules/terrain/TerrainRenderer.cpp
        src/modules/terrain/TerrainRenderer.hpp
        src/modules/terrain/TerrainShader.cpp
        src/modules/terrain/TerrainShader.hpp
        src/engine/shadow/ShadowBox.cpp
        src/engine/shadow/ShadowBox.hpp
        src/engine/shadow/ShadowShader.cpp
        src/engine/shadow/ShadowShader.hpp
        src/engine/shadow/ShadowManager.cpp
        src/engine/shadow/ShadowManager.hpp
        src/modules/gui/GuiQuad.cpp
        src/modules/gui/GuiQuad.hpp
        src/modules/gui/GuiRenderer.cpp
        src/modules/gui/GuiRenderer.hpp
        src/modules/gui/GuiShader.cpp
        src/modules/gui/GuiShader.hpp
        src/main.cpp)

# dependencies

# opengl
find_package(OpenGL REQUIRED)

target_include_directories(vd_game_engine PRIVATE ${OPENGL_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)

if (WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${CMAKE_SOURCE_DIR}/lib/win)
    find_library(GLFW_LIBRARY glfw3 HINTS ${CMAKE_SOURCE_DIR}/lib/win)

    target_link_libraries(vd_game_engine PRIVATE
            ${GLEW_LIBRARY}
            ${GLFW_LIBRARY}
            opengl32)

    if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/glfw3.dll)
        file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/glfw3.dll )
    endif()
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lib/win/glfw3.dll ${CMAKE_CURRENT_BINARY_DIR}/glfw3.dll COPYONLY)
endif (WIN32)

if (UNIX)
    find_library(GLEW_LIBRARY GLEW HINTS ${CMAKE_SOURCE_DIR}/lib/unix)
    find_library(GLFW_LIBRARY glfw3 HINTS ${CMAKE_SOURCE_DIR}/lib/unix)

    target_link_libraries(vd_game_engine PRIVATE ${GLFW_LIBRARY} ${GLEW_LIBRARY} GL dl X11 pthread)

    add_definitions(-DGLEW_NO_GLU)

    #if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/libGLEW.a)
    #    file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/libGLEW.a)
    #endif()
    #configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lib/unix/libGLEW.a ${CMAKE_CURRENT_BINARY_DIR}/libGLEW.a COPYONLY)
endif (UNIX)

# Copy resources to binary

if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/resources)
    file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/resources)
endif()
file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

