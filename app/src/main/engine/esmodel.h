#pragma once
// Std. Includes
#include <string>
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>

#include "esutil.h"
#include "esfile.h"
#include "esmesh.h"
#include "esprogram.h"
#include "loader/stbloader.h"

class ESModel {
public:
    static const int AI_POST_PROCESSING_FLAGS = aiProcess_Triangulate | aiProcess_FlipUVs;

    /*  Functions   */
    // Constructor, expects a filepath to a 3D model.
    ESModel(ESContext *esContext, string path) {
        this->loadModel(esContext, path);
    }

    // Draws the model, and thus all its meshes
    void draw(ESProgram &prog) {
        for (GLuint i = 0; i < this->meshes.size(); i++)
            this->meshes[i].draw(prog);
    }

private:
    /*  Model Data */
    vector<ESTexture> textures_loaded;// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<ESMesh> meshes;
    string directory;

    aiMatrix4x4t<float> globalInverseTransform;

    /*  Functions   */
    // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(ESContext *esContext, string path) {
        // Read file via ASSIMP
        Assimp::Importer* importer = new Assimp::Importer();

        // http://assimp.sourceforge.net/lib_html/postprocess_8h.html
        const aiScene* scene = importer->ReadFile(path, AI_POST_PROCESSING_FLAGS);
        // Check for errors
        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE
                || !scene->mRootNode) {                     // if is Not Zero
            cout << "ERROR::ASSIMP:: " << importer->GetErrorString() << endl;
            return;
        }
        this->globalInverseTransform = scene->mRootNode->mTransformation.Inverse();
        // Retrieve the directory path of the filepath
        this->directory = path.substr(0, path.find_last_of('/'));

        // Process ASSIMP's root node recursively
        this->processNode(scene->mRootNode, scene);
    }

    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene) {
        // Process each mesh located at the current node
        for (GLuint i = 0; i < node->mNumMeshes; i++) {
            // The node object only contains indices to index the actual objects in the scene. 
            // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            this->meshes.push_back(this->processMesh(mesh, scene));
        }
        // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (GLuint i = 0; i < node->mNumChildren; i++) {
            this->processNode(node->mChildren[i], scene);
        }

    }

    ESMesh processMesh(aiMesh* mesh, const aiScene* scene) {
        // Data to fill
        vector<ESVertex> vertices;
        vector<GLuint> indices;
        vector<ESTexture> textures;
        vector<ESVertexBoneData> vertexBoneDatas;
        vector<ESBone> bones;

        unsigned int numVertices = mesh->mNumVertices;
        // init this vector specifically, where it empty or not, it will load in the memory.
        vertexBoneDatas.resize(numVertices);

        // Walk through each of the mesh's vertices
        for (GLuint i = 0; i < mesh->mNumVertices; i++) {
            ESVertex vertex;
            glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // Positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // Normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // Texture Coordinates
            if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            } else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            vertices.push_back(vertex);
        }
        // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (GLuint i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            // Retrieve all indices of the face and store them in the indices vector
            for (GLuint j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // Process materials
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
            // Same applies to other texture as the following list summarizes:
            // Diffuse: texture_diffuseN
            // Specular: texture_specularN
            // Normal: texture_normalN

            // 1. Diffuse maps
            vector<ESTexture> diffuseMaps = this->loadMaterialTextures(material,
                    aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(),
                    diffuseMaps.end());
            // 2. Specular maps
            vector<ESTexture> specularMaps = this->loadMaterialTextures(material,
                    aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(),
                    specularMaps.end());
        }

        loadBones(mesh, bones, vertexBoneDatas);

        // Return a mesh object created from the extracted mesh data
        return ESMesh(vertices, indices, textures, bones, vertexBoneDatas);
    }

    // Checks all material textures of a given type and loads the textures if they're not loaded yet.
    // The required info is returned as a Texture struct.
    vector<ESTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
            string typeName) {
        vector<ESTexture> textures;
        for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            GLboolean skip = false;
            for (GLuint j = 0; j < textures_loaded.size(); j++) {
                if (textures_loaded[j].path == str) {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip) {   // If texture hasn't been loaded already, load it
                ESTexture texture;

                string filename = str.C_Str();
                filename = directory + '/' + filename;

                //http://stackoverflow.com/a/16502000
                //string str = "some string" ;
                //char *cstr = &str[0u];
                texture.id =  STBLoader::loadTex( &filename[0u], STBLoader::CHANNEL_RGB,
                        STBLoader::FLAG_FOPEN_ABSOULT | STBLoader::FLAG_MIPMAPS);
                texture.type = typeName;
                texture.path = str;
                textures.push_back(texture);
                this->textures_loaded.push_back(texture); // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }

    void loadBones(const aiMesh* pMesh, vector<ESBone>& bones, vector<ESVertexBoneData>& vertexBoneDatas) {

        for (uint i = 0 ; i < pMesh->mNumBones ; i++) {
            uint boneIndex = 0;
            string boneName(pMesh->mBones[i]->mName.data);

            ESboolean bFind = FALSE;
            for(vector<ESBone>::size_type index = 0; index != bones.size(); index++) {
                if(bones[index].boneName == boneName) {
                    boneIndex = index;
                    bFind = TRUE;
                }
            }

            if (bFind == FALSE) {
                // Allocate an index for a new bone
                boneIndex = bones.size();
                ESBone bone;
                bone.boneName = boneName;
                bone.boneOffset = pMesh->mBones[i]->mOffsetMatrix;
                bones[boneIndex] = bone;
            }

            for (uint j = 0 ; j < pMesh->mBones[i]->mNumWeights ; j++) {
                uint vertexID = pMesh->mBones[i]->mWeights[j].mVertexId;
                float weight  = pMesh->mBones[i]->mWeights[j].mWeight;
                vertexBoneDatas[vertexID].addBoneData(boneIndex, weight);
            }
        }
    }

};
