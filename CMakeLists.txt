cmake_minimum_required(VERSION 3.16)
project(vd_game_engine)

set(CMAKE_CXX_STANDARD 20)

include_directories(src/)
include_directories(src/engine/api/gl)
include_directories(src/engine/camera)
include_directories(src/engine/camera/first)
include_directories(src/engine/camera/free)
include_directories(src/engine/camera/third)
include_directories(src/engine/collision)
include_directories(src/engine/component)
include_directories(src/engine/core)
include_directories(src/engine/culling)
include_directories(src/engine/datastruct)
include_directories(src/engine/event)
include_directories(src/engine/exception)
include_directories(src/engine/fog)
include_directories(src/engine/kernel)
include_directories(src/engine/light)
include_directories(src/engine/loader)
include_directories(src/engine/logger)
include_directories(src/engine/math)
include_directories(src/engine/misc)
include_directories(src/engine/model)
include_directories(src/engine/object)
include_directories(src/engine/shader)
include_directories(src/engine/service)
include_directories(src/engine/window)
include_directories(src/modules/gui)
include_directories(src/modules/player)
include_directories(src/modules/shadow)
include_directories(src/modules/sky)
include_directories(src/modules/props)
include_directories(src/modules/terrain)
include_directories(src/modules/water)

add_executable(vd_game_engine
        src/engine/api/gl/GL.hpp
        src/engine/api/gl/GLTypes.hpp
        src/engine/api/gl/Buffer.hpp
        src/engine/api/gl/MeshBuffer.cpp
        src/engine/api/gl/MeshBuffer.hpp
        src/engine/api/gl/FrameBuffer.cpp
        src/engine/api/gl/FrameBuffer.hpp
        src/engine/api/gl/PatchBuffer.cpp
        src/engine/api/gl/PatchBuffer.hpp
        src/engine/api/gl/Texture.cpp
        src/engine/api/gl/Texture.hpp
        src/engine/api/gl/Shader.cpp
        src/engine/api/gl/Shader.hpp
        src/engine/camera/first/FirstPersonCamera.cpp
        src/engine/camera/first/FirstPersonCamera.hpp
        src/engine/camera/free/FreeCamera.cpp
        src/engine/camera/free/FreeCamera.hpp
        src/engine/camera/third/ThirdPersonCamera.cpp
        src/engine/camera/third/ThirdPersonCamera.hpp
        src/engine/camera/Camera.cpp
        src/engine/camera/Camera.hpp
        src/engine/camera/CameraWrapper.cpp
        src/engine/camera/CameraWrapper.hpp
        src/engine/collision/Detector.cpp
        src/engine/collision/Detector.hpp
        src/engine/component/IRenderer.cpp
        src/engine/component/IRenderer.hpp
        src/engine/component/IManager.cpp
        src/engine/component/IManager.hpp
        src/engine/component/IModuleFactory.hpp
        src/engine/component/RenderingPass.cpp
        src/engine/component/RenderingPass.hpp
        src/engine/component/IEntityShader.hpp
        src/engine/component/IShading.hpp
        src/engine/core/AsyncWorker.cpp
        src/engine/core/AsyncWorker.hpp
        src/engine/culling/FrustumCullingManager.cpp
        src/engine/culling/FrustumCullingManager.hpp
        src/engine/datastruct/Tree.cpp
        src/engine/datastruct/Tree.hpp
        src/engine/datastruct/Observer.cpp
        src/engine/datastruct/Observer.hpp
        src/engine/event/EventHandler.cpp
        src/engine/event/EventHandler.hpp
        src/engine/exception/Exceptions.cpp
        src/engine/exception/Exceptions.hpp
        src/engine/fog/FogManager.cpp
        src/engine/fog/FogManager.hpp
        src/engine/fog/Fog.cpp
        src/engine/fog/Fog.hpp
        src/engine/injector/CreateAndStore.hpp
        src/engine/injector/Injectable.cpp
        src/engine/injector/Injectable.hpp
        src/engine/injector/Linker.cpp
        src/engine/injector/Linker.hpp
        src/engine/injector/ObjectOfType.hpp
        src/engine/kernel/Engine.cpp
        src/engine/kernel/Engine.hpp
        src/engine/kernel/Context.cpp
        src/engine/kernel/Context.hpp
        src/engine/kernel/EngineFactory.cpp
        src/engine/kernel/EngineFactory.hpp
        src/engine/light/LightManager.cpp
        src/engine/light/LightManager.hpp
        src/engine/light/Light.cpp
        src/engine/light/Light.hpp
        src/engine/loader/FileLoader.cpp
        src/engine/loader/FileLoader.hpp
        src/engine/loader/ImageLoader.cpp
        src/engine/loader/ImageLoader.hpp
        src/engine/loader/ObjectLoader.cpp
        src/engine/loader/ObjectLoader.hpp
        src/engine/loader/ShaderLoader.cpp
        src/engine/loader/ShaderLoader.hpp
        src/engine/loader/PropertiesLoader.cpp
        src/engine/loader/PropertiesLoader.hpp
        src/engine/loader/impl/StreamImpl.cpp
        src/engine/loader/impl/StreamImpl.hpp
        src/engine/loader/impl/StbiImpl.cpp
        src/engine/loader/impl/StbiImpl.hpp
        src/engine/loader/impl/TinyObjLoaderImpl.cpp
        src/engine/loader/impl/TinyObjLoaderImpl.hpp
        src/engine/loader/impl/VDGEGLSLImpl.cpp
        src/engine/loader/impl/VDGEGLSLImpl.hpp
        src/engine/logger/Logger.cpp
        src/engine/logger/Logger.hpp
        src/engine/math/Frustum.cpp
        src/engine/math/Frustum.hpp
        src/engine/math/Interpolator.cpp
        src/engine/math/Interpolator.hpp
        src/engine/math/Bounds.cpp
        src/engine/math/Bounds.hpp
        src/engine/math/Plane.hpp
        src/engine/math/Plane.cpp
        src/engine/math/Transform.cpp
        src/engine/math/Transform.hpp
        src/engine/misc/Types.hpp
        src/engine/model/Image.cpp
        src/engine/model/Image.hpp
        src/engine/model/Material.hpp
        src/engine/model/Mesh.hpp
        src/engine/model/Vertex.hpp
        src/engine/object/Entity.cpp
        src/engine/object/Entity.hpp
        src/engine/property/Properties.cpp
        src/engine/property/Properties.hpp
        src/engine/service/TextureService.cpp
        src/engine/service/TextureService.hpp
        src/engine/window/Window.cpp
        src/engine/window/Window.hpp
        src/modules/gui/GuiQuad.cpp
        src/modules/gui/GuiQuad.hpp
        src/modules/gui/GuiRenderer.cpp
        src/modules/gui/GuiRenderer.hpp
        src/modules/gui/GuiShader.cpp
        src/modules/gui/GuiShader.hpp
        src/modules/gui/GuiFactory.cpp
        src/modules/gui/GuiFactory.hpp
        src/modules/player/Player.cpp
        src/modules/player/Player.hpp
        src/modules/player/PlayerShader.cpp
        src/modules/player/PlayerShader.hpp
        src/modules/player/PlayerRenderer.cpp
        src/modules/player/PlayerRenderer.hpp
        src/modules/player/PlayerFactory.cpp
        src/modules/player/PlayerFactory.hpp
        src/modules/shadow/ShadowBox.cpp
        src/modules/shadow/ShadowBox.hpp
        src/modules/shadow/ShadowShader.cpp
        src/modules/shadow/ShadowShader.hpp
        src/modules/shadow/ShadowManager.cpp
        src/modules/shadow/ShadowManager.hpp
        src/modules/shadow/ShadowFactory.cpp
        src/modules/shadow/ShadowFactory.hpp
        src/modules/sky/Sky.cpp
        src/modules/sky/Sky.hpp
        src/modules/sky/SkyShader.cpp
        src/modules/sky/SkyShader.hpp
        src/modules/sky/SkyRenderer.cpp
        src/modules/sky/SkyRenderer.hpp
        src/modules/sky/SkyFactory.cpp
        src/modules/sky/SkyFactory.hpp
        src/modules/props/Prop.cpp
        src/modules/props/Prop.hpp
        src/modules/props/PropsRenderer.cpp
        src/modules/props/PropsRenderer.hpp
        src/modules/props/PropShader.cpp
        src/modules/props/PropShader.hpp
        src/modules/props/PropGenerator.cpp
        src/modules/props/PropGenerator.hpp
        src/modules/props/PropsFactory.cpp
        src/modules/props/PropsFactory.hpp
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
        src/modules/terrain/normalmap/NormalMapBuilder.cpp
        src/modules/terrain/normalmap/NormalMapBuilder.hpp
        src/modules/terrain/splatmap/SplatMapBuilder.cpp
        src/modules/terrain/splatmap/SplatMapBuilder.hpp
        src/modules/terrain/TerrainFactory.cpp
        src/modules/terrain/TerrainFactory.hpp
        src/modules/water/Water.cpp
        src/modules/water/Water.hpp
        src/modules/water/WaterShader.cpp
        src/modules/water/WaterShader.hpp
        src/modules/water/WaterRenderer.cpp
        src/modules/water/WaterRenderer.hpp
        src/modules/water/WaterFactory.cpp
        src/modules/water/WaterFactory.hpp
        src/modules/ModuleFactory.cpp
        src/modules/ModuleFactory.hpp
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

