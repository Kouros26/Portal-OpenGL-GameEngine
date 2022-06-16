#pragma once
#include <glad/glad.h> // holds all OpenGL type declarations
#include <string>
#include <vector>

#include "../Ressource/AssimpShader.h"
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    lm::vec3 Position;
    // normal
    lm::vec3 Normal;
    // texCoords
    lm::vec2 TexCoords;
    // tangent
    lm::vec3 Tangent;
    // bitangent
    lm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    std::vector<lm::vec3> colors;
    unsigned int VAO;

    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, std::vector<lm::vec3> colors)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->colors = colors;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw(Shader& shader, lm::mat4& VPMatrix, lm::mat4& modelMatrix, lm::vec3& camPos)
    {
        glUseProgram(shader.GetShaderProgram());
        int MVPLocation = glGetUniformLocation(shader.GetShaderProgram(), "MVP");
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &(VPMatrix * modelMatrix)[0][0]);
        int ModelLocation = glGetUniformLocation(shader.GetShaderProgram(), "model");
        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        int camPosLocation = glGetUniformLocation(shader.GetShaderProgram(), "viewPos");
        glUniform3fv(camPosLocation, 1, &camPos[0]);
        int shininessLocation = glGetUniformLocation(shader.GetShaderProgram(), "material.shininess");
        glUniform1f(shininessLocation, 256.0f);
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            std::string uniformName = ("material." + name + number);
            glUniform1i(glGetUniformLocation(shader.GetShaderProgram(), ("material." + name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        shader.setVec3("material.diffuse_color", colors[0]);
        shader.setVec3("material.specular_color", colors[1]);
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

    void Draw(Shader& shader, lm::mat4& modelMatrix, lm::vec3& camPos)
    {
        glUseProgram(shader.GetShaderProgram());
        int MVPLocation = glGetUniformLocation(shader.GetShaderProgram(), "MVP");
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        int ModelLocation = glGetUniformLocation(shader.GetShaderProgram(), "model");
        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        int camPosLocation = glGetUniformLocation(shader.GetShaderProgram(), "viewPos");
        glUniform3fv(camPosLocation, 1, &camPos[0]);
        int shininessLocation = glGetUniformLocation(shader.GetShaderProgram(), "material.shininess");
        glUniform1f(shininessLocation, 256.0f);
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            std::string uniformName = ("material." + name + number);
            glUniform1i(glGetUniformLocation(shader.GetShaderProgram(), ("material." + name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        shader.setVec3("material.diffuse_color", colors[0]);
        shader.setVec3("material.specular_color", colors[1]);
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        // ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
        // weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        glBindVertexArray(0);
    }
};