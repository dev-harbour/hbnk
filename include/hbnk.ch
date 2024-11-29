/*
 *
 */

#ifndef HBNK_CH_
#define HBNK_CH_

/* Nuklear */
#define NK_RGB                       0
#define NK_RGBA                      1

#define NK_WINDOW_BORDER             0
#define NK_WINDOW_MOVABLE            1
#define NK_WINDOW_SCALABLE           2
#define NK_WINDOW_CLOSABLE           4
#define NK_WINDOW_MINIMIZABLE        8
#define NK_WINDOW_NO_SCROLLBAR       16
#define NK_WINDOW_TITLE              32
#define NK_WINDOW_SCROLL_AUTO_HIDE   64
#define NK_WINDOW_BACKGROUND         128
#define NK_WINDOW_SCALE_LEFT         256
#define NK_WINDOW_NO_INPUT           512

/* Text */
#define NK_TEXT_ALIGN_LEFT           0x01
#define NK_TEXT_ALIGN_CENTERED       0x02
#define NK_TEXT_ALIGN_RIGHT          0x04
#define NK_TEXT_ALIGN_TOP            0x08
#define NK_TEXT_ALIGN_MIDDLE         0x10
#define NK_TEXT_ALIGN_BOTTOM         0x20

#define NK_TEXT_LEFT                 NK_TEXT_ALIGN_MIDDLE + NK_TEXT_ALIGN_LEFT
#define NK_TEXT_CENTERED             NK_TEXT_ALIGN_MIDDLE + NK_TEXT_ALIGN_CENTERED
#define NK_TEXT_RIGHT                NK_TEXT_ALIGN_MIDDLE + NK_TEXT_ALIGN_RIGHT

#define NK_ANTI_ALIASING_OFF         0
#define NK_ANTI_ALIASING_ON          1

#define NK_KEY_NONE                  0
#define NK_KEY_SHIFT                 1
#define NK_KEY_CTRL                  2
#define NK_KEY_DEL                   3
#define NK_KEY_ENTER                 4
#define NK_KEY_TAB                   5
#define NK_KEY_BACKSPACE             6
#define NK_KEY_COPY                  7
#define NK_KEY_CUT                   8
#define NK_KEY_PASTE                 9
#define NK_KEY_UP                   10
#define NK_KEY_DOWN                 11
#define NK_KEY_LEFT                 12
#define NK_KEY_RIGHT                13
/* Shortcuts: text field */
#define NK_KEY_TEXT_INSERT_MODE     14
#define NK_KEY_TEXT_REPLACE_MODE    15
#define NK_KEY_TEXT_RESET_MODE      16
#define NK_KEY_TEXT_LINE_START      17
#define NK_KEY_TEXT_LINE_END        18
#define NK_KEY_TEXT_START           19
#define NK_KEY_TEXT_END             20
#define NK_KEY_TEXT_UNDO            21
#define NK_KEY_TEXT_REDO            22
#define NK_KEY_TEXT_SELECT_ALL      23
#define NK_KEY_TEXT_WORD_LEFT       24
#define NK_KEY_TEXT_WORD_RIGHT      25
/* Shortcuts: scrollbar */
#define NK_KEY_SCROLL_START         26
#define NK_KEY_SCROLL_END           27
#define NK_KEY_SCROLL_DOWN          28
#define NK_KEY_SCROLL_UP            29
#define NK_KEY_MA                   30

#endif /* End HBNK_CH_ */

