//
// Created by Vali on 9/11/2020.
//

#ifndef VD_GAME_ENGINE_WATER_HPP
#define VD_GAME_ENGINE_WATER_HPP

#include <engine/object/Entity.hpp>

#include <engine/glmodel/buffer/FrameBuffer.hpp>

#include <engine/misc/Properties.hpp>

#include <engine/misc/ObjectOfType.hpp>
#include <engine/kernel/Engine.hpp>

#include <memory>
#include <numbers>
#include <unordered_map>

namespace mod::water {
    class Water : public vd::object::Entity {
    public:
        explicit Water(const std::string& propsFilePath);
        ~Water();

        void Init() override;
        void Update() override;
        void CleanUp() override;

        [[nodiscard]] const vd::misc::PropertiesPtr& GetProperties() const;

        [[nodiscard]] const vd::model::Material& GetMaterial() const;

        [[nodiscard]] const vd::buffer::FrameBufferPtr& GetReflectionFramebuffer() const;
        [[nodiscard]] const vd::buffer::FrameBufferPtr& GetRefractionFramebuffer() const;

        [[nodiscard]] float GetHeight() const;

        [[nodiscard]] float GetMoveFactor() const;
    private:
        void PopulatePacks();
        void GeneratePatch();

        // Engine required to get current frame time
        vd::EnginePtr m_EnginePtr;

        std::string m_CurrentPack;

        float m_MoveFactor;

        std::unordered_map<std::string, vd::model::Material> m_PacksMap;

        vd::misc::PropertiesPtr m_PropsPtr;

        vd::buffer::FrameBufferPtr m_ReflectionFBO;
        vd::buffer::FrameBufferPtr m_RefractionFBO;
    };
    typedef std::shared_ptr<Water>  WaterPtr;
}

#endif //VD_GAME_ENGINE_WATER_HPP
