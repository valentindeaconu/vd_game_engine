//
// Created by Vali on 9/21/2020.
//

#include "PatchBuffer.hpp"

namespace vd::gl {
    PatchBuffer::PatchBuffer()
        : m_VaoId(0)
        , m_VboId(0)
        , m_Size(0)
    {
    }

    PatchBuffer::~PatchBuffer() = default;

    void PatchBuffer::Allocate(const vd::model::MeshPtr& meshPtr) {
        m_Size = meshPtr->vertices.size();

        std::vector<glm::vec2> patchVertices;
        patchVertices.reserve(meshPtr->vertices.size());

        for (auto& v : meshPtr->vertices) {
            patchVertices.emplace_back(v.Position.x, v.Position.z);
        }

        glGenVertexArrays(1, &m_VaoId);
        glGenBuffers(1, &m_VboId);

        glBindVertexArray(m_VaoId);

        glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
        glBufferData(GL_ARRAY_BUFFER, m_Size * sizeof(glm::vec2), &patchVertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);
        glPatchParameteri(GL_PATCH_VERTICES, m_Size);

        glBindVertexArray(0);
    }

    void PatchBuffer::Render() {
        glBindVertexArray(m_VaoId);

        glDrawArrays(GL_PATCHES, 0, m_Size);

        glBindVertexArray(0);
    }

    void PatchBuffer::CleanUp() {
        glBindVertexArray(m_VaoId);

        glDeleteBuffers(1, &m_VboId);
        glDeleteVertexArrays(1, &m_VaoId);

        glBindVertexArray(0);
    }
}

