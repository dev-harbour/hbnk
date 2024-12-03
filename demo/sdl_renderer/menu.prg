/*
 *
 */

#include "hbnksdl2/hbnksdl2.ch"
#include "hbnk.ch"

PROCEDURE Main()

   LOCAL pWin
   LOCAL pRenderer
   LOCAL pCtx
   LOCAL pEvent
   LOCAL lQuit := .F.
   LOCAL nWindowFlags
   LOCAL nRendererFlags
   LOCAL nFlags

   SDL_SetHint( SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0" )
   SDL_Init( SDL_INIT_VIDEO )

   nWindowFlags := hb_BitOr( SDL_WINDOW_SHOWN, SDL_WINDOW_ALLOW_HIGHDPI, SDL_WINDOW_RESIZABLE )
   pWin := SDL_CreateWindow( "Sample Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 450, nWindowFlags )
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

   pCtx := nk_sdl_init( pWin, pRenderer )

   hb_cdpSelect( "UTF8EX" )
   hb_SetTermCP( hb_cdpTerm() )

   /* NIL select default font or provide path to font */
   hbnk_LoadFonts( pCtx, "9x18.ttf", 18, "CP852" )

   DO WHILE( !lQuit )

      nk_input_begin( pCtx )
      DO WHILE( SDL_PollEvent( @pEvent ) )

         IF( sdl_Event_Type( pEvent ) == SDL_QUIT )
            lQuit := .T.
         ENDIF

         nk_sdl_handle_event( @pEvent )
      ENDDO
      nk_input_end( pCtx )

      /* GUI */
      nFlags := hb_BitOr( NK_WINDOW_BORDER, NK_WINDOW_MOVABLE, NK_WINDOW_SCALABLE, NK_WINDOW_MINIMIZABLE, NK_WINDOW_TITLE )
      IF( nk_begin( pCtx, "Main Window", nk_rect( 50, 50, 600, 400 ), nFlags ) )

         // Menu bar
         nk_menubar_begin( pCtx )
         nk_layout_row_static( pCtx, 25, 80, 3 )

         IF( nk_menu_begin_label( pCtx, "File", NK_TEXT_LEFT, nk_vec2( 120, 200 ) ) )

            nk_layout_row_dynamic( pCtx, 30, 1 )
            IF( nk_menu_item_label( pCtx, "New", NK_TEXT_LEFT ) )
               OutStd( e"\nNew clicked!" )
            ENDIF
            IF( nk_menu_item_label( pCtx, "Open", NK_TEXT_LEFT ) )
               OutStd( e"\nOpen clicked!" )
            ENDIF
            IF( nk_menu_item_label( pCtx, "Exit", NK_TEXT_LEFT ) )
               lQuit = 0
            ENDIF
            nk_menu_end( pCtx )
         ENDIF

         IF( nk_menu_begin_label( pCtx, "Edit", NK_TEXT_LEFT, nk_vec2( 120, 200 ) ) )

            nk_layout_row_dynamic( pCtx, 30, 1 )
            IF( nk_menu_item_label( pCtx, "Cut", NK_TEXT_LEFT ) )
               OutStd( e"\nCut clicked!" )
            ENDIF
            IF( nk_menu_item_label( pCtx, "Copy", NK_TEXT_LEFT ) )
               OutStd( e"\nCopy clicked!" )
            ENDIF
            IF( nk_menu_item_label( pCtx, "Paste", NK_TEXT_LEFT ) )
               OutStd( e"\nPaste clicked!" )
            ENDIF
             nk_menu_end( pCtx )
         ENDIF

         IF( nk_menu_begin_label( pCtx, "Help", NK_TEXT_LEFT, nk_vec2( 120, 200 ) ) )

            nk_layout_row_dynamic( pCtx, 30, 1 )
            IF( nk_menu_item_label( pCtx, "About", NK_TEXT_LEFT ) )
               OutStd( e"\nAbout clicked!" )
            ENDIF
            nk_menu_end( pCtx )
         ENDIF

         nk_menubar_end( pCtx )
      ENDIF
      nk_end( pCtx )

      SDL_SetRenderDrawColor( pRenderer, 26, 46, 61, 255 )
      SDL_RenderClear( pRenderer )

      nk_sdl_render( NK_ANTI_ALIASING_ON )
      SDL_RenderPresent( pRenderer )

   ENDDO

RETURN

