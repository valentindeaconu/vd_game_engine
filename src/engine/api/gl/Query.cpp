//
// Created by Vali on 3/16/2021.
//

#include "Query.hpp"

#define RT_Check()  { \
        if (!m_Allocated) { \
            throw vd::UnallocatedResourceError("using an unallocated QueryObject"); \
        } \
    }

namespace vd::gl {

    Query::Query(const Type& type)
        : m_Id(0)
        , m_Allocated(false)
        , m_InUse(false)
        , m_Type(type)
    {
    }

    void Query::Create() {
        if (!m_Allocated) {
            glGenQueries(1, &m_Id);
            m_Allocated = true;
        }
    }

    void Query::CleanUp() {
        if (m_Allocated) {
            glDeleteQueries(1, &m_Id);
            m_Allocated = false;
        }
    }

    void Query::Prepare() {
        glEnable(GL_DEPTH_TEST);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);
    }

    void Query::Finish() {
        glDisable(GL_DEPTH_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);
    }

    void Query::Start() {
        RT_Check();

        glBeginQuery(m_Type, m_Id);
        m_InUse = true;
    }

    void Query::End() {
        RT_Check();

        glEndQuery(m_Type);
    }

    bool Query::ResultReady() {
        RT_Check();

        if (!m_InUse) {
            return false;
        }

        int result = 0;
        glGetQueryObjectiv(m_Id, GL_QUERY_RESULT_AVAILABLE, &result);

        return result == GL_TRUE;
    }

    bool Query::InUse() {
        return m_InUse;
    }

    int Query::GetResult() {
        RT_Check();

        int result = 0;
        glGetQueryObjectiv(m_Id, GL_QUERY_RESULT, &result);
        m_InUse = false;

        return result;
    }

}