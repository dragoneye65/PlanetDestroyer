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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include "Shader.hpp"
#include "Model.hpp"
#include "Camera.hpp"


// Have to add this everytime I export from blender in "compass.mtl"
// map_Kd compass_texture_bg.png

class GameObject {

public:
    GameObject() {
    };

    // If collider is set, then this are in the list of collidables
    void SetCollider( const bool Collider);
    // Is this object to be rendered
    void SetRenderable( bool RenderAble = true) { renderAble = RenderAble; }
    bool GetRenderable() { return renderAble; }
    // return true if the collider flagg is set
    bool GetCollider()  { return collider; }
    // Set the dimentions of the collider box in height,width,depth
    void SetColliderBoxDimentions( const glm::vec3& ColliderBoxDimention);
    // Get the dimentions of the collider box in height,width,depth
    glm::vec3 GetColliderBoxDimentions() { return colliderBoxDimention; }
    // set the movement bounds
    void SetClampMovementBounds(const glm::vec3 Bounds);
    void SetClampMovementBounds(const glm::vec3 BoundsMin, const glm::vec3 BoundsMax);
    // Set the position
    bool SetPosition( const glm::vec3& Pos, bool clamp = true);
    // Get the position
    glm::vec3 GetPosition();
    // Set the center point (pivot)
    void SetCenter( const glm::vec3& Center);
    // Get the center point (pivot)
    glm::vec3 GetCenter();
    // Set the rotation
    void SetRotation( const glm::vec3& Rotation);
    // Get the rotation
    glm::vec3 GetRotation();
    // Set the scale
    void SetScale( const glm::vec3& Scale);
    // Get the scale
    glm::vec3 GetScale();
    // Set the camera position
    void SetCameraPosition( const glm::vec3& CameraPos);
    // Get the camera position
    glm::vec3 GetCameraPosition( ) { return cameraPos; };
    // Set Jumping order
    void Jump( bool JumpOrder);
    // Return if the object is jumping (doh, do we need this?)
    bool isJumping();
    // Set the gravity of the object
    void SetGravity( const GLfloat& Gravity);
    // Set the mass of the object
    void SetMass( const GLfloat& Mass);
    // Set the object as movable
    void Moveable( bool Moveable);
    // Set the object as movable
    bool isMoveable();
    // Update the objects mechanics
    void Update( float deltaTime);
    // Draw the object
    void Draw( bool globalWireframe_enabled = false);
    // Draw collision bounding box for visualisation
    void DrawCollisionBox();
    // Set the wireframe mode and/or color
    void SetWireframe( bool Wireframe, const glm::vec3& WireframeColor = glm::vec3(0.0f,0.0f,0.4f));
    // Get the wireframe mode status
    bool GetWireframe() { return wireframe; }
    // Get wireframe color depended on if collider is set
    glm::vec3 GetColliderWireframeColor() {
        if (GetCollider())
            return collisionBoxColorActive;
        else
            return collisionBoxColorInActive;
        }
    glm::vec3 GetWireframeColor() { return wireframeColor; }
    // set the world matrix
    void SetProjectionMatrix( const glm::mat4& ProjectionMatrix);
    // Get the world matrix
    glm::mat4 GetProjectionMatrix() { return projectionMatrix; };
    // set the view matrix
    void SetViewMatrix( const glm::mat4& ViewMatrix);
    // set the Model matrix
    void SetModelMatrix( const glm::mat4& ModelMatrix);
    // Attach the camera to the object
    void AttachCamera( Camera *Camera = nullptr);
    // Detach the camera
    void DetachCamera();
    // Set the shader program
    void SetShader( Shader *mShader);
    // Set the model
    void SetModel( Model *mModel);
    // Set the name of the object
    void SetName( std::string Name);
    // Get the name of the object
    std::string GetName();
    void SetCollisionBoxColorActive( const glm::vec3& col = glm::vec3(0.0f,0.5f, 0.0f));
    void SetCollisionBoxColorInActive( const glm::vec3& col = glm::vec3(0.5f,0.0f, 0.0f));
    void SetColliderBoxWireframeThickness( const float ColliderBoxWireframeThickness) {
        colliderBoxWireframeThickness = ColliderBoxWireframeThickness;
    }
    float GetColliderBoxWireframeThickness() {
        return colliderBoxWireframeThickness;
    }

    void SetStatus( bool PlayerStatus) { playerStatus = PlayerStatus; }
    bool GetStatus( ) { return playerStatus; }

    // Dont need this, just the other draw member function with view and model set to identity
    // void DrawOrtho(bool globalWireframe_enabled);
    enum { ALIVE,DEAD };

    void SetMoveController();
private:
    std::string name;
    Shader *shader;                // The shader program to use with this object
    Model *model;
    Camera *camera;

    bool renderAble{true};
    bool playerStatus{ ALIVE};

    glm::vec3 boundsMin{-1000.0f, -1000.0f, -1000.0f};                   // the boundaries it is allowed to move in
    glm::vec3 boundsMax{1000.0f, 1000.0f, 1000.0f};                   // the boundaries it is allowed to move in

    // TODO: Why this one?
    bool wireframe{false};                  // set the wireframe mode
    glm::vec3 wireframeColor{1.0f};       // set the color for the wireframe mode
    bool moveable{false};                   // are the object movable?
    glm::vec3 position{0.0f};               // Objects position
    glm::vec3 center{0.0f};                 // The center point
    glm::vec3 rotate{0.0f};                 // This is the current rotation
    glm::vec3 scale{1.f,1.f,1.f};           // The scale of the object
    glm::vec3 cameraPos{0.0f, 0.0f, 0.0f};  // where are the camera to look out from
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 modelMatrix;

    bool collider{true};                   // Does this object collide with others?
    float colliderBoxWireframeThickness{2.0f};
    glm::vec3 colliderBoxDimention{0.0f,0.0f,0.0f};    // the width,height,depth of the collider box
    glm::vec3 collisionBoxColorActive{0.0f,0.5f, 0.0f};
    glm::vec3 collisionBoxColorInActive{0.5f,0.0f, 0.0f};

    // This should be a modifier on the object of sorts...
    bool walkingMovement{false};            // is this a walking movement behaviar on this object?
    float walkingMovementY{0.f};
    bool jumpOrder{false};                  // Does he have a jump order?
    bool jumpingStatus{false};              // check if we hit some other object to stand on
    GLfloat jumpForce{20.f};                // The jumping force
    GLfloat gravity{10.f};                  // Gravity
    GLfloat mass{1.0f};                     // Mass of the object
    GLfloat initialGroundLevel{0.f};        // this is where he started jumping from, (get rid of it)
    GLfloat currentJumpVelocity{0.f};       // Current jump velocity, calculated on the fly until we hit something

};
