#pragma once
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "../LowRenderer/AssimpMesh.h"
#include "../Ressource/ResourcesManager.h"

class Model : public IResource
{
public:
    // model data 
    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh>    meshes;
    lm::mat4& VPMatrix;
    lm::vec3& camPos;
    std::string directory;
    bool gammaCorrection;
    std::string path;
    bool externalTexture = false;
    Texture* externalTexturePath = new Texture();

    // constructor, expects a filepath to a 3D model.
    Model(std::string path, Camera* cam, bool gamma = false) : gammaCorrection(gamma), VPMatrix(cam->GetVPMatrix()), camPos(cam->GetPosition())
    {        
        this->path = path;
        if (path == "Assets/Meshes\\portal_gun\\PortalGun.FBX")
        {
            externalTexture = true;
            externalTexturePath->path = "Assets/Meshes/portal_gun/gun.png";
            externalTexturePath->type = "texture_diffuse";
            externalTexturePath->id = TextureFromFile("gun.png", "Assets/Meshes/portal_gun");
        }
        else if (path == "BluePortal")
        {
            externalTexture = true;
            externalTexturePath->path = "Assets/Textures/BluePortalConnected.png";
            externalTexturePath->type = "texture_diffuse";
            externalTexturePath->id = TextureFromFile("BluePortalConnected.png", "Assets/Textures");
            path = "Assets/Meshes\\Quad.obj";
        }
        else if (path == "OrangePortal")
        {
            externalTexture = true;
            externalTexturePath->path = "Assets/Textures/OrangePortalConnected.png";
            externalTexturePath->type = "texture_diffuse";
            externalTexturePath->id = TextureFromFile("OrangePortalConnected.png", "Assets/Textures");
            path = "Assets/Meshes\\Quad.obj";
        }
        loadModel(path);
        
    }

    static unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

    // draws the model, and thus all its meshes
    void Draw(lm::mat4& modelMatrix, Shader& shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader, VPMatrix, modelMatrix, camPos);
    }

    void Draw(lm::mat4& modelMatrix, Shader& shader, bool usingDifferentShader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader, modelMatrix, camPos);
    }

    lm::mat4& GetVPMatrix()
    {
        return VPMatrix;
    }

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('\\'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        std::vector<lm::vec3> colors;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            lm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.X() = mesh->mVertices[i].x;
            vector.Y() = mesh->mVertices[i].y;
            vector.Z() = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.X() = mesh->mNormals[i].x;
                vector.Y() = mesh->mNormals[i].y;
                vector.Z() = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                lm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.X() = mesh->mTextureCoords[0][i].x;
                vec.Y() = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.X() = mesh->mTangents[i].x;
                vector.Y() = mesh->mTangents[i].y;
                vector.Z() = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.X() = mesh->mBitangents[i].x;
                vector.Y() = mesh->mBitangents[i].y;
                vector.Z() = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = lm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        if (externalTexture)
        {
            textures.push_back(*externalTexturePath);
        }
        else
        {
            // 1. diffuse maps
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            // 4. height maps
            std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        }
        aiColor4D diffuse;
        aiColor4D specular;
        
        if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse) && textures.size() == 0)
            colors.emplace_back(lm::vec3(diffuse.r, diffuse.g, diffuse.b));
        else
            colors.emplace_back(lm::vec3(0, 0, 0));
        if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular) && textures.size() == 0)
            colors.emplace_back(lm::vec3(specular.r, specular.g, specular.b));
        else
            colors.emplace_back(lm::vec3(0, 0, 0));

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures, colors);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }
};