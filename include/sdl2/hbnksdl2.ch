#ifndef HBSDL2_CH_
#define HBSDL2_CH_

#include "SDL_events.ch"
#include "SDL_hints.ch"
#include "SDL_keycode.ch"
#include "SDL_scancode.ch"
#include "SDL_render.ch"
#include "SDL_video.ch"

#define SDL_INIT_TIMER          0x00000001
#define SDL_INIT_AUDIO          0x00000010
#define SDL_INIT_VIDEO          0x00000020  /**< SDL_INIT_VIDEO implies SDL_INIT_EVENTS */
#define SDL_INIT_JOYSTICK       0x00000200  /**< SDL_INIT_JOYSTICK implies SDL_INIT_EVENTS */
#define SDL_INIT_HAPTIC         0x00001000
#define SDL_INIT_GAMECONTROLLER 0x00002000  /**< SDL_INIT_GAMECONTROLLER implies SDL_INIT_JOYSTICK */
#define SDL_INIT_EVENTS         0x00004000
#define SDL_INIT_SENSOR         0x00008000
#define SDL_INIT_NOPARACHUTE    0x00100000  /**< compatibility; this flag is ignored. */
#define SDL_INIT_EVERYTHING     hb_BitOr( SDL_INIT_TIMER, SDL_INIT_AUDIO, SDL_INIT_VIDEO, SDL_INIT_EVENTS, SDL_INIT_JOYSTICK, SDL_INIT_HAPTIC, SDL_INIT_GAMECONTROLLER, SDL_INIT_SENSOR )

#endif /* End HBSDL2_CH_ */
