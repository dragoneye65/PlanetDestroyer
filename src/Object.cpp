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

#include "Object.hpp"

// If collider is set, then this are in the list of collidables
void GameObject::SetCollider( const bool Collider) { collider = Collider; }
// Set the dimentions of the collider box in height,width,depth
void GameObject::SetColliderBoxDimentions( const glm::vec3& ColliderBoxDimention) { colliderBoxDimention = ColliderBoxDimention; }

// Set the movements clamping boundaries's
void GameObject::SetClampMovementBounds(const glm::vec3 Bounds) {
    boundsMin = -(Bounds/2.0f);
    boundsMax = Bounds/2.0f;
}
void GameObject::SetClampMovementBounds(const glm::vec3 BoundsMin, const glm::vec3 BoundsMax) {
    boundsMin = BoundsMin;
    boundsMax = BoundsMax;
};

// Set the position
// returns true if it is out of bounds
// TODO: should probably set wich boundary's it is out of bounds on too
bool GameObject::SetPosition( const glm::vec3& Pos, bool clamp) {
    glm::vec3 newPos = Pos;
    bool outaBounds = false;

    if (clamp) {
        if( Pos.x < boundsMin.x) {
            newPos.x = boundsMin.x;
            outaBounds = true;
        }
        if( Pos.x > boundsMax.x) {
            newPos.x = boundsMax.x;
            outaBounds = true;
        }
        if( Pos.y < boundsMin.y) {
            newPos.y = boundsMin.y;
            outaBounds = true;
        }
        if( Pos.y > boundsMax.y) {
            newPos.y = boundsMax.y;
            outaBounds = true;
        }

        if( Pos.z < boundsMin.z) {
            newPos.z = boundsMin.z;
            outaBounds = true;
        }
        if( Pos.z > boundsMax.z) {
            newPos.z = boundsMax.z;
            outaBounds = true;
        }
    }

    position = newPos;
    return outaBounds;
}

// Get the position
glm::vec3 GameObject::GetPosition( ) { return position; }
// Set the center point (pivot)
void GameObject::SetCenter( const glm::vec3& Center) { center = Center; }
// Get the center point (pivot)
glm::vec3 GameObject::GetCenter() { return center; }
// Set the rotation
void GameObject::SetRotation( const glm::vec3& Rotation) { rotate = Rotation; }
// Get the rotation
glm::vec3 GameObject::GetRotation( ) { return rotate; }
// Set the scale
void GameObject::SetScale( const glm::vec3& Scale) { scale = Scale; }
// Get the scale
glm::vec3 GameObject::GetScale() { return scale; }
// Set the camera position
void GameObject::SetCameraPosition( const glm::vec3& CameraPos) { cameraPos = CameraPos; }
// Set Jumping order
void GameObject::Jump( bool JumpOrder) { jumpOrder = JumpOrder; }
// Return if the object is jumping (doh, do we need this?)
bool GameObject::isJumping() { return jumpingStatus; }
// Set the gravity of the object
void GameObject::SetGravity( const GLfloat& Gravity) { gravity = Gravity; }
// Set the mass of the object
void GameObject::SetMass( const GLfloat& Mass) { mass = Mass; }
// Set the object as movable
void GameObject::Moveable( bool Moveable) { moveable = Moveable; }
// Set the object as movable
bool GameObject::isMoveable( ) { return moveable; }
// Set the wireframe
void GameObject::SetWireframe( bool Wireframe, const glm::vec3& WireframeColor) {
    wireframe = Wireframe;
    wireframeColor = WireframeColor;
}
// set the world matrix
void GameObject::SetProjectionMatrix( const glm::mat4& ProjectionMatrix) { projectionMatrix = ProjectionMatrix; }
// set the view matrix
void GameObject::SetViewMatrix( const glm::mat4& ViewMatrix) { viewMatrix = ViewMatrix; }
// set the Model matrix
void GameObject::SetModelMatrix( const glm::mat4& ModelMatrix) { modelMatrix = ModelMatrix; }
// Attach the camera to the object
void GameObject::AttachCamera( Camera *Camera) { camera = Camera; }
// Detach the camera
void GameObject::DetachCamera() {  camera = nullptr;  }
// Set the shader program
void GameObject::SetShader( Shader *mShader) { shader = mShader; }
// Set the model
void GameObject::SetModel( Model *mModel) { model = mModel; }
// Set the name of the object
void GameObject::SetName( std::string Name) { name = Name; }
// Get the name of the object
std::string GameObject::GetName( ) { return name; }
// Draw collision bounding box for visualisation
void GameObject::DrawCollisionBox()
{

}

void GameObject::SetCollisionBoxColorActive( const glm::vec3& col) { collisionBoxColorActive = col; }
void GameObject::SetCollisionBoxColorInActive( const glm::vec3& col) { collisionBoxColorInActive = col; }

// Draw the object
void GameObject::Draw(bool globalWireframe_enabled)
{
    shader->Use();

    // if either Collider set og global wireframe set then do the wireframe
    if ( globalWireframe_enabled || GetWireframe()) {
        glLineWidth( GetColliderBoxWireframeThickness());
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        shader->setBool("wireframe_enable", 1);
        if ( GetWireframe())
            shader->setVec3("wireframeColor", GetWireframeColor());
        else
            shader->setVec3("wireframeColor", GetColliderWireframeColor());
    } else { // if not global wireframe set then set back to solid mode
        glLineWidth(1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shader->setBool("wireframe_enable", 0);
    }

    shader->setMat4("projection", projectionMatrix);
    shader->setMat4("view", viewMatrix);
    modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);

    modelMatrix = glm::rotate(modelMatrix, rotate.z, glm::vec3(0.f, 0.f, 1.f));
    modelMatrix = glm::rotate(modelMatrix, rotate.y, glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, rotate.x, glm::vec3(1.f, 0.f, 0.f));

    shader->setMat4("model", modelMatrix);

    model->Draw( *shader);
}



// Update the objects mechanics
void GameObject::Update(float deltaTime) {

    glm::vec3 t;

    if (jumpOrder && !jumpingStatus)
    {
        jumpOrder = false;
        jumpingStatus = true;
        currentJumpVelocity = jumpForce;
        initialGroundLevel = position.y;
    }
    if (jumpingStatus)
    {
        position.y += currentJumpVelocity * deltaTime;
        currentJumpVelocity -= gravity * 10.0f * deltaTime;
        // check if he still is airborn
        // TODO: Get ridd of initialGroundLevel and check for collision under
        if (position.y - initialGroundLevel < 0.01f)
        {
            jumpingStatus = false;
            jumpOrder = false;
            currentJumpVelocity = 0.f;
            position.y = initialGroundLevel;
        }
        t = position;
        t.y = position.y;
        position = t;
    }
    else if (walkingMovement)
    {
        // Player are not in the air so
        // Do the running movement

        // TODO:   have the player drop from FreeCam position down to ground or object.
        t = position;
        walkingMovementY = cameraPos.y - fabs(sin(std::sqrt(position.x * position.x + position.z * position.z) * 2.f)) / 6.f;

        t.y = walkingMovementY;
        position = t;
    }
}
