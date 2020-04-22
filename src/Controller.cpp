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
#include "Controller.hpp"


Controller::Controller()
{
	m_joystick = nullptr;
	m_gameController = nullptr;
}

int Controller::processEvent(const SDL_Event& event)
{

	switch (event.type) {
		case SDL_CONTROLLERAXISMOTION: {
			// handle axis motion
			break;
		}
		case SDL_CONTROLLERBUTTONDOWN:
			std::cout << "Controller button pressed" << std::endl;
			break;
		case SDL_CONTROLLERBUTTONUP: {
			// handle button up/down
			break;
		}
		case SDL_JOYBUTTONDOWN:
			std::cout << "Joystick button pressed" << std::endl;
			break;
		case SDL_JOYBUTTONUP:
			break;
		case SDL_CONTROLLERDEVICEADDED: {
			std::cout << "Controller added" << std::endl;
			if (event.cdevice.which < MAX_CONTROLLERS ) {
//				JoypadController& jc = s_controllers[event.cdevice.which];
//				jc.Open(event.cdevice.which);
			}
			break;
		}
		case SDL_CONTROLLERDEVICEREMOVED: {
			int cIndex = GetControllerIndex(event.cdevice.which);
			std::cout << "Controller removed" << std::endl;
			if (cIndex < 0) return 0; // unknown controller?
//			JoypadController& jc = ms_controllers[cIndex];
//			jc.Close();
			break;
		}
	}
	return 0;

}

/*
void Controller::Open(int device)
{
	m_gamepad = SDL_GameControllerOpen(device);
	SDL_Joystick *j = SDL_GameControllerGetJoystick(m_gamepad);
	m_instance_id = SDL_JoystickInstanceID(j);
	is_connected = true;
	if (SDL_JoystickIsHaptic(j)) {
		m_haptic = SDL_HapticOpenFromJoystick(j);
		printf("Haptic Effects: %d\n", SDL_HapticNumEffects(m_haptic));
		printf("Haptic Query: %x\n", SDL_HapticQuery(m_haptic));
		if (SDL_HapticRumbleSupported(m_haptic)) {
			if (SDL_HapticRumbleInit(m_haptic) != 0) {
				printf("Haptic Rumble Init: %s\n", SDL_GetError());
				SDL_HapticClose(m_haptic);
				m_haptic = 0;
			}
		} else {
			SDL_HapticClose(m_haptic);
			m_haptic = 0;
		}
	}
}

void Controller::Close(void)
{
	if (is_connected) {
		is_connected = false;
		if (haptic) {
			SDL_HapticClose(haptic);
			haptic = 0;
		}
		SDL_GameControllerClose(gamepad);
		gamepad = 0;
	}
}
*/


int Controller::GetControllerIndex(SDL_JoystickID instance)
{
/* 	for (int i = 0; i < MAX_CONTROLLERS; ++i)
	{
		if (ms_controllers[i].m_is_connected &&  ms_controllers[i].m_instance_id == instance) {
			return i;
		}
	}
 */
    std::cout << "TODO: Controller::GetControllerIndex instance: " << std::to_string( instance) << "\n";
	return -1;
}


int Controller::Init(void)
{
   	SDL_Joystick *joy = nullptr;
    int ret = false;

	// Check for joysticks
    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        // is the joystick an SDL game controller interface
        if (SDL_IsGameController(i))
        {
            m_gameController = SDL_GameControllerOpen(i);
            if (m_gameController)
            {
                ret = true;

                // Are there more joysticks on a game controller?  Perhaps 2?
                joy = SDL_GameControllerGetJoystick(m_gameController);

                gameControllers gc;
                gc.name = std::string(SDL_GameControllerName(m_gameController));
                gc.type = CONTROLLER;
                gc.index = i;
                gc.joysticks = 1;  // How to get the number of joysticks from game controller?
                gc.connected = true;
                gc.buttons = SDL_JoystickNumButtons(joy);
                gc.hats = SDL_JoystickNumHats(joy);
                gc.axis = SDL_JoystickNumAxes(joy);
                gc.balls = SDL_JoystickNumBalls(joy);

                m_gameControllers.push_back(gc);

            }
        }
        else
        {
            joy = SDL_JoystickOpen(i);

            // Joystick is valid
            if (joy)
            {
                gameControllers gc;
                gc.type = JOYSTICK;
                gc.joysticks = 1;
                gc.index = i;
                gc.connected = true;
                gc.name     = SDL_JoystickName(joy);
                gc.buttons  = SDL_JoystickNumButtons(joy);
                gc.hats     = SDL_JoystickNumHats(joy);
                gc.axis     = SDL_JoystickNumAxes(joy);
                gc.balls    = SDL_JoystickNumBalls(joy);

                m_gameControllers.push_back(gc);

                ret = true;
            }
        }
    }


    return ret;
}

