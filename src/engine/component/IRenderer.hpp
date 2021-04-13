//
// Created by Vali on 11/10/2020.
//

#ifndef VD_GAME_ENGINE_IRENDERER_HPP
#define VD_GAME_ENGINE_IRENDERER_HPP

#include <engine/datastruct/Observer.hpp>
#include <engine/defines/Types.hpp>

#include <engine/exception/Exceptions.hpp>

#include <string>

namespace vd::component {
    namespace exception {
        struct RendererError : public CustomException {
            explicit RendererError(const std::string& name, const std::string& message);
        };
    }

    class IRenderer : public datastruct::Observer {
    public:
        static const vd::datastruct::Observable::priority_t kDefaultPriority = 200;

        explicit IRenderer(std::string name);

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;

        virtual void OnInit() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnRender(const params_t& params) = 0;
        virtual void OnCleanUp() = 0;

        [[nodiscard]] const std::string& Name() const;
    protected:
        virtual bool Precondition(const params_t& params) = 0;
        virtual void Prepare() = 0;
        virtual void Finish() = 0;

    private:
        bool        m_Initialized;
        std::string m_Name;
    };
    typedef std::shared_ptr<IRenderer>	RendererPtr;
}

#endif // VD_GAME_ENGINE_IRENDERER_HPP