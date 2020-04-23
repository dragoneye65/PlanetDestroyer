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



#include <iostream>
#include <string>
#include <unordered_map>

#ifdef _WIN32
    #include <SDL2/include/SDL.h>
#else
    #include <SDL2/SDL.h>
#endif

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <SOIL/SOIL.h>
#include <vector>
#include "Controller.hpp"

#include "Shader.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include "Skybox.hpp"
#include "Collision.hpp"


#define SDL_WINDOW_FLAG SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE


struct PlayerTRS
{
    glm::vec3 translate{0.0f};
    glm::vec3 rotate{0.0f};
    glm::vec3 scale{1.0f,1.0f,1.0f};
    glm::vec3 cameraPos{0.0f, 1.75f, 0.0f};

    float walkingMovementY{0.0f};
    bool jumpOrder{false};  // Does he have a jump order?
    bool isJumping{false};  // check original y translation to reset
    GLfloat jumpForce{20.0f};
    GLfloat gravity{10.0f};
    GLfloat initialGroundLevel{0.0f}; // this is where he started jumping from
    GLfloat currentJumpVelocity{0.0f};
};


class Game {
public:
    Game() {}
    ~Game() {}

    int InitSDL(std::string title, int width, int height);
    void InitControllers();
    void InitData();
    bool InitGL();
    void InitCamera();

    // renderer
    void Clear( glm::vec4 col);
    void Render();
    void RenderModels();

    void Update();
    int  Run();
    void CleanUp();
    void InitObject();
    void HandleEvents();
    void Timing();
    void SetSpawnPoint( const glm::vec3& SpawnPoint) {
        spawnPoint = SpawnPoint;
    }
    glm::vec3 GetSpawnPoint( ) {
        return spawnPoint;
    }
    void toggleLineMode();


    void PlotObjectsOnCompass(float direction,
        const glm::vec3 scale = glm::vec3(0.4f, 0.4f, 0.4f),
        const glm::vec3 pos = glm::vec3( 1.0f, -1.0f, 0.0f),
        const float showRange = 2000.0f);

    void DrawCompass( float direction,
        glm::vec3 scale = glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3 pos = glm::vec3( 1.0f, -1.0f, 0.0f));

    bool GetRenderCollisionBoxes() { return renderCollisionBoxes; }
    void InitSystemObjects();
    void InitGameObjects();
    void InitHUDObjects();

    void ReSpawnGameObjects();


private:
    vector<GameObject> systemObjects;
    vector<GameObject> gameObjects;
    vector<GameObject> hudObjects;

    GameObject* player{nullptr}; // pointer to the player object
    glm::vec3 spawnPoint{0.0f};
    bool drawLineMode_enable{false};

    std::unordered_map<std::string, Shader> shaders;
    std::unordered_map<std::string, Model> systemModels;
    std::unordered_map<std::string, Model> gameModels;
    std::unordered_map<std::string, Model> hudModels;

    GameObject obj; // just a temperary placeholder to cache object data.
    std::unordered_map<std::string, Shader>::iterator shaderItr;
    std::unordered_map<std::string, Model>::iterator modelItr;

    SkyBox skybox;

    Collision collision;

    bool renderCollisionBoxes{false};

    // Timeing stuff
//    std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds> tp1;
//    std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds> tp2;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp1;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp2;
    int	nFrameCount = 0;
	float fFrameTimer = 1.0f;
    float dt{0.f};

    float m_WMVelY{100.0f};

    std::string titleHeader;

    SDL_Window* sdlWindow;
    SDL_GLContext sdlGLContext{nullptr};

    SDL_Renderer* displayRenderer;
    SDL_RendererInfo displayRendererInfo;


    glm::mat4 modelMatrix{1.f};

    PlayerTRS playerTRS;
};