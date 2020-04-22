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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "Object.hpp"

// very rudementery box
class Collision
{
public:
    GameObject* Collided( vector<GameObject>& gameObjects, GameObject& o);
    bool Intersect( GameObject &ob, GameObject &o);

private:
    // thisObject collided with
    GameObject* collidedWithThisObject;
};

