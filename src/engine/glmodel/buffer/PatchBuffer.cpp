//
// Created by Vali on 9/21/2020.
//

#include "PatchBuffer.hpp"

namespace vd::buffer {
    PatchBuffer::PatchBuffer()
        : vaoId(0)
        , vbId(0)
        , size(0)
    {
    }

    PatchBuffer::~PatchBuffer() = default;

    void PatchBuffer::allocate(const model::MeshPtr& meshPtr) {
        size = meshPtr->vertices.size();

        std::vector<glm::vec2> patchVertices;
        patchVertices.reserve(meshPtr->vertices.size());

        for (auto& v : meshPtr->vertices) {
            patchVertices.emplace_back(v.Position.x, v.Position.z);
        }

        glGenVertexArrays(1, &vaoId);
        glGenBuffers(1, &vbId);

        glBindVertexArray(vaoId);

        glBindBuffer(GL_ARRAY_BUFFER, vbId);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec2), &patchVertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);
        glPatchParameteri(GL_PATCH_VERTICES, size);

        glBindVertexArray(0);
    }

    void PatchBuffer::render() {
        glBindVertexArray(vaoId);

        glDrawArrays(GL_PATCHES, 0, size);

        glBindVertexArray(0);
    }

    void PatchBuffer::cleanUp() {
        glBindVertexArray(vaoId);

        glDeleteBuffers(1, &vbId);
        glDeleteVertexArrays(1, &vaoId);

        glBindVertexArray(0);
    }
}

