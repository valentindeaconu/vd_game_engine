#include "Terrain.hpp"

#include "TerrainShader.hpp"

namespace mod::terrain
{
    Terrain::Terrain(const vd::EnginePtr& enginePtr, const std::string& configFilePath)
        : Entity(enginePtr)
    {
        configPtr = std::make_shared<TerrainConfig>(configFilePath);
    }

    Terrain::~Terrain() = default;

    void Terrain::init()
    {
        configPtr->parse();

        configPtr->initializeObjects(getParentEngine());

        generatePatch(); // generate mesh;

        Entity::init(); // generate meshbuffer;
    }

    void Terrain::update()
    {
    }

    void Terrain::cleanUp()
    {
        Entity::cleanUp();
    }

    const TerrainConfigPtr& Terrain::getTerrainConfig() const
    {
        return configPtr;
    }

    void Terrain::generatePatch()
    {
        vd::model::MeshPtr meshPtr = std::make_shared<vd::model::Mesh>();

        const size_t size = configPtr->getSize();

        for (size_t i = 0; i <= size; ++i)
        {
            for (size_t j = 0; j <= size; ++j)
            {
                meshPtr->vertices.emplace_back();
                vd::model::Vertex& v = meshPtr->vertices.back();

                float x = ((float)i / size);
                float z = ((float)j / size);

                float y = configPtr->getHeight(j, i);

                v.Position = glm::vec3(i, y, j);
                v.TexCoords = glm::vec2(x, z);
            }
        }

        for (size_t i = 0; i <= size; ++i)
        {
            for (size_t j = 0; j <= size; ++j)
            {
                /*
                 * z0 -- z1 -- z2
                 * |     |     |
                 * z3 --  c -- z4
                 * |     |     |
                 * z5 -- z6 -- z7
                 */

                float z0 = configPtr->getHeight(j - 1, i - 1);
                float z1 = configPtr->getHeight(j, i - 1);
                float z2 = configPtr->getHeight(j + 1, i - 1);

                float z3 = configPtr->getHeight(j - 1, i);
                float z4 = configPtr->getHeight(j + 1, i);

                float z5 = configPtr->getHeight(j - 1, i + 1);
                float z6 = configPtr->getHeight(j, i + 1);
                float z7 = configPtr->getHeight(j + 1, i + 1);

                glm::vec3 normal(0.0f);

                normal.z = 1.0f / configPtr->getNormalStrength();
                normal.x = z0 + 2*z3 + z5 - z2 - 2*z4 - z7;
                normal.y = z0 + 2*z1 + z2 - z5 - 2*z6 - z7;

                meshPtr->vertices[i * size + j].Normal = (glm::normalize(normal) + 1.0f) / 2.0f;
            }
        }

        for (size_t i = 1; i <= size; ++i)
        {
            for (size_t j = 1; j <= size; ++j)
            {
                GLuint current = i * (size + 1) + j;
                GLuint west = current - 1;
                GLuint north = (i - 1) * (size + 1) + j;
                GLuint northWest = north - 1;

                meshPtr->indices.push_back(current);
                meshPtr->indices.push_back(west);
                meshPtr->indices.push_back(northWest);

                meshPtr->indices.push_back(current);
                meshPtr->indices.push_back(northWest);
                meshPtr->indices.push_back(north);
            }
        }

        getMeshes().push_back(meshPtr);
    }
}