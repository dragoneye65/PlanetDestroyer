/*
 * Copyright (C) 2020  Dragoneye
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

#include "Controller.hpp"


struct Keys
{
    bool Pressed{false};
    bool Released{false};
    bool Held{false};

    bool Left{false};
    bool Right{false};
    bool Up{false};
    bool Down{false};
    bool LShift{false};
    bool RShift{false};
    bool LCtrl{false};
    bool RCtrl{false};
    bool PageUp{false};
    bool PageDown{false};
    bool Space{false};
    bool Esc{false};
    bool Return{false};

    bool W{false};
    bool S{false};
    bool A{false};
    bool D{false};
    bool R{false};
    bool Q{false};
    bool E{false};
    bool M{false};
    bool X{false};
    bool C{false};
    bool F{false};
    bool P{false};
    bool I{false};
    bool J{false};
    bool K{false};
    bool L{false};
    bool G{false};
    bool B{false};
    bool H{false};

};


struct SDLEvents
{
    bool ScreenResize{false};
};

struct Mouse {
    int x{0};
    int y{0};
    int startx{0};
    int starty{0};
    int endx{0};
    int endy{0};
    int oldx{0};
    int oldy{0};
    bool startDrag{false};
    bool endDrag{false};

    bool motion{false};
    bool LBPressed{false};
    bool MBPressed{false};
    bool RBPressed{false};
    bool LBReleased{false};
    bool MBReleased{false};
    bool RBReleased{false};
    int MWDirY{0};

    bool toggleOnOff{false};
};

class Events
{
public:
    Keys keys;
    Controller m_controller;
    Mouse mouse;
    SDLEvents sdlEvents;

    void Update();
};