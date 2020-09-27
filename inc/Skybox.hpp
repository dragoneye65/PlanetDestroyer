/*
 * Copyright (C) 2020 Dragoneye
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
#include <vector>
#include <GL/glew.h>
#include <stb_image.h>

#include "Shader.hpp"
#include "Camera.hpp"

class SkyBox {
public:
    void RenderSkyBox();
    void LoadCubemap(std::vector<const GLchar * > faces, GLuint InternalFormat = GL_RGBA);

private:
    GLuint cubemapTexture;
    GLuint skyboxVAO, skyboxVBO;

    GLuint internalFormat{GL_RGBA};
};