#include "MeshBuffer.hpp"

namespace vd::buffer {
    MeshBuffer::MeshBuffer()
        : m_VaoId(0)
        , m_VboId(0)
        , m_EboId(0)
    {
    };

    MeshBuffer::~MeshBuffer() = default;

    void MeshBuffer::Allocate(const vd::model::MeshPtr& meshPtr) {
        this->m_MeshPtr = meshPtr;

        // Create buffers/arrays
        glGenVertexArrays(1, &m_VaoId);
        glGenBuffers(1, &m_VboId);
        glGenBuffers(1, &m_EboId);

        glBindVertexArray(m_VaoId);

        // Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
        glBufferData(GL_ARRAY_BUFFER, meshPtr->vertices.size() * sizeof(vd::model::Vertex), &meshPtr->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshPtr->indices.size() * sizeof(GLuint), &meshPtr->indices[0], GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vd::model::Vertex), (GLvoid*)0);

        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vd::model::Vertex), (GLvoid*)offsetof(vd::model::Vertex, Normal));

        // Vertex Texture Coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vd::model::Vertex), (GLvoid*)offsetof(vd::model::Vertex, TexCoords));

        glBindVertexArray(0);
    }

    void MeshBuffer::Render()
    {
        std::vector<GLuint>& indices = m_MeshPtr->indices;

        glBindVertexArray(m_VaoId);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void MeshBuffer::CleanUp()
    {
        glBindVertexArray(m_VaoId);

        glDeleteBuffers(1, &m_VboId);
        glDeleteBuffers(1, &m_EboId);
        glDeleteVertexArrays(1, &m_VaoId);

        glBindVertexArray(0);
    }
}