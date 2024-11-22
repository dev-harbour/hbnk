#ifndef SDL_EVENTS_CH_
#define SDL_EVENTS_CH_

/* General keyboard/mouse state definitions */
#define SDL_RELEASED    0
#define SDL_PRESSED     1

#define SDL_FIRSTEVENT  0

/* Application events */
#define SDL_QUIT                     0x100

/* These application events have special meaning on iOS, see README-ios.md for details */
#define SDL_APP_TERMINATING          0x101
#define SDL_APP_LOWMEMORY            0x102
#define SDL_APP_WILLENTERBACKGROUND  0x103
#define SDL_APP_DIDENTERBACKGROUND   0x104
#define SDL_APP_WILLENTERFOREGROUND  0x105
#define SDL_APP_DIDENTERFOREGROUND   0x106
#define SDL_LOCALECHANGED            0x107

/* Display events */
#define SDL_DISPLAYEVENT             0x150

/* Window events */
#define SDL_WINDOWEVENT              0x200
#define SDL_SYSWMEVENT               0x201

/* Keyboard events */
#define SDL_KEYDOWN                  0x300
#define SDL_KEYUP                    0x301
#define SDL_TEXTEDITING              0x302
#define SDL_TEXTINPUT                0x303
#define SDL_KEYMAPCHANGED            0x304

/* Mouse events */
#define SDL_MOUSEMOTION              0x400
#define SDL_MOUSEBUTTONDOWN          0x401
#define SDL_MOUSEBUTTONUP            0x402
#define SDL_MOUSEWHEEL               0x403

/* Joystick events */
#define SDL_JOYAXISMOTION            0x600
#define SDL_JOYBALLMOTION            0x601
#define SDL_JOYHATMOTION             0x602
#define SDL_JOYBUTTONDOWN            0x603
#define SDL_JOYBUTTONUP              0x604
#define SDL_JOYDEVICEADDED           0x605
#define SDL_JOYDEVICEREMOVED         0x606

/* Game controller events */
#define SDL_CONTROLLERAXISMOTION     0x650
#define SDL_CONTROLLERBUTTONDOWN     0x651
#define SDL_CONTROLLERBUTTONUP       0x652
#define SDL_CONTROLLERDEVICEADDED    0x653
#define SDL_CONTROLLERDEVICEREMOVED  0x654
#define SDL_CONTROLLERDEVICEREMAPPED 0x655
#define SDL_CONTROLLERTOUCHPADDOWN   0x656
#define SDL_CONTROLLERTOUCHPADMOTION 0x657
#define SDL_CONTROLLERTOUCHPADUP     0x658
#define SDL_CONTROLLERSENSORUPDATE   0x659

/* Touch events */
#define SDL_FINGERDOWN               0x700
#define SDL_FINGERUP                 0x701
#define SDL_FINGERMOTION             0x702

/* Gesture events */
#define SDL_DOLLARGESTURE            0x800
#define SDL_DOLLARRECORD             0x801
#define SDL_MULTIGESTURE             0x802

/* Clipboard events */
#define SDL_CLIPBOARDUPDATE          0x900

/* Drag and drop events */
#define SDL_DROPFILE                 0x1000
#define SDL_DROPTEXT                 0x1001
#define SDL_DROPBEGIN                0x1002
#define SDL_DROPCOMPLETE             0x1003

/* Audio hotplug events */
#define SDL_AUDIODEVICEADDED         0x1100
#define SDL_AUDIODEVICEREMOVED       0x1101

/* Sensor events */
#define SDL_SENSORUPDATE             0x1200

/* Render events */
#define SDL_RENDER_TARGETS_RESET     0x2000
#define SDL_RENDER_DEVICE_RESET      0x2001

/* Internal events */
#define SDL_POLLSENTINEL             0x7F00

#define SDL_USEREVENT                0x8000
#define SDL_LASTEVENT                0xFFFF

#endif /* End SDL_EVENTS_CH_ */
