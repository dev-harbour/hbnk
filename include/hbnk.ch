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

#endif /* End HBNK_CH_ */

