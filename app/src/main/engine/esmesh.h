#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <assert.h>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec2;

#include <assimp/types.h>
#include <assimp/matrix4x4.h>

#include "esdef.h"
#include "esutil.h"      // Contains all the necessary OpenGL includes
#include "esprogram.h"

struct ESVertex {
    // Position
    vec3 Position;
    // Normal
    vec3 Normal;
    // TexCoords
    vec2 TexCoords;
};

struct ESTexture {
    GLuint id;
    string type;
    aiString path;
};

struct ESBone {
    string boneName;
    aiMatrix4x4t<float> boneOffset;
    aiMatrix4x4t<float> finalTransformation;
};

struct ESVertexBoneData {
    uint ids[ES_NUM_BONES_PER_VEREX];
    float weights[ES_NUM_BONES_PER_VEREX];

    ESVertexBoneData() {
        reset();
    };

    void addBoneData(uint boneID, float weight) {
        for (uint i = 0 ; i < ES_ARRAY_SIZE_IN_ELEMENTS(ids) ; i++) {
            if (weights[i] == 0.0) {
                ids[i]     = boneID;
                weights[i] = weight;
                return;
            }
        }

        // should never get here - more bones than we have space for
        assert(0);
    }

    void reset() {
        ES_ZERO_MEM(ids);
        ES_ZERO_MEM(weights);
    }
};

class ESMesh {
public:

    static const GLuint POSITION_LOCATION    = 0;
    static const GLuint NORMAL_LOCATION      = 1;
    static const GLuint TEX_COORD_LOCATION   = 2;
    static const GLuint BONE_ID_LOCATION     = 3;
    static const GLuint BONE_WEIGHT_LOCATION = 4;

    /*  Mesh Data  */
    vector<ESVertex> vertices;
    vector<GLuint> indices;
    vector<ESTexture> textures;
    vector<ESBone> bones;
    vector<ESVertexBoneData> vertexBoneDatas;

    GLuint VAO;

    /*  Functions  */
    // Constructor
    ESMesh(vector<ESVertex> vertices, vector<GLuint> indices, vector<ESTexture> textures,
            vector<ESBone> bones, vector<ESVertexBoneData> vertexBoneDatas) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->bones = bones;
        this->vertexBoneDatas = vertexBoneDatas;

        // Now that we have all the required data, set the vertex buffers and its attribute pointers.
        this->setupMesh();
    }

    // Render the mesh
    void draw(ESProgram &prog) {
        // Bind appropriate textures
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;
        for (GLuint i = 0; i < this->textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
            // Retrieve texture number (the N in diffuse_textureN)
            stringstream ss;
            string number;
            string name = this->textures[i].type;
            if (name == "texture_diffuse")
                ss << diffuseNr++; // Transfer GLuint to stream
            else if (name == "texture_specular")
                ss << specularNr++; // Transfer GLuint to stream
            number = ss.str();
            // Now set the sampler to the correct texture unit
            prog.setUniform((name + number).c_str(), i);
            // And finally bind the texture
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        }

        // Draw mesh
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Always good practice to set everything back to defaults once configured.
        for (GLuint i = 0; i < this->textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

private:
    /*  Render data  */
    GLuint vboVertex, vboVertexBoneData, eboIndex;

    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void setupMesh() {
        // Create buffers/arrays
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->vboVertex);
        glGenBuffers(1, &this->vboVertexBoneData);
        glGenBuffers(1, &this->eboIndex);

        glBindVertexArray(this->VAO);
        // Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, this->vboVertex);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(ESVertex),
                &this->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboIndex);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
                &this->indices[0], GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(POSITION_LOCATION);
        glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(ESVertex), (GLvoid*) 0);
        // Vertex Normals
        glEnableVertexAttribArray(NORMAL_LOCATION);
        glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(ESVertex),
                (GLvoid*) offsetof(ESVertex, Normal));
        // Vertex Texture Coords
        glEnableVertexAttribArray(TEX_COORD_LOCATION);
        glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(ESVertex),
                (GLvoid*) offsetof(ESVertex, TexCoords));

        // Set the vertex bone data
        glBindBuffer(GL_ARRAY_BUFFER, this->vboVertexBoneData);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBoneDatas[0]) * vertexBoneDatas.size(), &vertexBoneDatas[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(BONE_ID_LOCATION);
        glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(ESVertexBoneData), (const GLvoid*)0);
        glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
        glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(ESVertexBoneData), (const GLvoid*)16);


        glBindVertexArray(0);
    }
};

