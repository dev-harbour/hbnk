/*
 *
 */

#include "hbnksdl2/hbnksdl2.ch"
#include "hbnk.ch"

PROCEDURE Main()

   LOCAL pWin
   LOCAL pRenderer
   LOCAL pContext
   LOCAL pEvent
   LOCAL lQuit := .F.
   LOCAL nWindowFlags
   LOCAL nRendererFlags
   LOCAL nFlags

   SDL_SetHint( SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0" )
   SDL_Init( SDL_INIT_VIDEO )

   nWindowFlags := hb_BitOr( SDL_WINDOW_SHOWN, SDL_WINDOW_ALLOW_HIGHDPI, SDL_WINDOW_RESIZABLE )
   pWin := SDL_CreateWindow( "Welcome to Harbour + Nuklear Integration!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 450, nWindowFlags )
   IF( pWin == NIL )
      OutStd( e"\nError SDL_CreateWindow", SDL_GetError() )
      RETURN
   ENDIF

   nRendererFlags := hb_BitOr( SDL_RENDERER_ACCELERATED, SDL_RENDERER_PRESENTVSYNC )
   pRenderer := SDL_CreateRenderer( pWin, -1, nRendererFlags )
   IF( pRenderer == NIL )
      OutStd( e"\nError SDL_CreateRenderer", SDL_GetError() )
      RETURN
   ENDIF

   pContext := nk_sdl_init( pWin, pRenderer )

   /* NIL select default font or provide path to font */
   hbnk_LoadFonts( pContext, NIL, 13 )

   DO WHILE( !lQuit )

      nk_input_begin( pContext )
      DO WHILE( SDL_PollEvent( @pEvent ) )

         IF( sdl_EventType( pEvent ) == SDL_QUIT )
            lQuit := .T.
         ENDIF

         nk_sdl_handle_event( @pEvent )
      ENDDO
      nk_input_end( pContext )

      /* GUI */
      nFlags := hb_BitOr( NK_WINDOW_BORDER, NK_WINDOW_MOVABLE, NK_WINDOW_SCALABLE, NK_WINDOW_MINIMIZABLE, NK_WINDOW_TITLE )
      IF( nk_begin( pContext, "Hello Harbour Nuklear!", { 50, 50, 230, 250 }, nFlags ) )
      ENDIF
      nk_end( pContext )

      SDL_SetRenderDrawColor( pRenderer, 26, 46, 61, 255 )
      SDL_RenderClear( pRenderer )

      nk_sdl_render( NK_ANTI_ALIASING_ON )
      SDL_RenderPresent( pRenderer )

   ENDDO

RETURN

