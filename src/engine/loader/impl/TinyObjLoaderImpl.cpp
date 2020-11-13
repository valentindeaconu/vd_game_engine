//
// Created by Vali on 11/13/2020.
//

#include "TinyObjLoaderImpl.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace vd::loader::impl {
    vd::model::MeshPtrVec TinyObjLoaderImpl::Load(const std::string &path) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::size_t found = path.find_last_of("/\\");

        // dirname + '/' or '\\'
        const std::string dirname = path.substr(0, found + 1);

        // basename
        const std::string basename = path.substr(found + 1);

        uint32_t materialId;

        std::string err;
        std::string warn;
        bool ret = tinyobj::LoadObj(&attrib,
                                    &shapes,
                                    &materials,
                                    &warn,
                                    &err,
                                    path.c_str(),
                                    dirname.c_str(),
                                    GL_TRUE);

        if (!warn.empty()) {
            vd::Logger::warn(warn);
        }

        if (!ret) {
            vd::Logger::terminate(err, 1);
        }

        vd::Logger::log("[" + basename + "] " + std::to_string(shapes.size()) + " shape(s), " + std::to_string(materials.size()) + " material(s)");

        vd::model::MeshPtrVec meshes;
        meshes.reserve(shapes.size());

        // Loop over shapes
        for (auto & shape : shapes) {
            vd::model::MeshPtr meshPtr = std::make_shared<vd::model::Mesh>();

            // Loop over faces
            size_t index_offset = 0;
            for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
                int fv = shape.mesh.num_face_vertices[f];

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    // access to vertex
                    tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

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

                    meshPtr->vertices.emplace_back(vertexPosition, vertexNormal, vertexTexCoords);

                    meshPtr->indices.push_back((GLuint)index_offset + v);
                }

                index_offset += fv;
            }

            // get material id
            if (!shape.mesh.material_ids.empty() && !materials.empty()) {
                materialId = shape.mesh.material_ids[0];
                if (materialId != -1) {
                    meshPtr->materials.emplace_back();

                    meshPtr->materials.back().name = materials[materialId].name;

                    meshPtr->materials.back().emission = glm::vec3(materials[materialId].emission[0],
                                                                   materials[materialId].emission[1],
                                                                   materials[materialId].emission[2]);

                    meshPtr->materials.back().shininess = materials[materialId].shininess;

                    //ambient texture
                    std::string ambientTexturePath = materials[materialId].ambient_texname;
                    if (!ambientTexturePath.empty()) {
                        vd::gl::Texture2DPtr currentTexture = vd::gl::TextureService::Get(dirname + ambientTexturePath);
                        currentTexture->Bind();
                        currentTexture->WrapRepeat();
                        currentTexture->TrilinearFilter();
                        meshPtr->materials.back().ambientMap = currentTexture;
                    }

                    //diffuse texture
                    std::string diffuseTexturePath = materials[materialId].diffuse_texname;
                    if (!diffuseTexturePath.empty()) {
                        vd::gl::Texture2DPtr currentTexture = vd::gl::TextureService::Get(dirname + diffuseTexturePath);
                        currentTexture->Bind();
                        currentTexture->WrapRepeat();
                        currentTexture->TrilinearFilter();
                        meshPtr->materials.back().diffuseMap = currentTexture;
                    }

                    //specular texture
                    std::string specularTexturePath = materials[materialId].specular_texname;
                    if (!specularTexturePath.empty()) {
                        vd::gl::Texture2DPtr currentTexture = vd::gl::TextureService::Get(dirname + specularTexturePath);
                        currentTexture->Bind();
                        currentTexture->WrapRepeat();
                        currentTexture->TrilinearFilter();
                        meshPtr->materials.back().specularMap = currentTexture;
                    }

                    //normal texture
                    std::string normalTexturePath = materials[materialId].normal_texname;
                    if (!normalTexturePath.empty()) {
                        vd::gl::Texture2DPtr currentTexture = vd::gl::TextureService::Get(dirname + normalTexturePath);
                        currentTexture->Bind();
                        currentTexture->WrapRepeat();
                        currentTexture->TrilinearFilter();
                        meshPtr->materials.back().normalMap = currentTexture;
                    }

                    //displacement texture
                    std::string displaceTexturePath = materials[materialId].displacement_texname;
                    if (!displaceTexturePath.empty()) {
                        vd::gl::Texture2DPtr currentTexture = vd::gl::TextureService::Get(dirname + displaceTexturePath);
                        currentTexture->Bind();
                        currentTexture->WrapRepeat();
                        currentTexture->TrilinearFilter();
                        meshPtr->materials.back().displaceMap = currentTexture;
                    }

                    //alpha texture
                    std::string alphaTexturePath = materials[materialId].alpha_texname;
                    if (!alphaTexturePath.empty()) {
                        vd::gl::Texture2DPtr currentTexture = vd::gl::TextureService::Get(dirname + alphaTexturePath);
                        currentTexture->Bind();
                        currentTexture->WrapRepeat();
                        currentTexture->TrilinearFilter();
                        meshPtr->materials.back().alphaMap = currentTexture;
                    }

                    //bump texture
                    std::string bumpTexturePath = materials[materialId].bump_texname;
                    if (!bumpTexturePath.empty()) {
                        vd::gl::Texture2DPtr currentTexture = vd::gl::TextureService::Get(dirname + bumpTexturePath);
                        currentTexture->Bind();
                        currentTexture->WrapRepeat();
                        currentTexture->TrilinearFilter();
                        meshPtr->materials.back().bumpMap = currentTexture;
                    }
                }
            }

            meshes.push_back(meshPtr);
        }

        return meshes;
    }
}