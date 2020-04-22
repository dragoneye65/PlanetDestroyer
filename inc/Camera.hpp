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

// Std. Includes
#include <vector>

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    YAW_LEFT,
    YAW_RIGHT,
    PITCH_UP,
    PITCH_DOWN,
    ROLL_CCW,
    ROLL_CW,
    KEY_Q,
    KEY_E
};

enum Camera_PersentVelocity
{
    FORWARD_BACKWARD,
    LEFT_RIGHT,
    UP_DOWN,
    YAW_LEFT_RIGHT,
    PITCH_UP_DOWN,
    ROLL_CCW_CW,
    THE_END
};

// Default camera values
const GLfloat YAW        =  180.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat ROLL       =  0.0f;
const GLfloat SPEED      =  4.0f;
const GLfloat SENSITIVTY =  0.25f;
const GLfloat ZOOM       =  45.0f;
const GLfloat myEpsilon  = 0.05f;

struct PersentVelocity
{
    GLfloat persentVelocity{0.0f};
};

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:

    // Constructor with vectors
    Camera( glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3 up = glm::vec3( 0.0f, 1.0f, 0.0f ), GLfloat yaw = YAW, GLfloat pitch = PITCH);

    // Constructor with scalar values
    Camera( GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard( Camera_Movement direction, GLfloat userVel, GLfloat deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement( GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
 //   void ProcessMouseScroll( GLfloat yOffset);

    GLfloat GetZoom();
    glm::vec3 GetPosition();
    glm::vec3 GetFront();
    glm::vec3 GetUp();
    glm::vec3 GetRight();

    void Spawn(glm::vec3 Position) {  position = Position; updateCameraVectors();}
    void Spawn(glm::vec3 Position, glm::vec3 lookat) {
        position = Position;
        yaw = lookat.x;
        pitch = lookat.y;
        roll = lookat.z;
        updateCameraVectors();
        }

    void SetYPR( glm::vec3 ypr) {
        yaw = ypr.x;
        pitch = ypr.y;
        roll = ypr.z;
        updateCameraVectors();
    }

    glm::vec3 GetYPR() {
        return { glm::vec3(yaw, pitch, roll) };
    }
    void SetYaw(bool setYaw) { yaw_enable = setYaw; }
    float GetYaw() { return yaw; }
    void SetPitch(bool setPitch) { pitch_enable = setPitch; }
    void SetRoll(bool setRoll) { roll_enable = setRoll; }
    void SetLateral(bool setLateral) { lateral_enable = setLateral; }
    void SetVertical(bool setVertical) { vertical_enable = setVertical; }
    void setLongitudinal(bool setLongitudinal) { longitudinal_enable = setLongitudinal; }

    void SetYawLeft(bool setYawLeft) { yawRight_enable = setYawLeft; }
    void SetYawRight(bool setYawRight) { yawRight_enable = setYawRight; }

    void SetPitchUp(bool setPitchUp) { pitchUp_enable = setPitchUp; }
    void SetPitchDown(bool setPitchDown) { pitchDown_enable = setPitchDown; }
    void SetRollCCW(bool setRollCCW) { rollCCW_enable = setRollCCW; }
    void SetRollCW(bool setRollCW) { rollCW_enable = setRollCW; }

    void SetRight(bool setRight) { right_enable = setRight; }
    void SetLeft(bool setLeft) { left_enable = setLeft; }

    bool toggleFreeCam() {
        allMovements_enable = !allMovements_enable;
        return allMovements_enable;
    }
    bool SetFreeCamMode( bool fc) { allMovements_enable = fc; return allMovements_enable; }
    bool GetFreeCamMode() { return allMovements_enable; }

    void SetVelocityDampening( GLfloat damp);
    void SetPosition( glm::vec3 pos);
    void ProcessInertia(GLfloat deltaTime);
    void SetTurnVelocity( GLfloat tVelocity) { turnVelocity = tVelocity; }
    glm::vec3 position;
private:
    // Camera Attributes
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Enable / disable movements
    bool yaw_enable{true};
    bool pitch_enable{true};
    bool roll_enable{true};
    bool yawLeft_enable{true};
    bool yawRight_enable{true};
    bool rollCCW_enable{true};
    bool rollCW_enable{true};
    bool pitchUp_enable{true};
    bool pitchDown_enable{true};
    bool forward_enable{true};
    bool backward_enable{true};
    bool left_enable{true};
    bool right_enable{true};
    bool up_enable{true};
    bool down_enable{true};
    bool lateral_enable{true};
    bool vertical_enable{true};
    bool longitudinal_enable{true};
    bool allMovements_enable{false};

    GLfloat velocityMagnitude{10000.0f};
    GLfloat Velocity{0.0f}; // temperary velocity placeholder

    // Eular Angles
    GLfloat yaw;
    GLfloat pitch;
    GLfloat roll{0.0f};

    // Inter cache so we dont have to create and destroy the same on the stack in each frame
    glm::vec3 oldPosition;
    glm::vec3 oldOrientation{0.0f, 0.0f, 0.0f};
    bool needRecalc{false};

    // Camera options
    GLfloat turnVelocity;
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;
    GLfloat velocity;
    GLfloat userVelocity;
    GLfloat accelerateVelocity;
    GLfloat velocityDampening;
    PersentVelocity persentVelocity[6];

    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors();
};
