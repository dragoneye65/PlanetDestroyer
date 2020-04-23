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

#ifdef _WIN32
    #include <SDL2/include/SDL.h>
#else
    #include <SDL2/SDL.h>
#endif


#include "Events.hpp"
#include "Globals.hpp"


extern Globals globals;
Events events;

void Events::Update( )
{
    SDL_Event event;
    SDL_PumpEvents();
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    sdlEvents.ScreenResize = false;

    keys.Up = false;
    keys.Down = false;
    keys.Left = false;
    keys.Right = false;
    keys.RShift = false;
    keys.LShift = false;
    keys.LCtrl = false;
    keys.RCtrl = false;
    keys.PageDown = false;
    keys.PageUp = false;
    keys.Space = false;
    keys.Esc = false;
    keys.Return = false;


    keys.W = false;
    keys.S = false;
    keys.A = false;
    keys.D = false;
    keys.Q = false;
    keys.E = false;
    keys.M = false;
    keys.X = false;
    keys.C = false;
    keys.R = false;
    keys.F = false;
    keys.P = false;
    keys.I = false;
    keys.J = false;
    keys.K = false;
    keys.L = false;
    keys.G = false;
    keys.B = false;
    keys.H = false;


    mouse.motion = false;
    mouse.LBPressed = false;
    mouse.MBPressed = false;
    mouse.RBPressed = false;

    mouse.LBReleased = false;
    mouse.MBReleased = false;
    mouse.RBReleased = false;

    mouse.MWDirY = 0;

    //continuous-response keys
    if(keystate[SDL_SCANCODE_LEFT])     keys.Left = true;
    if(keystate[SDL_SCANCODE_RIGHT])    keys.Right = true;
    if(keystate[SDL_SCANCODE_UP])       keys.Up = true;
    if(keystate[SDL_SCANCODE_DOWN])     keys.Down = true;
    if(keystate[SDL_SCANCODE_LSHIFT])   keys.LShift = true;
    if(keystate[SDL_SCANCODE_RSHIFT])   keys.RShift = true;
    if(keystate[SDL_SCANCODE_LCTRL])    keys.LCtrl = true;
    if(keystate[SDL_SCANCODE_RCTRL])    keys.RCtrl = true;
    if(keystate[SDL_SCANCODE_PAGEDOWN]) keys.PageUp = true;
    if(keystate[SDL_SCANCODE_PAGEUP])   keys.PageDown = true;
    if(keystate[SDL_SCANCODE_SPACE])    keys.Space = true;
    if(keystate[SDL_SCANCODE_ESCAPE])   keys.Esc = true;
    if(keystate[SDL_SCANCODE_RETURN])   keys.Return = true;

    if(keystate[SDL_SCANCODE_W ])       keys.W = true;
    if(keystate[SDL_SCANCODE_S ])       keys.S = true;
    if(keystate[SDL_SCANCODE_A ])       keys.A = true;
    if(keystate[SDL_SCANCODE_D ])       keys.D = true;
    if(keystate[SDL_SCANCODE_R ])       keys.R = true;
    if(keystate[SDL_SCANCODE_Q ])       keys.Q = true;
    if(keystate[SDL_SCANCODE_E ])       keys.E = true;
    if(keystate[SDL_SCANCODE_M ])       keys.M = true;
    if(keystate[SDL_SCANCODE_X ])       keys.X = true;
    if(keystate[SDL_SCANCODE_C ])       keys.C = true;
    if(keystate[SDL_SCANCODE_F ])       keys.F = true;
    if(keystate[SDL_SCANCODE_P ])       keys.P = true;

    if(keystate[SDL_SCANCODE_I ])       keys.I = true;
    if(keystate[SDL_SCANCODE_J ])       keys.J = true;
    if(keystate[SDL_SCANCODE_K ])       keys.K = true;
    if(keystate[SDL_SCANCODE_L ])       keys.L = true;
    if(keystate[SDL_SCANCODE_G ])       keys.G = true;
    if(keystate[SDL_SCANCODE_B ])       keys.B = true;
    if(keystate[SDL_SCANCODE_H ])       keys.H = true;


    while (SDL_PollEvent(&event)) {

        // Check the controllers for events
		m_controller.processEvent(event);

        switch (event.type) {

        // Check for window movement
        case SDL_WINDOWEVENT:
            // Do we need to resize the glViewPort?
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                sdlEvents.ScreenResize = true;
                globals.screenwidth = event.window.data1;
                globals.screenheight = event.window.data2;
            }

            break;

        case SDL_QUIT:
            keys.Esc = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState( &mouse.startx, &mouse.starty );
            mouse.startDrag = true;
            mouse.endDrag = false;
            mouse.endx = mouse.startx;
            mouse.endy = mouse.starty;
            break;
        case SDL_MOUSEBUTTONUP:
            SDL_GetMouseState( &mouse.endx, &mouse.endy );
            mouse.startDrag = false;
            mouse.endDrag = true;
            break;
        case SDL_MOUSEMOTION:
       		mouse.oldx = mouse.x;
       		mouse.oldy = mouse.y;
            SDL_GetMouseState( &mouse.x, &mouse.y );
            mouse.motion = true;
            break;
        case SDL_MOUSEWHEEL:
            if ( event.wheel.y > 0)
                mouse.MWDirY = 1;
            if ( event.wheel.y < 0)
                mouse.MWDirY = -1;
            break;

        case SDL_KEYDOWN:
            keys.Pressed = true;
            keys.Released = false;
            keys.Held = true;
            break;

        case SDL_KEYUP:
            keys.Pressed = false;
            keys.Released = true;
            keys.Held = false;
            break;

        }
    }
}
