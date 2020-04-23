/*
 * Copyright (C) 2020 John Severin Flood aka Dragoneye
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>

#include "Shader.hpp"
#include "Mesh.hpp"


unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model
{
public:
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;
    bool gammaCorrection;

   Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        this->loadModel( path);
    }

    void Draw( Shader &shader);

    glm::vec3 GetMinValue() { return minValue; }
    glm::vec3 GetMaxValue() { return maxValue; }

private:
    glm::vec3 minValue{10000.0f};
    glm::vec3 maxValue{-10000.0f};


    void loadModel( string const &path);
    void processNode( aiNode *node, const aiScene *scene);
    Mesh processMesh( aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures( aiMaterial *mat, aiTextureType type, string typeName );

};


