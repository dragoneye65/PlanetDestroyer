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


#include <random>
#include <ctime>

#include "Game.hpp"

#include "Shader.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "Events.hpp"
#include "Globals.hpp"

#include <glm/gtx/vector_angle.hpp>

extern Camera camera;
extern Events events;
extern Globals globals;


int Game::InitSDL(std::string title, int width, int height) {
    titleHeader = title;
    globals.screenheight = height;
    globals.screenwidth = width;
// SDL_INIT_EVERYTHING |
    if(SDL_Init( SDL_WINDOW_RESIZABLE) != 0 ) {
        std::cout << "Error in Game::Init->SDL_Init" << std::endl;
        return false;
    }


    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FLAG, &sdlWindow, &displayRenderer);
    SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

    sdlGLContext = SDL_GL_CreateContext(sdlWindow);
    if (sdlGLContext == nullptr) {
        std::cout << "Error in Game::Init->SDL_GL_CreateContext" <<
        SDL_GetError() << std::endl;
        return false;
    }

    if (SDL_GL_SetSwapInterval(0) <0) {
        std::cout << "Error in Game::Init->SDL_GL_SetSwapInterval < 0" << std::endl;
        return false;
    }

    glewExperimental = GL_TRUE;
    glewInit();

    return true;
}

void Game::InitControllers()
{
	int joyChoosen = false;

	// Controller controller;
	if (!events.m_controller.Init())
	{
		std::cout << "  No controllers found" << std::endl;
	}
	else
	{
		std::cout << "  Controllers:" << std::endl;
		for (auto gc : events.m_controller.m_gameControllers)
		{
			std::cout << "   Index: " << gc.index;
			std::cout << " Name: '" << gc.name;
			std::cout << "' Type: ";
			if (gc.type == JOYSTICK)
			{
				std::cout << "Joystick";
				// pick first joystick
				if (!joyChoosen)
				{
					joyChoosen = true;
					events.m_controller.m_joystick = SDL_JoystickOpen(gc.index);
					std::cout << " (enabled)";
				}
			}
			else
			{
				std::cout << "  Controller";
			}
			std::cout << " Buttons: " << gc.buttons;
			std::cout << " Axis: " << gc.axis;
			std::cout << " Hats: " << gc.hats;
			std::cout << " Balls: " << gc.balls;
			std::cout << "\n";
		}
	}

	if (SDL_JoystickGetAttached(events.m_controller.m_joystick))
	{
		std::cout << "  Joy got attached ID: " << SDL_JoystickInstanceID(events.m_controller.m_joystick) << std::endl;
	}

}

void Game::InitData() {
    std::cout << "Initializing data...";

    // Setup and compile our shaders
    std::cout << "Shaders...";
    shaders.insert( std::make_pair( std::string("model"), Shader( "res/shaders/model/modelLoading.vert","res/shaders/model/modelLoading.frag")) ) ;
    shaders.insert( std::make_pair( std::string("orthomodel"), Shader( "res/shaders/model/modelLoadingOrtho.vert","res/shaders/model/modelLoadingOrtho.frag")) ) ;
    shaders.insert( std::make_pair( std::string("skybox"), Shader( "res/shaders/cubemap/skybox.vert","res/shaders/cubemap/skybox.frag")) ) ;


    std::cout << "Loading Models...";

    // System objects
    systemModels.insert( std::make_pair("collisionbox",Model( const_cast<char *>( "res/models/box/box.obj"))) );
    systemModels.insert( std::make_pair("sphere",Model( const_cast<char *>( "res/models/sphere/sphere.obj"))) );

    // Game objects
    gameModels.insert( std::make_pair("player",Model( const_cast<char *>( "res/models/humanref/humanref.obj"))) );
    gameModels.insert( std::make_pair("sphere",Model( const_cast<char *>( "res/models/sphere/sphere.obj"))) );

    // HUD objects
    hudModels.insert( std::make_pair("compass",Model( const_cast<char *>( "res/models/compass/compass.obj"))) );

    std::cout << "ok\n";


    SetSpawnPoint( glm::vec3(9.0f, 1.65f, 10.0f));
    camera.SetPosition( GetSpawnPoint());

    InitSystemObjects();
    InitGameObjects();
    InitHUDObjects();

    // initialise the players config,   this will evaporate at some point....
    playerTRS.translate = camera.GetPosition();         // Translate
    playerTRS.rotate = glm::vec3(0.0f, 0.0f, 0.0f);     // Rotate
    playerTRS.scale = glm::vec3(1.0f, 1.0f, 1.0f);      // scale
    playerTRS.cameraPos = glm::vec3(0.0f, 1.65f, 0.0f); // camera location on player (eyes)
    playerTRS.walkingMovementY = 0.0f;                  // Walking movement added to Translate .y
    playerTRS.jumpOrder = false;                        // Jump order
    playerTRS.isJumping = false;                        // check original y translation to reset
    playerTRS.jumpForce = 10.0f;                        // jumpForce
    playerTRS.gravity = 6.0f;                           // gravity
    playerTRS.initialGroundLevel = 0.0f;                // initial Ground Level ( calculated at jump time )
    playerTRS.currentJumpVelocity = 0.0f;               // current Jump Velocity ( calculated at jump time )

    std::cout << "ok\n";

    // Load textures for the skybox
    std::vector<const GLchar*> faces;

    /* https://www.reddit.com/r/gamedev/comments/3ys4vg/a_month_ago_i_released_a_free_space_skybox/
    Planet
    Link: http://wwwtyro.github.io/planet-3d/
    Github: https://github.com/wwwtyro/planet-3d
    Screenshots: http://imgur.com/a/kHIJC

    Space
    Link: http://wwwtyro.github.io/space-3d
    Github: https://github.com/wwwtyro/space-3d
    Screenshots: http://imgur.com/a/Upzuv
    */

    faces.emplace_back( "res/images/skybox/stars/right.png"  );
    faces.emplace_back( "res/images/skybox/stars/left.png"   );
    faces.emplace_back( "res/images/skybox/stars/top.png"    );
    faces.emplace_back( "res/images/skybox/stars/bottom.png" );
    faces.emplace_back( "res/images/skybox/stars/back.png"   );
    faces.emplace_back( "res/images/skybox/stars/front.png"  );

    skybox.LoadCubemap( faces, GL_RGBA);
}


// Initialize the system objects
void Game::InitSystemObjects() {
    // set the default shader for the models
    auto myShader = shaders.find( "model");
    if ( myShader  == shaders.end()) {
        std::cout << "Could not find shader model" << endl;
    }

    GameObject obj;
    for (auto mItr = systemModels.begin(); mItr != systemModels.end(); ++mItr) {
        obj.SetViewMatrix( camera.GetViewMatrix());
        obj.SetProjectionMatrix(globals.projectionMatrix);
        obj.SetCenter( glm::vec3(
            abs( mItr->second.GetMaxValue().x - mItr->second.GetMinValue().x)/2,
            abs( mItr->second.GetMaxValue().y - mItr->second.GetMinValue().y)/2,
            abs( mItr->second.GetMaxValue().z - mItr->second.GetMinValue().z)/2
            ) );

        obj.SetColliderBoxDimentions( glm::vec3(
             mItr->second.GetMaxValue().x - mItr->second.GetMinValue().x,
             mItr->second.GetMaxValue().y - mItr->second.GetMinValue().y,
             mItr->second.GetMaxValue().z - mItr->second.GetMinValue().z
            ) );
        obj.SetPosition(glm::vec3( 0.0f, 0.0f, 0.0f));
        obj.SetRenderable(false);
        if ( mItr->first == "ground_small") {
            obj.SetRenderable(false);
            std::cout << "Playground: (" << obj.GetColliderBoxDimentions().x << "," << obj.GetColliderBoxDimentions().y <<"," << obj.GetColliderBoxDimentions().z << ")\n";
        }
        obj.SetModel( &mItr->second);
        obj.SetShader( &myShader->second);
        obj.SetCollider(false);
        obj.DetachCamera();

        systemObjects.push_back(obj);
    }

}

float floatrand()
{
    static std::mt19937 rng_engine( (unsigned)time(nullptr) );
    static std::uniform_real_distribution<float> distribution;

    return distribution(rng_engine);
}

// Initialize the game objects
void Game::InitGameObjects() {
    // set the default shader for the models
    auto myShader = shaders.find( "model");
    if ( myShader  == shaders.end()) {
        std::cout << "Could not find shader model" << endl;
    }

    int offset = 0;
    GameObject obj;
    for (auto mItr = gameModels.begin(); mItr != gameModels.end(); ++mItr) {
        obj.SetViewMatrix( camera.GetViewMatrix());
        obj.SetProjectionMatrix(globals.projectionMatrix);
        // obj.SetPosition( glm::vec3( (float) ((float)offset * 2.5f), (float) ((float)offset * 2.5f), (float) ((float)offset * 2.5f)));
        obj.SetPosition( glm::vec3( 200.0f*(floatrand()-0.5f),  200.0f*(floatrand()-0.5f), 200.0f*(floatrand()-0.5f)));
        obj.SetName( mItr->first);
        obj.SetCollider( true);
        obj.SetStatus( obj.ALIVE);
        obj.SetCenter( glm::vec3(
            abs( mItr->second.GetMaxValue().x - mItr->second.GetMinValue().x)/2,
            abs( mItr->second.GetMaxValue().y - mItr->second.GetMinValue().y)/2,
            abs( mItr->second.GetMaxValue().z - mItr->second.GetMinValue().z)/2
            ) );

        obj.SetColliderBoxDimentions( glm::vec3(
             mItr->second.GetMaxValue().x - mItr->second.GetMinValue().x,
             mItr->second.GetMaxValue().y - mItr->second.GetMinValue().y,
             mItr->second.GetMaxValue().z - mItr->second.GetMinValue().z
            ) );

        if ( mItr->first == "player") {
            obj.SetPosition( camera.GetPosition());
            obj.AttachCamera( &camera);
            obj.SetCameraPosition(camera.GetPosition());
            obj.SetRenderable( false);
            // std::cout << "Init: setting player....\n";
        } else
            obj.SetRenderable( true);


        obj.SetModel( &mItr->second);
        obj.SetShader( &myShader->second);

        // Make 10 of each sphere object
        if ( mItr->first == "sphere") {
            for ( int i = 0; i < 10; ++i) {
                obj.SetPosition( glm::vec3( 200.0f*(floatrand()-0.5f),  200.0f*(floatrand()-0.5f), 200.0f*(floatrand()-0.5f)));
                gameObjects.push_back(obj);
            }
        }   else    // else just make one
            gameObjects.push_back(obj);


        offset++;
    }

    // find and attach player
    for ( auto p = gameObjects.begin(); p != gameObjects.end(); p++) {
        if (p->GetName() == "player") {
            p->SetPosition( camera.GetPosition());
            p->AttachCamera( &camera);
            p->SetCameraPosition( glm::vec3(0.0f, 1.65f, 0.0f));
            std::cout << "players camera position: (" << p->GetCameraPosition().x << "," << p->GetCameraPosition().y << "," << p->GetCameraPosition().z << ")\n" ;
            p->SetClampMovementBounds(
                    glm::vec3(-100.0f, -100.0f, -100.0f),
                    glm::vec3(100.0f, 100.0f, 100.0f)
                    );
            player =  &(*p);
            break;      // Player object found get out'a here
        }

    }

}


// initialize HUD objects
void Game::InitHUDObjects() {
    // set the default shader for the models
    auto myShader = shaders.find( "model");
    if ( myShader  == shaders.end()) {
        std::cout << "Could not find shader model" << endl;
    }

    GameObject obj;
    for (auto mItr = hudModels.begin(); mItr != hudModels.end(); ++mItr) {
        if ( obj.GetName() == "compass")
            obj.SetPosition(glm::vec3( 1.0f, -1.0f, 0.0f));
        obj.SetCollider(false);

        obj.SetModel( &mItr->second);
        obj.SetShader( &myShader->second);

        hudObjects.push_back(obj);
    }
}



void Game::ReSpawnGameObjects() {
    for ( auto& go: gameObjects) {
        if ( go.GetName() == "player")
            go.SetRenderable( false);
        else
            go.SetRenderable( true);

        go.SetCollider( true);
        go.SetWireframe( false);
        go.SetStatus(go.ALIVE);
        go.SetPosition( glm::vec3( 200.0f*(floatrand()-0.5f),  200.0f*(floatrand()-0.5f), 200.0f*(floatrand()-0.5f)));
    }
}


void Game::InitCamera() {
    camera.SetFreeCamMode(true);
}

bool Game::InitGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat fov = 65.f;
    GLfloat nearPlane = 0.1f;
    GLfloat farPlane = 1000.f;
    globals.projectionMatrix = glm::perspective(
        glm::radians(fov),
        static_cast<float>(globals.screenwidth) / globals.screenheight,
        nearPlane, farPlane
        );

    //                               left, right, bottom, top, near, far
    globals.orthoMatrix = glm::ortho(0.0f,(float) globals.screenwidth, (float) globals.screenheight, 0.0f);

    // set the world matrix
    globals.worldMatrix = glm::mat4(1.f);
	return true;
}


void Game::Timing()
{
	// Handle Timing tnx @javidx9
	tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = tp2 - tp1;
	tp1 = tp2;
	// Our time per frame coefficient
	dt = elapsedTime.count();

	fFrameTimer += dt;
	nFrameCount++;
	if (fFrameTimer >= 1.0f)
	{
		fFrameTimer -= 1.0f;
		std::string sTitle = titleHeader + " - FPS: " + std::to_string(nFrameCount) + " / " + std::to_string(dt*1000) + "ms";
        SDL_SetWindowTitle(sdlWindow, sTitle.c_str());
		nFrameCount = 0;
	}
}


void Game::HandleEvents()
{
    events.Update();   // Get key / mouse input

    // SDL Events
    if (events.sdlEvents.ScreenResize)  {
        glViewport(0, 0, globals.screenwidth, globals.screenheight);
    }

    // Key Events
    if ( events.keys.Esc) {
        globals.m_gameRunning = false;
    }
}


void Game::Update()
{
    static bool toggleKey{false};
    static bool toggleKeyRel{false};
    static char toggleKeyID{'\0'};

    // Check if the player shall jump
    if ( events.keys.Space) {
        playerTRS.jumpOrder = true;
    }

    // Movements
    float cameraVelocity = 10.f;
    float cameraTurnVelocity = 4.0f*10;

    // stealthy
    if ( events.keys.LShift || events.keys.RShift) {
        cameraVelocity /= 4.0f;
        cameraTurnVelocity /= 10.0f;
    }
    // Run
    if ( events.keys.LCtrl || events.keys.RCtrl) {
        cameraVelocity *= 2.0f;
        cameraTurnVelocity *= 2.0f;
    }

        // Respawn player
        if ( events.keys.B && events.keys.LCtrl) {
            camera.Spawn( GetSpawnPoint(), { 180.f, 0.f, 0.f});
        } else
        // Reset player orientation
        if ( events.keys.G && events.keys.LCtrl)
            camera.SetYPR( { 180.f, 0.f, 0.f} );
        else
        // Toggle FreeCam
        if ( events.keys.C && events.keys.LCtrl) {
            toggleKey = true;
            toggleKeyID = 'C';
        } else
        // Toggle Wireframe drawing mode
        if ( events.keys.L && events.keys.LCtrl) {
            toggleKey = true;
            toggleKeyID = 'L';
        } else
        // Toggle reset object
        if ( events.keys.P) {
            toggleKey = true;
            toggleKeyID = 'P';  // TODO:  This must change   (etc. Return key?)
        } else
        if ( events.keys.H) {
            toggleKey = true;
            toggleKeyID = 'H';  // TODO:  This must change   (etc. Return key?)
        } else
        // reset toggle if none of the other keys are pressed above
        if (toggleKey) {
            toggleKeyRel = true;
        }

    if ( toggleKeyRel) {
        switch (toggleKeyID)
        {
        case 'C':
            std::cout << "ToggleKeyRel = true, toggleKeyID: " << toggleKeyID << std::endl;
            camera.toggleFreeCam();
            toggleKey = false;
            toggleKeyRel = false;
            toggleKeyID = 0;
            break;
        case 'L':
            std::cout << "ToggleKeyRel = true, toggleKeyID: " << toggleKeyID << std::endl;
            toggleLineMode();
            toggleKey = false;
            toggleKeyRel = false;
            toggleKeyID = 0;
            break;
        case 'P':
            ReSpawnGameObjects();
            toggleKey = false;
            toggleKeyRel = false;
            toggleKeyID = 0;
            break;
        case 'H':
            toggleKey = false;
            toggleKeyRel = false;
            toggleKeyID = 0;
            for ( auto& go: gameObjects) {
                glm::vec3 vecToObj = player->GetPosition() - go.GetPosition();
                glm::vec3 normVecToObj = glm::normalize(vecToObj);
                float dist = glm::length2( vecToObj);
                std::cout << "player to Object(" << go.GetName() << "):  Lenght=" << dist << " dir = (" << normVecToObj.x << "," << normVecToObj.y << "," << normVecToObj.z << ")\n";
            }
            break;
        default:
            break;
        }

    }



    // TODO:   The player moves and  camera is stuck on object!   ENGAGE!!!

        if ( events.keys.D && !events.keys.A)  camera.ProcessKeyboard( RIGHT, cameraVelocity, dt );
        if ( events.keys.A && !events.keys.D)  camera.ProcessKeyboard( LEFT, cameraVelocity, dt );
        if ( events.keys.W && !events.keys.S) {
            camera.ProcessKeyboard( FORWARD, cameraVelocity, dt );
        }
        if ( events.keys.S && !events.keys.W)  camera.ProcessKeyboard( BACKWARD, cameraVelocity, dt );
        if ( events.keys.F && !events.keys.R)  camera.ProcessKeyboard( UP, cameraVelocity, dt );
        if ( events.keys.R && !events.keys.F)  camera.ProcessKeyboard( DOWN, cameraVelocity, dt );


        if ( events.keys.Up && !events.keys.Down) {
            camera.SetTurnVelocity(cameraTurnVelocity);
            camera.ProcessKeyboard( PITCH_UP, cameraVelocity*2,dt );
        }
        if ( events.keys.Down && !events.keys.Up) {
            camera.SetTurnVelocity(cameraTurnVelocity);
            camera.ProcessKeyboard( PITCH_DOWN, cameraVelocity*2, dt );
        }

        if ( events.keys.Left && !events.keys.Right) {
            camera.SetTurnVelocity(cameraTurnVelocity);
            camera.ProcessKeyboard( YAW_LEFT, cameraVelocity*2, dt );
            }
        if ( events.keys.Right && !events.keys.Left) {
            camera.SetTurnVelocity(cameraTurnVelocity);
            camera.ProcessKeyboard( YAW_RIGHT, cameraVelocity*2, dt );
        }

        if ( events.keys.Q) {
            camera.SetTurnVelocity(cameraTurnVelocity);
            camera.ProcessKeyboard(ROLL_CCW, cameraVelocity, dt );

        }
        if ( events.keys.E) {
            camera.SetTurnVelocity(cameraTurnVelocity);
            camera.ProcessKeyboard(ROLL_CW, cameraTurnVelocity, dt );
        }

        // print out direction vector
        if ( events.keys.I) {
            glm::vec3 p = camera.GetPosition();
            glm::vec3 f = camera.GetFront();
            glm::vec3 u = camera.GetUp();
            std::cout << "position(" << p.x << "," << p.y << "," << p.z << ")"
                << "front(" << f.x << "," << f.y << "," << f.z << ")"
                << "up(" << u.x << "," << u.y << "," << u.z << ")"
                << " cam velocity: " << cameraVelocity <<  std::endl;
        }

        // Wheel Mouse Button zoom
        if ( events.mouse.MWDirY == 1)  camera.ProcessKeyboard( FORWARD, m_WMVelY, dt );
        if ( events.mouse.MWDirY == -1) camera.ProcessKeyboard( BACKWARD, m_WMVelY, dt );

        if ( events.keys.M) {
            events.mouse.toggleOnOff = !events.mouse.toggleOnOff;
        }

        if ( events.mouse.toggleOnOff) {
            // pitch = (static_cast<GLfloat>(events.mouse.x) - static_cast<GLfloat>(events.mouse.oldx)) * 50 * dt;
            // yaw = (static_cast<GLfloat>(events.mouse.oldy) - static_cast<GLfloat>(events.mouse.y)) * 50 * dt;
            // camera.ProcessMouseMovement( pitch, yaw, false);
            // SDL_WarpMouseInWindow( sdlWindow, globals.screenwidth/2, globals.screenheight/2);
        }




    // turn off jump if we are in freecam mode
    if (camera.GetFreeCamMode())
        playerTRS.jumpOrder = false;

    // update the player's posistion to camera, change this to update camera to player position
    playerTRS.translate = camera.GetPosition();

    // make the player jump
    if ( playerTRS.jumpOrder && !playerTRS.isJumping) {
        playerTRS.jumpOrder = false;
        playerTRS.isJumping = true;
        playerTRS.currentJumpVelocity = playerTRS.jumpForce;
        playerTRS.initialGroundLevel =  playerTRS.translate.y;
    }
    if (playerTRS.isJumping) {
        playerTRS.translate.y += playerTRS.currentJumpVelocity*dt;
        playerTRS.currentJumpVelocity -= playerTRS.gravity*10.0f*dt;
        // check if he still is airborn
         if ( playerTRS.translate.y-playerTRS.initialGroundLevel < 0.01f) {
            playerTRS.isJumping = false;
            playerTRS.jumpOrder = false;
            playerTRS.currentJumpVelocity = 0.f;
            playerTRS.translate.y = playerTRS.initialGroundLevel;
        }
        // keep camera's initial x and z position, only jump in y direction
        glm::vec3 t = camera.GetPosition();
        t.y = playerTRS.translate.y;
        camera.SetPosition( t);
    } else if (!camera.GetFreeCamMode() )  {
        // Player are not in the air so
        // Do the running movement

        // TODO:   have the player drop from FreeCam position down to ground or object.
        glm::vec3 t = camera.GetPosition();
        playerTRS.walkingMovementY = playerTRS.cameraPos.y + fabs(sin( std::sqrt(playerTRS.translate.x*playerTRS.translate.x
            +playerTRS.translate.z*playerTRS.translate.z)*2.f))/6.f;

        t.y = playerTRS.walkingMovementY;
        camera.SetPosition( t);

    }

    // TODO: temperary set player to the camera position,  it will be the other way around at some point.
    if (player != nullptr) {
        player->SetRotation(  glm::vec3( 0.0f, glm::radians( camera.GetYaw()), 0.0f)  );
        player->SetPosition( camera.GetPosition() - glm::vec3( 0.0f,1.65f,0.0f ));
        // set the camera back to the player position because of the player's bounderies/limitations...
        camera.SetPosition( player->GetPosition()+player->GetCameraPosition());
    }

    camera.ProcessInertia( dt);
}


void Game::RenderModels()
{
    GameObject *gO = nullptr;

    // Draw the system objects
    for ( auto &go: systemObjects) {
        go.SetViewMatrix(camera.GetViewMatrix( ));

        if ( go.GetName() == "ground_small")
            go.SetPosition( glm::vec3(0.0f));

        // This is set to the player
        if ( go.GetName() == "collisionbox")
            gO = &go;

        if ( go.GetRenderable())
            go.Draw(drawLineMode_enable);
    }


    // Draw the game objects
    for ( auto &go: gameObjects) {
        go.SetViewMatrix(camera.GetViewMatrix( ));
        if ( go.GetRenderable())
            go.Draw(drawLineMode_enable);
    }


    // draw the bounding boxes in wireframe
    if ( GetRenderCollisionBoxes())
    if ( gO != nullptr ) {
        for ( auto &go: gameObjects) {
            // Draw collision box on all object exept the collision box itself
            if ( go.GetRenderable()) {
                gO->SetViewMatrix(camera.GetViewMatrix( ));
                gO->SetCenter(go.GetCenter());
                gO->SetPosition(go.GetPosition());

                gO->SetScale( go.GetColliderBoxDimentions());

                gO->SetWireframe( true);
                gO->SetColliderBoxWireframeThickness(1.0f);
                gO->Draw(drawLineMode_enable);
                if ( !drawLineMode_enable)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

        }
    }



    // Check collisions of all objects
    // TODO: only check collision for player against other objects for now
    GameObject* objCollidedWith = nullptr;
    bool AllObjectTaken = true;
    for ( auto &go: gameObjects) {

        // Check if all abject is dead/taken/collected whatever.
        if ( go.GetStatus() == go.ALIVE && &go != player)
            AllObjectTaken = false;

        objCollidedWith = collision.Collided( gameObjects, go);
        if ( objCollidedWith != nullptr ) {
            if ( objCollidedWith->GetStatus() == objCollidedWith->ALIVE) {
                // Set the object to dead and not renderable if not the player
                if ( objCollidedWith != player) {
                    std::cout << go.GetName() << " Collided with " << objCollidedWith->GetName() << " object killed\n";
                    objCollidedWith->SetRenderable(false);
                    objCollidedWith->SetStatus(objCollidedWith->DEAD);
                    objCollidedWith->SetCollider(false);
                    objCollidedWith->SetWireframe(true);
                }
            }
        }
    }

    // respawn the objects if all where taken/dead
    if( AllObjectTaken) {
        ReSpawnGameObjects();
        std::cout << "Respawning objects..";
    }

    // Skybox
    shaderItr = shaders.find( "skybox"); if ( shaderItr  == shaders.end()) { std::cout << "Could not find shader model" << endl; }
    shaderItr->second.Use();
    shaderItr->second.setMat4("view", glm::mat3(camera.GetViewMatrix( )) ); // Remove any translation component of the view matrix
    shaderItr->second.setMat4("projection",  globals.projectionMatrix);
    skybox.RenderSkyBox();


    // Draw ortho gui
    PlotObjectsOnCompass( -camera.GetYaw(), glm::vec3(0.02f));
    // Draw compass: direction, scale, position
    DrawCompass( -camera.GetYaw());

}



/*
    Algo:
    Known parameters,
        - direction
        - distance

    What I need:
        - Angular direction

    Cartesian to Polar Coordinates
    Clamp the distance to the screen limits, in this case it is a
        raius of 0 - (compass dimentions * 0.4) in screen space (top:-1.5 <-> bottom:1.5)
    Caluculate the angular direction

    Polar to Carthesian coordinates
    plot it and transform it to suit the radar dimentions
*/
// plot the positions of the objects on the compass
void Game::PlotObjectsOnCompass( float direction, glm::vec3 scale, glm::vec3 pos, const float showRange) {
    float deleteme = direction+0.001f;deleteme += 0.00001f; // shut up compiler!!! I will use it sometime...

    shaderItr = shaders.find( "model"); if ( shaderItr  == shaders.end()) { std::cout << "Could not find shader modelLoadingOrtho" << endl; }
    obj.SetShader( &shaderItr->second);

    modelItr = systemModels.find("sphere"); if ( modelItr == systemModels.end()) { std::cout << "Could not find ortho test model box " << endl; }
    obj.SetName( modelItr->first);
    obj.SetModel( &modelItr->second);
    glm::mat4 orthoMat4 = glm::ortho(
        -2.0f,  // left
        2.0f,   // right
        -1.5f,  // bottom
        1.5f,   // top
        -1.0f, 1.0f // near, far
        );
    obj.SetPosition( glm::vec3(0.0f));
    obj.SetViewMatrix( glm::mat4(1.0f));
    obj.SetModelMatrix( glm::mat4(1.0f));
    // obj.SetScale( glm::vec3(0.05f));
    obj.SetScale(scale);
    obj.SetRenderable(true);


    glm::mat4 projMat4 = orthoMat4;
    for ( auto& go: gameObjects) {
        glm::vec3 vecToObj = player->GetPosition() - go.GetPosition();
        float dist = glm::length2( vecToObj);

        if ( dist < showRange && go.GetRenderable() && &go != player) {
            projMat4 = orthoMat4;

            glm::vec3 tmpVec3;
            tmpVec3.x = vecToObj.x/200.0f*2.0f;
            tmpVec3.y = vecToObj.z/200.0f*1.5f;
            tmpVec3.z = 0.5f;   // Draw the radar object over the compass

            // plot the object
            projMat4 = glm::translate( projMat4, tmpVec3);

            // Move the origin to the radar position
            projMat4 = glm::translate(projMat4, pos);
            obj.SetProjectionMatrix( projMat4);

            obj.Draw(drawLineMode_enable);
       }

    }
}



// Draw compass: direction, scale, position
void Game::DrawCompass( float direction, glm::vec3 scale, glm::vec3 pos) {

    // This will make the gui turn when we turn right?
    // obj.SetViewMatrix( glm::mat3(camera.GetViewMatrix() )); // Remove any translation component of the view matrix

    // set the orthographics perspective
    glm::mat4 orthoMat4 = glm::ortho(
        -2.0f,  // left
        2.0f,   // right
        -1.5f,  // bottom
        1.5f,   // top
        -1.0f, 1.0f // near, far
        );
    obj.SetViewMatrix( glm::mat4(1.0f));
    obj.SetModelMatrix( glm::mat4(1.0f));

    shaderItr = shaders.find( "model"); if ( shaderItr  == shaders.end()) { std::cout << "Could not find shader modelLoadingOrtho" << endl; }
    obj.SetShader( &shaderItr->second);

    modelItr = hudModels.find( "compass"); if ( modelItr == hudModels.end()) { std::cout << "Could not find ortho test model box " << endl; }
    obj.SetName( modelItr->first);
    obj.SetModel( &modelItr->second);

    glm::mat4 modelMat4 = orthoMat4;
    // scale the compass down to a given size
    obj.SetScale( scale);

    // modelMat4 = glm::translate(modelMat4, glm::vec3( 1.0f, -1.0f, 0.0f));
    modelMat4 = glm::translate(modelMat4, pos);

    // Rotate the compass at an angle so it will not be so flat
    // @CPU 90degrees * 1/1.618033988d (the inverse of the golden ratio)
    modelMat4 = glm::rotate(modelMat4,  glm::radians( 90.0f * 1.0f/1.618033988f ), glm::vec3( 1.0f, 0.0f, 0.0f) );

    // Rotate the compass to the direction of the camera Yaw
    modelMat4 = glm::rotate(modelMat4, glm::radians( direction), glm::vec3( 0.0f, 0.0f, 1.0f) );

    obj.SetProjectionMatrix( modelMat4);

    obj.Draw(drawLineMode_enable);
}

void Game::Clear( glm::vec4 col)
{
    glClearColor( col.r, col.g, col.b, col.a );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /* | GL_STENCIL_BUFFER_BIT */);
}

void Game::Render()
{
    Clear( glm::vec4(0.1f, 0.0f, 0.0f,1.0f));
    RenderModels();
}


void Game::toggleLineMode() {
    drawLineMode_enable = !drawLineMode_enable;
    if ( drawLineMode_enable) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}



// Main Engine loop
int Game::Run()
{
    tp1 = std::chrono::system_clock::now();
	tp2 = std::chrono::system_clock::now();

    Timing();
    InitControllers();
    InitData();
    InitCamera();

    // MAIN GAME LOOP
    std::cout << "Running engine..." << endl;
    while(globals.m_gameRunning)
    {
        Timing();
        HandleEvents();
        Update();   // UPDATE game logic
        Render();   // RENDER it
        SDL_GL_SwapWindow(sdlWindow);   // NO rendering after this point ---
        // glFinish();       // @SlicEnDicE, but if you run into issues and get graphics glitches use either glflush or glfinish
    }
    std::cout << "Engine exiting..." << endl;
    return globals.m_gameRunning;
}


// Housework
void Game::CleanUp()
{
    std::cout << "Cleaning up before exiting...\n";

	std::cout << "  SDL Closing Controllers...";
    if (SDL_JoystickGetAttached(events.m_controller.m_joystick)) {
        SDL_JoystickClose(events.m_controller.m_joystick);
		std::cout << "Joystick released...";
    }
	std::cout << "ok\n";

	std::cout << "  SDL GL Deleting Context...";
    SDL_GL_DeleteContext(sdlGLContext);
	std::cout << "  ok\n";

	std::cout << "  SDL Destroying Window...";
    SDL_DestroyWindow(sdlWindow);
	std::cout << "  ok\n";

	std::cout << "  SDL Quitting...";
    SDL_Quit();
	std::cout << "  ok\n";

	std::cout << "Cleanup complete" << std::endl;
}

