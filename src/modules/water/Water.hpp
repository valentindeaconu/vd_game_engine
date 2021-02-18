//
// Created by Vali on 9/11/2020.
//

#ifndef VD_GAME_ENGINE_WATER_HPP
#define VD_GAME_ENGINE_WATER_HPP

#include <engine/object/Entity3D.hpp>

#include <engine/api/gl/FrameBuffer.hpp>

#include <engine/loader/PropertiesLoader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/kernel/Context.hpp>

#include <memory>
#include <numbers>
#include <unordered_map>

namespace mod::water {
    class Water : public vd::object::Entity3D, public vd::injector::Injectable {
    public:
        explicit Water(const std::string& propsFilePath);
        ~Water();

        void Link() override;

        void Setup() override;
        void Update() override;
        void CleanUp() override;

        [[nodiscard]] const vd::property::PropertiesPtr& Properties() const;

        [[nodiscard]] const vd::model::Material& Material() const;

        [[nodiscard]] const vd::gl::FrameBufferPtr& ReflectionFramebuffer() const;
        [[nodiscard]] const vd::gl::FrameBufferPtr& RefractionFramebuffer() const;

        [[nodiscard]] float GetHeight() const;

        [[nodiscard]] float GetMoveFactor() const;
    private:
        void PopulatePacks();
        void GeneratePatch();

        // Engine required to get current frame time
        vd::kernel::ContextPtr m_pContext;

        std::string m_CurrentPack;

        float m_MoveFactor;

        std::unordered_map<std::string, vd::model::Material> m_PacksMap;

        vd::property::PropertiesPtr m_pProperties;

        vd::gl::FrameBufferPtr m_pReflectionFBO;
        vd::gl::FrameBufferPtr m_pRefractionFBO;
    };
    typedef std::shared_ptr<Water>  WaterPtr;
}

#endif //VD_GAME_ENGINE_WATER_HPP
