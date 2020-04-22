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

#include "Camera.hpp"
#include <iostream>

Camera camera( glm::vec3( 0.0f, 7.0f, 12.0f ));

Camera::Camera( glm::vec3 Position, glm::vec3 Up, GLfloat Yaw, GLfloat Pitch)
        : front( glm::vec3( 0.0f, 0.0f, 1.0f ) ), movementSpeed( SPEED ), mouseSensitivity( SENSITIVTY ), zoom( ZOOM )
{
    position = Position;
    worldUp = Up;
    yaw = Yaw;
    pitch = Pitch;

    accelerateVelocity = 50.0f;
    velocityDampening = 5.0f;
    for( int i = 0; i < THE_END; i++)
        persentVelocity[i].persentVelocity = 0.0f;

    updateCameraVectors( );
}

// Constructor with scalar values
Camera::Camera( GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat Yaw, GLfloat Pitch )
    : front( glm::vec3( 0.0f, 0.0f, 1.0f ) ), movementSpeed( SPEED ), mouseSensitivity( SENSITIVTY ), zoom( ZOOM )
{
    position = glm::vec3( posX, posY, posZ );
    worldUp = glm::vec3( upX, upY, upZ );
    yaw = Yaw;
    pitch = Pitch;

    accelerateVelocity = 50.0f;
    velocityDampening = 5.0f;
    for( int i = 0; i < THE_END; i++)
        persentVelocity[i].persentVelocity = 0.0f;
    updateCameraVectors( );
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix( )
{
    return glm::lookAt( position, position + front, up );
}

void Camera::SetVelocityDampening( GLfloat damp)
{
    velocityDampening = damp;
}

void Camera::ProcessInertia(GLfloat deltaTime) {

    // iterate through the velocities
    for( int i = 0; i < THE_END; i++) {
        // clamp it to -1 , 1
        if ( persentVelocity[i].persentVelocity > 1.0f) {
            persentVelocity[i].persentVelocity = 1.0f;
            needRecalc = true;
            continue;
        }
        else if ( persentVelocity[i].persentVelocity < -1.0f) {
            persentVelocity[i].persentVelocity = -1.0f;
            needRecalc = true;
            continue;
        }

        // zero out the persentVelocity if under minimum
         if ( persentVelocity[i].persentVelocity < myEpsilon
            && persentVelocity[i].persentVelocity > -myEpsilon) {
            persentVelocity[i].persentVelocity = 0.0f;
            }

     }

    if ( persentVelocity[FORWARD_BACKWARD].persentVelocity > myEpsilon) {
        velocity = userVelocity * deltaTime;
        position += front * velocity*persentVelocity[FORWARD_BACKWARD].persentVelocity;
        persentVelocity[FORWARD_BACKWARD].persentVelocity -= velocityDampening*deltaTime;
        needRecalc = true;

    } else
    if ( persentVelocity[FORWARD_BACKWARD].persentVelocity < myEpsilon) {
        velocity = userVelocity * deltaTime;
        position += front * velocity*persentVelocity[FORWARD_BACKWARD].persentVelocity;
        persentVelocity[FORWARD_BACKWARD].persentVelocity += velocityDampening*deltaTime;
        needRecalc = true;
    }

    if ( persentVelocity[LEFT_RIGHT].persentVelocity > myEpsilon) {
        velocity = userVelocity * deltaTime;
        position += right * velocity*persentVelocity[LEFT_RIGHT].persentVelocity;
        persentVelocity[LEFT_RIGHT].persentVelocity -= velocityDampening*deltaTime;
        needRecalc = true;
    }
    else if ( persentVelocity[LEFT_RIGHT].persentVelocity < myEpsilon) {
        velocity = userVelocity * deltaTime;
        position += right * velocity*persentVelocity[LEFT_RIGHT].persentVelocity;
        persentVelocity[LEFT_RIGHT].persentVelocity += velocityDampening*deltaTime;
        needRecalc = true;
    }

    if ( persentVelocity[UP_DOWN].persentVelocity > myEpsilon) {
        velocity = userVelocity * deltaTime;
        position += up * velocity*persentVelocity[UP_DOWN].persentVelocity;
        persentVelocity[UP_DOWN].persentVelocity -= velocityDampening*deltaTime;
        needRecalc = true;
    }
    else if ( persentVelocity[UP_DOWN].persentVelocity < myEpsilon) {
        velocity = userVelocity * deltaTime;
        position += up * velocity*persentVelocity[UP_DOWN].persentVelocity;
        persentVelocity[UP_DOWN].persentVelocity += velocityDampening*deltaTime;
        needRecalc = true;
    }

    if ( persentVelocity[YAW_LEFT_RIGHT].persentVelocity > myEpsilon) {
        velocity = userVelocity * deltaTime;
        yaw +=  2 * turnVelocity*deltaTime*persentVelocity[YAW_LEFT_RIGHT].persentVelocity;
        persentVelocity[YAW_LEFT_RIGHT].persentVelocity -= velocityDampening*deltaTime;
        needRecalc = true;
    }
    else if ( persentVelocity[YAW_LEFT_RIGHT].persentVelocity < myEpsilon) {
        velocity = userVelocity * deltaTime;
        yaw +=  2 * turnVelocity*deltaTime*persentVelocity[YAW_LEFT_RIGHT].persentVelocity;
        persentVelocity[YAW_LEFT_RIGHT].persentVelocity += velocityDampening*deltaTime;
        needRecalc = true;
    }

    if ( persentVelocity[PITCH_UP_DOWN].persentVelocity > myEpsilon) {
        velocity = userVelocity * deltaTime;
        pitch +=  2*velocity*persentVelocity[PITCH_UP_DOWN].persentVelocity;
        persentVelocity[PITCH_UP_DOWN].persentVelocity -= velocityDampening*deltaTime;
        needRecalc = true;
    }
    else if ( persentVelocity[PITCH_UP_DOWN].persentVelocity < myEpsilon) {
        velocity = userVelocity * deltaTime;
        pitch +=  2 * velocity*persentVelocity[PITCH_UP_DOWN].persentVelocity;
        persentVelocity[PITCH_UP_DOWN].persentVelocity += velocityDampening*deltaTime;
        needRecalc = true;
    }

    // TODO: The CCW drifting happens here!!!  0.2f seams to help   originally myEpsilon = 0.05f
    if ( (persentVelocity[ROLL_CCW_CW].persentVelocity > 0.2f)) {
        velocity = userVelocity * deltaTime;
        roll +=  2 * turnVelocity*deltaTime*persentVelocity[ROLL_CCW_CW].persentVelocity;
        persentVelocity[ROLL_CCW_CW].persentVelocity -= velocityDampening*2*deltaTime;
        needRecalc = true;
    }
    else
        if ( (persentVelocity[ROLL_CCW_CW].persentVelocity < myEpsilon)) {
            velocity = userVelocity * deltaTime;
            roll +=  2 * turnVelocity*deltaTime*persentVelocity[ROLL_CCW_CW].persentVelocity;
            persentVelocity[ROLL_CCW_CW].persentVelocity += velocityDampening*2*deltaTime;
            needRecalc = true;
        }



    if (needRecalc) {
        updateCameraVectors();
    }
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard( Camera_Movement direction, GLfloat userVel, GLfloat deltaTime )
{
    bool tmp_yaw = yaw_enable;
    bool tmp_pitch = pitch_enable;
    bool tmp_roll = roll_enable;
    bool tmp_lateral = lateral_enable;
    bool tmp_vertical = vertical_enable;
    bool tmp_longitudinal = longitudinal_enable;

    if ( allMovements_enable) {
        yaw_enable = true;
        pitch_enable = true;
        roll_enable = true;
        lateral_enable = true;
        vertical_enable = true;
        longitudinal_enable = true;
    }

    userVelocity = userVel;

    if ( direction == FORWARD && forward_enable && longitudinal_enable) {
        Velocity = accelerateVelocity*velocityMagnitude*deltaTime;
        persentVelocity[FORWARD_BACKWARD].persentVelocity += Velocity;
    }

    if ( direction == BACKWARD && backward_enable && longitudinal_enable) {
        Velocity = accelerateVelocity*velocityMagnitude*deltaTime;
        persentVelocity[FORWARD_BACKWARD].persentVelocity -= Velocity;
    }

    if ( direction == LEFT && left_enable && lateral_enable) {
        Velocity = accelerateVelocity*velocityMagnitude*deltaTime;
        persentVelocity[LEFT_RIGHT].persentVelocity -= Velocity;
    }

    if ( direction == RIGHT && right_enable && lateral_enable) {
        Velocity = accelerateVelocity*velocityMagnitude*deltaTime;
        persentVelocity[LEFT_RIGHT].persentVelocity += Velocity;
    }

    if ( direction == UP && up_enable && vertical_enable) {
        Velocity = accelerateVelocity*velocityMagnitude*deltaTime;
        persentVelocity[UP_DOWN].persentVelocity -= Velocity;
    }

    if ( direction == DOWN && down_enable && vertical_enable) {
        Velocity = accelerateVelocity*velocityMagnitude*deltaTime;
        persentVelocity[UP_DOWN].persentVelocity += Velocity;
    }

    if ( direction == YAW_LEFT && yawLeft_enable && yaw_enable) {
        Velocity = accelerateVelocity*velocityMagnitude*deltaTime;
        persentVelocity[YAW_LEFT_RIGHT].persentVelocity += Velocity*0.5f;
    }

    if ( direction == YAW_RIGHT && yawRight_enable && yaw_enable) {
        Velocity = accelerateVelocity*velocityMagnitude*deltaTime;
        persentVelocity[YAW_LEFT_RIGHT].persentVelocity -= Velocity*0.5f;
    }

    if ( direction == PITCH_UP && pitchUp_enable && pitch_enable) {
        Velocity = accelerateVelocity*velocityMagnitude*deltaTime;
        persentVelocity[PITCH_UP_DOWN].persentVelocity -= Velocity*0.5f;
    }

    if ( direction == PITCH_DOWN && pitchDown_enable && pitch_enable) {
        Velocity = accelerateVelocity*velocityMagnitude*deltaTime;
        persentVelocity[PITCH_UP_DOWN].persentVelocity += Velocity*0.5f;
    }

    if ( direction == ROLL_CCW && rollCCW_enable && roll_enable) {
        if ( roll < 20.0f) {
            Velocity = accelerateVelocity*velocityMagnitude*deltaTime;
            persentVelocity[ROLL_CCW_CW].persentVelocity += Velocity*0.5f;
        }
    }
    if ( direction == ROLL_CW && rollCW_enable && roll_enable) {
        if ( roll > -20.0f) {
            Velocity = accelerateVelocity*velocityMagnitude*deltaTime;
           persentVelocity[ROLL_CCW_CW].persentVelocity -= Velocity*0.5f;
        }
    }


    yaw_enable          = tmp_yaw;
    pitch_enable        = tmp_pitch;
    roll_enable         = tmp_roll;
    lateral_enable      = tmp_lateral;
    vertical_enable     = tmp_vertical;
    longitudinal_enable = tmp_longitudinal;

}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement( GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true )
{
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw   += xOffset;
    pitch += yOffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if ( constrainPitch )
    {
        if ( pitch > 89.0f ) {
            pitch = 89.0f;
        }

        if ( pitch < -89.0f ) {
            pitch = -89.0f;
        }
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    updateCameraVectors( );
}

GLfloat Camera::GetZoom( ) {
    return zoom;
}

glm::vec3 Camera::GetPosition( ) {
    return position;
}

void Camera::SetPosition( glm::vec3 pos) {
    position = pos;
    needRecalc = true;
    updateCameraVectors();
}

glm::vec3 Camera::GetRight( ) {
    return right;
}

glm::vec3 Camera::GetFront( ) {
    return front;
}

glm::vec3 Camera::GetUp( ) {
    return up;
}

void Camera::updateCameraVectors( )
{
    if (needRecalc) {
        front = glm::normalize(glm::vec3(
            cos(glm::radians(pitch)) * sin(glm::radians(yaw)),
            sin(glm::radians(pitch)),
            cos(glm::radians(pitch)) * cos(glm::radians(yaw))
        ));
        // Right vector
        right = glm::normalize(glm::vec3(
            sin(glm::radians(yaw) - 3.14159265359f / 2.0f),
            glm::radians(roll),
            cos(glm::radians(yaw) - 3.14159265359f / 2.0f)
        ));
        // Up vector
        up = glm::normalize(glm::cross(right, front));
        needRecalc = false;
    }
}

