//
// Created by Vali on 3/16/2021.
//

#ifndef VDGE_QUERY_HPP
#define VDGE_QUERY_HPP

#include <memory>
#include <unordered_map>

#include "GL.hpp"
#include "Texture.hpp"

#include <engine/service/TextureService.hpp>

namespace vd::gl {

    class Query {
    public:
        enum QueryType {
            eSamplesPassed = GL_SAMPLES_PASSED,
            eAnySamplesPassed = GL_ANY_SAMPLES_PASSED
        };

        explicit Query(const QueryType& type);

        void Create();
        void CleanUp();

        void Prepare();
        void Finish();

        void Start();
        void End();

        bool ResultReady();
        bool InUse();

        int GetResult();

    private:
        uint32_t    m_Id;
        bool        m_Allocated;
        bool        m_InUse;
        QueryType   m_Type;

    };
    typedef std::shared_ptr<Query>  QueryPtr;

}

#endif //VDGE_QUERY_HPP