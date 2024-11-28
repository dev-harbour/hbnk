/*
 *
 */

#include "hbnksdl2/hbnksdl2.ch"
#include "hbnk.ch"

#define EASY 0
#define HARD 1

#define R 1
#define G 2
#define B 3
#define A 4

PROCEDURE Main()

   LOCAL pWin
   LOCAL pRenderer
   LOCAL pCtx
   LOCAL pEvent
   LOCAL lQuit := .F.
   LOCAL nWindowFlags
   LOCAL nRendererFlags
   LOCAL nFlags
   LOCAL nOp := EASY
   LOCAL nProperty := 20
   LOCAL aBg := { 0.10, 0.18, 0.24, 1.0 }

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

   pCtx := nk_sdl_init( pWin, pRenderer )

   hb_cdpSelect( "UTF8EX" )
   hb_SetTermCP( hb_cdpTerm() )

   /* NIL select default font or provide path to font */
   hbnk_LoadFonts( pCtx, "9x18.ttf", 18, "CP852" )

   DO WHILE( !lQuit )

      nk_input_begin( pCtx )
      DO WHILE( SDL_PollEvent( @pEvent ) )

         IF( sdl_EventType( pEvent ) == SDL_QUIT )
            lQuit := .T.
         ENDIF

         nk_sdl_handle_event( @pEvent )
      ENDDO
      nk_input_end( pCtx )

      /* GUI */
      nFlags := hb_BitOr( NK_WINDOW_BORDER, NK_WINDOW_MOVABLE, NK_WINDOW_SCALABLE, NK_WINDOW_MINIMIZABLE, NK_WINDOW_TITLE )
      IF( nk_begin( pCtx, "ĄĆĘŁŃÓŚŹŻ", { 50, 50, 230, 250 }, nFlags ) )
      ENDIF
      nk_end( pCtx )

      IF( nk_begin( pCtx, "Hello Harbour Nuklear!", { 50, 50, 230, 250 }, nFlags ) )
      ENDIF
      nk_end( pCtx )

      nFlags := NK_WINDOW_BORDER + NK_WINDOW_MOVABLE + NK_WINDOW_SCALABLE + NK_WINDOW_MINIMIZABLE + NK_WINDOW_TITLE
      IF( nk_begin( pCtx, "Demo", nk_rect( 50, 50, 230, 250 ), nFlags ) )

         nk_layout_row_static( pCtx, 30, 80, 1 )
         IF( nk_button_label( pCtx, "button" ) )
            OutStd( e"\nbutton pressed" )
         ENDIF

         nk_layout_row_dynamic( pCtx, 30, 2 )
         IF( nk_option_label( pCtx, "easy", nOp == EASY ) )
            nOp := EASY
         ENDIF
         IF( nk_option_label( pCtx, "hard", nOp == HARD ) )
            nOp := HARD
         ENDIF

         nk_layout_row_dynamic( pCtx, 25, 1 )
         nk_property_int( pCtx, "Compression:", 0, @nProperty, 100, 10, 1 )

         nk_layout_row_dynamic( pCtx, 20, 1 )
         nk_label( pCtx, "background:", NK_TEXT_LEFT )
         nk_layout_row_dynamic( pCtx, 25, 1 )
         IF( nk_combo_begin_color( pCtx, nk_rgb_cf( aBg ), nk_vec2( nk_widget_width( pCtx ), 400 ) ) )
            nk_layout_row_dynamic( pCtx, 120, 1 )
            aBg := nk_color_picker( pCtx, aBg, NK_RGBA )
            nk_layout_row_dynamic( pCtx, 25, 1 )
            aBg[ R ] := nk_propertyf( pCtx, "#R:", 0, aBg[ 1 ], 1.0, 0.01, 0.005 )
            aBg[ G ] := nk_propertyf( pCtx, "#G:", 0, aBg[ 2 ], 1.0, 0.01, 0.005 )
            aBg[ B ] := nk_propertyf( pCtx, "#B:", 0, aBg[ 3 ], 1.0, 0.01, 0.005 )
            aBg[ A ] := nk_propertyf( pCtx, "#A:", 0, aBg[ 4 ], 1.0, 0.01, 0.005 )
            nk_combo_end( pCtx )
         ENDIF
      ENDIF
      nk_end( pCtx )

      SDL_SetRenderDrawColor( pRenderer, aBg[ R ] * 255, aBg[ G ] * 255, aBg[ B ] * 255, aBg[ A ] * 255  )
      SDL_RenderClear( pRenderer )

      nk_sdl_render( NK_ANTI_ALIASING_ON )
      SDL_RenderPresent( pRenderer )

   ENDDO

RETURN

