#ifndef __SKY_HPP_
#define __SKY_HPP_

#include <engine/object/Entity.hpp>

#include <memory>

namespace mod::sky
{
    class Sky : public vd::object::Entity
    {
    public:
        Sky(const vd::EnginePtr& enginePtr);
        ~Sky();

        void init() override;
        void update() override;
        void cleanUp() override;
    private:
        const std::vector<float> kSkyboxVertices = {
            -1.0f, -1.0f, -1.0f,    // 0
            -1.0f, -1.0f, 1.0f,     // 1
            -1.0f, 1.0f, -1.0f,     // 2
            -1.0f, 1.0f, 1.0f,      // 3
            1.0f, -1.0f, -1.0f,     // 4
            1.0f, -1.0f, 1.0f,      // 5
            1.0f, 1.0f, -1.0f,      // 6
            1.0f, 1.0f, 1.0f        // 7
        };

        const std::vector<uint32_t> kSkyboxIndices = {
            2, 0, 4, 4, 6, 2,
            1, 0, 2, 2, 3, 1,
            4, 5, 7, 7, 6, 4,
            1, 3, 7, 7, 5, 1,
            2, 6, 7, 7, 3, 2,
            0, 1, 4, 4, 1, 5
        };

    };
    typedef std::shared_ptr<Sky>	SkyPtr;
}

#endif // !__SKY_HPP_
