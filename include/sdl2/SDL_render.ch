#ifndef SDL_RENDER_CH_
#define SDL_RENDER_CH_

#define    SDL_RENDERER_SOFTWARE       0x00000001   /**< The renderer is a software fallback */
#define    SDL_RENDERER_ACCELERATED    0x00000002   /**< The renderer uses hardware acceleration */
#define    SDL_RENDERER_PRESENTVSYNC   0x00000004   /**< Present is synchronized with the refresh rate */
#define    SDL_RENDERER_TARGETTEXTURE  0x00000008   /**< The renderer supports rendering to texture */

#define    SDL_ScaleModeNearest                 0   /**< nearest pixel sampling */
#define    SDL_ScaleModeLinear                  1   /**< linear filtering */
#define    SDL_ScaleModeBest                    2   /**< anisotropic filtering */

#define    SDL_TEXTUREACCESS_STATIC                  /**< Changes rarely, not lockable */
#define    SDL_TEXTUREACCESS_STREAMING               /**< Changes frequently, lockable */
#define    SDL_TEXTUREACCESS_TARGET                  /**< Texture can be used as a render target */

#define    SDL_TEXTUREMODULATE_NONE    0x00000000    /**< No modulation */
#define    SDL_TEXTUREMODULATE_COLOR   0x00000001    /**< srcC = srcC * color */
#define    SDL_TEXTUREMODULATE_ALPHA   0x00000002    /**< srcA = srcA * alpha */

#define    SDL_FLIP_NONE               0x00000000    /**< Do not flip */
#define    SDL_FLIP_HORIZONTAL         0x00000001    /**< flip horizontally */
#define    SDL_FLIP_VERTICAL           0x00000002    /**< flip vertically */

#endif /* SDL_RENDER_CH_ */


