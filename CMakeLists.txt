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
include_directories(src/engine/foundation/img/imgloader)
include_directories(src/engine/foundation/img/model)
include_directories(src/engine/math)
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
        src/engine/collision/Detector.cpp
        src/engine/collision/Detector.hpp
        src/engine/component/IRenderer.cpp
        src/engine/component/IRenderer.hpp
        src/engine/component/IManager.cpp
        src/engine/component/IManager.hpp
        src/engine/config/MetaConfig.hpp
        src/engine/config/MetaConfig.cpp
        src/engine/camera/third/ThirdPersonCamera.cpp
        src/engine/camera/third/ThirdPersonCamera.hpp
        src/engine/camera/free/FreeCamera.cpp
        src/engine/camera/free/FreeCamera.hpp
        src/engine/camera/ICamera.cpp
        src/engine/camera/ICamera.hpp
        src/engine/event/EventHandler.cpp
        src/engine/event/EventHandler.hpp
        src/engine/window/Window.cpp
        src/engine/window/Window.hpp
        src/engine/core/AsyncWorker.cpp
        src/engine/core/AsyncWorker.hpp
        src/engine/misc/ObjectOfType.hpp
        src/engine/culling/FrustumCullingManager.cpp
        src/engine/culling/FrustumCullingManager.hpp
        src/engine/foundation/img/Image.cpp
        src/engine/foundation/img/Image.hpp
        src/engine/foundation/img/imgloader/IIMGLoader.cpp
        src/engine/foundation/img/imgloader/IIMGLoader.hpp
        src/engine/foundation/img/imgloader/IMGLoader.cpp
        src/engine/foundation/img/imgloader/IMGLoader.hpp
        src/engine/foundation/img/imghelper/ImageHelper.cpp
        src/engine/foundation/img/imghelper/ImageHelper.hpp
        src/engine/math/Transform.cpp
        src/engine/math/Transform.hpp
        src/engine/foundation/objloader/OBJLoader.cpp
        src/engine/foundation/objloader/OBJLoader.hpp
        src/engine/foundation/GL.hpp
        src/engine/math/Frustum.cpp
        src/engine/math/Frustum.hpp
        src/engine/math/Bounds.cpp
        src/engine/math/Bounds.hpp
        src/engine/math/Plane.hpp
        src/engine/math/Plane.cpp
        src/engine/foundation/datastruct/Tree.cpp
        src/engine/foundation/datastruct/Tree.hpp
        src/engine/foundation/datastruct/Observer.cpp
        src/engine/foundation/datastruct/Observer.hpp
        src/engine/foundation/algorithm/TreeHelper.cpp
        src/engine/foundation/algorithm/TreeHelper.hpp
        src/engine/glmodel/buffer/Buffer.hpp
        src/engine/glmodel/buffer/MeshBuffer.cpp
        src/engine/glmodel/buffer/MeshBuffer.hpp
        src/engine/glmodel/buffer/FrameBuffer.cpp
        src/engine/glmodel/buffer/FrameBuffer.hpp
        src/engine/glmodel/buffer/PatchBuffer.cpp
        src/engine/glmodel/buffer/PatchBuffer.hpp
        src/engine/kernel/Engine.cpp
        src/engine/kernel/Engine.hpp
        src/engine/kernel/EngineFactory.cpp
        src/engine/kernel/EngineFactory.hpp
        src/engine/light/LightManager.cpp
        src/engine/light/LightManager.hpp
        src/engine/light/Light.cpp
        src/engine/light/Light.hpp
        src/engine/logger/Logger.cpp
        src/engine/logger/Logger.hpp
        src/engine/misc/Properties.cpp
        src/engine/misc/Properties.hpp
        src/engine/misc/Types.hpp
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
        src/modules/player/PlayerRenderer.cpp
        src/modules/player/PlayerRenderer.hpp
        src/modules/sky/Sky.cpp
        src/modules/sky/Sky.hpp
        src/modules/sky/SkyShader.cpp
        src/modules/sky/SkyShader.hpp
        src/modules/sky/SkyRenderer.cpp
        src/modules/sky/SkyRenderer.hpp
        src/modules/sobj/StaticObject.cpp
        src/modules/sobj/StaticObject.hpp
        src/modules/sobj/StaticObjectPlacer.cpp
        src/modules/sobj/StaticObjectPlacer.hpp
        src/modules/sobj/StaticObjectRenderer.cpp
        src/modules/sobj/StaticObjectRenderer.hpp
        src/modules/sobj/StaticObjectShader.cpp
        src/modules/sobj/StaticObjectShader.hpp
        src/modules/terrain/TerrainShader.cpp
        src/modules/terrain/TerrainShader.hpp
        src/modules/terrain/Terrain.cpp
        src/modules/terrain/Terrain.hpp
        src/modules/terrain/TerrainNode.cpp
        src/modules/terrain/TerrainNode.hpp
        src/modules/terrain/Biome.cpp
        src/modules/terrain/Biome.hpp
        src/modules/terrain/TerrainRenderer.cpp
        src/modules/terrain/TerrainRenderer.hpp
        src/modules/terrain/normalmap/NormalMapShader.cpp
        src/modules/terrain/normalmap/NormalMapShader.hpp
        src/modules/terrain/normalmap/NormalMapRenderer.cpp
        src/modules/terrain/normalmap/NormalMapRenderer.hpp
        src/modules/terrain/splatmap/SplatMapRenderer.cpp
        src/modules/terrain/splatmap/SplatMapRenderer.hpp
        src/modules/terrain/splatmap/SplatMapShader.cpp
        src/modules/terrain/splatmap/SplatMapShader.hpp
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
        src/modules/water/Water.cpp
        src/modules/water/Water.hpp
        src/modules/water/WaterShader.cpp
        src/modules/water/WaterShader.hpp
        src/modules/water/WaterRenderer.cpp
        src/modules/water/WaterRenderer.hpp
        src/main.cpp src/engine/camera/first/FirstPersonCamera.cpp src/engine/camera/first/FirstPersonCamera.hpp src/engine/component/RenderingPass.cpp src/engine/component/RenderingPass.hpp)

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

