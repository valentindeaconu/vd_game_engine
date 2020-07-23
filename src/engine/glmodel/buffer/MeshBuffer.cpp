#include "MeshBuffer.hpp"

namespace vd::buffer
{
    MeshBuffer::MeshBuffer()
    {
    }

    MeshBuffer::~MeshBuffer()
    {
    }

    void MeshBuffer::allocate(const vd::model::MeshPtr& meshPtr)
    {
        this->meshPtr = meshPtr;

        // Create buffers/arrays
        glGenVertexArrays(1, &vaoId);
        glGenBuffers(1, &vbId);
        glGenBuffers(1, &ebId);

        glBindVertexArray(vaoId);

        // Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, vbId);
        glBufferData(GL_ARRAY_BUFFER, meshPtr->vertices.size() * sizeof(vd::model::Vertex), &meshPtr->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebId);
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

    void MeshBuffer::render()
    {
        std::vector<GLuint>& indices = meshPtr->indices;

        glBindVertexArray(vaoId);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void MeshBuffer::cleanUp()
    {
        glBindVertexArray(vaoId);

        glDeleteBuffers(1, &vbId);
        glDeleteBuffers(1, &ebId);
        glDeleteVertexArrays(1, &vaoId);

        glBindVertexArray(0);
    }
}