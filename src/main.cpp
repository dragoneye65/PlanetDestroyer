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
#include "Game.hpp"
#include "Globals.hpp"

Globals globals;

int main() {
    std::cout << "Loading Game Engine\n";
    Game game;
    std::cout << "Initializing SDL...";
    if (game.InitSDL("SDL2/OpenGL Engine by Dragoneye", 1024, 600))
        std::cout << "ok\n";

    std::cout << "Initializing OpenGL...\n";
    if (game.InitGL()) {
        globals.m_gameRunning = true;
        std::cout << "Initialized OpenGL ok\n";
    } else {
        std::cout << "Initializing OpenGL Failed!\n";
    }

    game.Run();
    game.CleanUp();
    std::cout << "Game Engine exited" << std::endl;
}