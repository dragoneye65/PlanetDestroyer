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

#include "Collision.hpp"
#include "Object.hpp"

// Tnx @gorbit
//    return (a.minX <= b.maxX && a.maxX >= b.minX) &&
//        (a.minY <= b.maxY && a.maxY >= b.minY) &&
//        (a.minZ <= b.maxZ && a.maxZ >= b.minZ);

// Check if the two objects intersect's
bool Collision::Intersect( GameObject &ob, GameObject &o) {
    glm::vec3 o_min,ob_min;
    glm::vec3 o_max,ob_max;

    glm::vec3 o_pos = o.GetPosition();
    glm::vec3 ob_pos = ob.GetPosition();

    glm::vec3 o_center = o.GetCenter();
    glm::vec3 ob_center = ob.GetCenter();

    o_min = o_pos-o_center;
    ob_min = ob_pos-ob_center;
    o_max = o_pos+o_center;
    ob_max = ob_pos+ob_center;

    return (o_min.x <= ob_max.x && o_max.x >= ob_min.x) &&
        (o_min.y <= ob_max.y && o_max.y >= ob_min.y) &&
        (o_min.z <= ob_max.z && o_max.z >= ob_min.z);
}


GameObject* Collision::Collided( vector<GameObject>& gameObjects, GameObject& o)
{
    for ( auto& go: gameObjects)
        if ( go.GetRenderable())
            if ( &go != &o)
                if ( Intersect( go, o))
                    return &go;

    return nullptr;
}
