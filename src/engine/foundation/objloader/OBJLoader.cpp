#include "OBJLoader.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace vd::objloader
{
    OBJLoader::OBJLoader()
    {
    }

    OBJLoader::~OBJLoader()
    {
    }

    void OBJLoader::load(const std::string& basePath, const std::string& objFile, vd::model::MeshPtrVec& meshes)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string fullPath = basePath + "/";
        std::string fullObjPath = fullPath + objFile;

        uint32_t materialId;

        std::string err;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fullObjPath.c_str(), fullPath.c_str(), GL_TRUE);

        if (!err.empty())
        {
            std::cerr << err << "\n";
        }

        if (!ret)
        {
            exit(1);
        }

        std::stringstream ss;
        ss << "[" << objFile << "] " << shapes.size() << " shape(s), " << materials.size() << " material(s)\n";
        std::cout << ss.str();

        meshes.reserve(shapes.size());
        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++)
        {
            vd::model::MeshPtr meshPtr = std::make_shared<vd::model::Mesh>();

            // Loop over faces
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
            {
                int fv = shapes[s].mesh.num_face_vertices[f];

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++)
                {
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                    float vx = attrib.vertices[3 * idx.vertex_index + 0];
                    float vy = attrib.vertices[3 * idx.vertex_index + 1];
                    float vz = attrib.vertices[3 * idx.vertex_index + 2];

                    float nx = attrib.normals[3 * idx.normal_index + 0];
                    float ny = attrib.normals[3 * idx.normal_index + 1];
                    float nz = attrib.normals[3 * idx.normal_index + 2];

                    float tx = 0.0f;
                    float ty = 0.0f;

                    if (idx.texcoord_index != -1) {
                        tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                        ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                    }

                    glm::vec3 vertexPosition(vx, vy, vz);
                    glm::vec3 vertexNormal(nx, ny, nz);
                    glm::vec2 vertexTexCoords(tx, ty);

                    vd::model::Vertex currentVertex;

                    currentVertex.Position = vertexPosition;
                    currentVertex.Normal = vertexNormal;
                    currentVertex.TexCoords = vertexTexCoords;

                    meshPtr->vertices.push_back(currentVertex);

                    meshPtr->indices.push_back((GLuint)index_offset + v);
                }

                index_offset += fv;
            }

            // get material id
            if (shapes[s].mesh.material_ids.size() > 0 && materials.size() > 0)
            {
                materialId = shapes[s].mesh.material_ids[0];
                if (materialId != -1)
                {
                    meshPtr->materials.push_back(vd::model::Material());

                    meshPtr->materials.back().name = materials[materialId].name;

                    meshPtr->materials.back().emission = glm::vec3(
                        materials[materialId].emission[0],
                        materials[materialId].emission[1],
                        materials[materialId].emission[2]);

                    meshPtr->materials.back().shininess = materials[materialId].shininess;

                    vd::model::TextureService& textureService = vd::model::TextureService::getInstance();

                    //ambient texture
                    std::string ambientTexturePath = materials[materialId].ambient_texname;
                    if (!ambientTexturePath.empty())
                    {
                        vd::model::Texture2DPtr currentTexture = textureService.get(fullPath + ambientTexturePath);
                        currentTexture->bind();
                        currentTexture->wrapRepeat();
                        currentTexture->trilinearFilter();
                        meshPtr->materials.back().ambientmap = currentTexture;
                    }

                    //diffuse texture
                    std::string diffuseTexturePath = materials[materialId].diffuse_texname;
                    if (!diffuseTexturePath.empty())
                    {
                        vd::model::Texture2DPtr currentTexture = textureService.get(fullPath + diffuseTexturePath);
                        currentTexture->bind();
                        currentTexture->wrapRepeat();
                        currentTexture->trilinearFilter();
                        meshPtr->materials.back().diffusemap = currentTexture;
                    }

                    //specular texture
                    std::string specularTexturePath = materials[materialId].specular_texname;
                    if (!specularTexturePath.empty())
                    {
                        vd::model::Texture2DPtr currentTexture = textureService.get(fullPath + specularTexturePath);
                        currentTexture->bind();
                        currentTexture->wrapRepeat();
                        currentTexture->trilinearFilter();
                        meshPtr->materials.back().specularmap = currentTexture;
                    }

                    //normal texture
                    std::string normalTexturePath = materials[materialId].normal_texname;
                    if (!normalTexturePath.empty())
                    {
                        vd::model::Texture2DPtr currentTexture = textureService.get(fullPath + normalTexturePath);
                        currentTexture->bind();
                        currentTexture->wrapRepeat();
                        currentTexture->trilinearFilter();
                        meshPtr->materials.back().normalmap = currentTexture;
                    }

                    //displacement texture
                    std::string displaceTexturePath = materials[materialId].displacement_texname;
                    if (!displaceTexturePath.empty())
                    {
                        vd::model::Texture2DPtr currentTexture = textureService.get(fullPath + displaceTexturePath);
                        currentTexture->bind();
                        currentTexture->wrapRepeat();
                        currentTexture->trilinearFilter();
                        meshPtr->materials.back().displacemap = currentTexture;
                    }

                    //alpha texture
                    std::string alphaTexturePath = materials[materialId].alpha_texname;
                    if (!alphaTexturePath.empty())
                    {
                        vd::model::Texture2DPtr currentTexture = textureService.get(fullPath + alphaTexturePath);
                        currentTexture->bind();
                        currentTexture->wrapRepeat();
                        currentTexture->trilinearFilter();
                        meshPtr->materials.back().alphamap = currentTexture;
                    }

                    //bump texture
                    std::string bumpTexturePath = materials[materialId].bump_texname;
                    if (!bumpTexturePath.empty())
                    {
                        vd::model::Texture2DPtr currentTexture = textureService.get(fullPath + bumpTexturePath);
                        currentTexture->bind();
                        currentTexture->wrapRepeat();
                        currentTexture->trilinearFilter();
                        meshPtr->materials.back().bumpmap = currentTexture;
                    }
                }
            }

            meshes.push_back(meshPtr);
        }
    }
}