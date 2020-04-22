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

#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>

#include <string>
#include <vector>

#define MAX_CONTROLLERS 4

enum typeGamecontroller { CONTROLLER, JOYSTICK};

struct gameControllers
{
    std::string name;
    int type{JOYSTICK};
    int index{0};
    bool connected{false};
    int joysticks{0};
    int buttons{0};
    int hats{0};
    int axis{0};
    int balls{0};
};


class Controller {

public:
    Controller();
	int Init(void);
	int processEvent(const SDL_Event& event);
	int GetControllerIndex(SDL_JoystickID instance);
    std::vector<gameControllers> m_gameControllers;

    SDL_GameController *m_gameController;
    SDL_Joystick *m_joystick;

private:
	SDL_GameController *m_gamepad;
	SDL_Haptic *m_haptic;
	SDL_JoystickID m_instance_id;

	bool m_is_connected;
	std::string m_name;
	int m_axis;
	int m_buttons;

};