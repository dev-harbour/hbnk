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
   hbnk_LoadFonts( pContext, NIL, 13, "CP852" )

   DO WHILE( !lQuit )

      nk_input_begin( pContext )
      DO WHILE( SDL_PollEvent( @pEvent ) )

         IF( sdl_Event_Type( pEvent ) == SDL_QUIT )
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

      calculator( pContext )

      SDL_SetRenderDrawColor( pRenderer, 26, 46, 61, 255 )
      SDL_RenderClear( pRenderer )

      nk_sdl_render( NK_ANTI_ALIASING_ON )
      SDL_RenderPresent( pRenderer )

   ENDDO

RETURN

#pragma BEGINDUMP

#include "./../include/hbnksdl2/hb_nuklear_sdl_renderer.h"

HB_FUNC( CALCULATOR )
{
   struct nk_context *ctx = hb_nk_context_Param( 1 );

   if( nk_begin( ctx, "Calculator", nk_rect( 10, 10, 180, 250 ), NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_MOVABLE ) )
   {
      static int set = 0, prev = 0, op = 0;
      static const char numbers[] = "789456123";
      static const char ops[] = "+-*/";
      static double a = 0, b = 0;
      static double *current = &a;

      size_t i = 0;
      int solve = 0;
      {
         int len; char buffer[ 256 ];
         nk_layout_row_dynamic( ctx, 35, 1 );
         len = snprintf( buffer, 256, "%.2f", *current );
         nk_edit_string( ctx, NK_EDIT_SIMPLE, buffer, &len, 255, nk_filter_float );
         buffer[ len ] = 0;
        *current = atof( buffer );
      }

      nk_layout_row_dynamic( ctx, 35, 4 );
      for( i = 0; i < 16; ++i )
      {
         if( i >= 12 && i < 15 )
         {
            if( i > 12 ) continue;
            if( nk_button_label( ctx, "C" ) )
            {
               a = b = op = 0; current = &a; set = 0;
            }
            if( nk_button_label( ctx, "0" ) )
            {
              *current = *current * 10.0f; set = 0;
            }
            if( nk_button_label( ctx, "=" ) )
            {
               solve = 1; prev = op; op = 0;
            }
         }
         else if( ( ( i + 1 ) % 4 ) )
         {
            if( nk_button_text( ctx, &numbers[ ( i / 4 ) * 3 + i % 4 ], 1 ) )
            {
              *current = *current * 10.0f + numbers[ ( i / 4 ) * 3 + i % 4 ] - '0';
               set = 0;
            }
         }
         else if( nk_button_text( ctx, &ops[ i / 4 ], 1 ) )
         {
            if( !set )
            {
               if( current != &b )
               {
                  current = &b;
               }
               else
               {
                  prev = op;
                  solve = 1;
               }
            }
            op = ops[ i / 4 ];
            set = 1;
         }
      }
      if( solve  )
      {
         if( prev == '+' ) a = a + b;
         if( prev == '-' ) a = a - b;
         if( prev == '*' ) a = a * b;
         if( prev == '/' ) a = a / b;
         current = &a;
         if( set ) current = &b;
         b = 0;
         set = 0;
       }
   }
   nk_end( ctx );
}

#pragma ENDDUMP