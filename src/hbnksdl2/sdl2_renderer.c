/*
 * Harbour Nuklear (HBNK) Project
 * Copyright 2014 - 2024 Rafał Jopek
 * Website: https://harbour.pl
 *
 */

#include "hb_nuklear_sdl_renderer.h"

/* -------------------------------------------------------------------------
Garbage Collector SDL_Window
------------------------------------------------------------------------- */
static HB_GARBAGE_FUNC( hb_sdl_window_Destructor )
{
   SDL_Window **ppSDL_Window = ( SDL_Window ** ) Cargo;

   if( ppSDL_Window && *ppSDL_Window )
   {
      SDL_DestroyWindow( *ppSDL_Window );

      SDL_QuitSubSystem( SDL_INIT_VIDEO  );
      SDL_Quit();

      *ppSDL_Window = NULL;
   }
}

static const HB_GC_FUNCS s_gc_sdl_window_Funcs =
{
   hb_sdl_window_Destructor,
   hb_gcDummyMark
};

SDL_Window *hb_sdl_window_Param( int iParam )
{
   SDL_Window **ppSDL_Window = ( SDL_Window ** ) hb_parptrGC( &s_gc_sdl_window_Funcs, iParam );

   if( ppSDL_Window && *ppSDL_Window )
   {
      return *ppSDL_Window;
   }
   else
   {
      HB_ERR_ARGS();
      return NULL;
   }
}

PHB_ITEM hb_sdl_window_ItemPut( PHB_ITEM pItem, SDL_Window *pSDL_Window )
{
   SDL_Window **ppSDL_Window = ( SDL_Window ** ) hb_gcAllocate( sizeof( SDL_Window * ), &s_gc_sdl_window_Funcs );

   *ppSDL_Window = pSDL_Window;
   return hb_itemPutPtrGC( pItem, ppSDL_Window );
}

void hb_sdl_window_Return( SDL_Window *pSDL_Window )
{
   if( pSDL_Window )
   {
      hb_sdl_window_ItemPut( hb_param( -1, HB_IT_ANY ), pSDL_Window );
   }
   else
   {
      hb_ret();
   }
}

/* -------------------------------------------------------------------------
Garbage Collector SDL_Renderer
------------------------------------------------------------------------- */
static HB_GARBAGE_FUNC( hb_sdl_renderer_Destructor )
{
   SDL_Renderer **ppSDL_Renderer = ( SDL_Renderer ** ) Cargo;

   if( ppSDL_Renderer && *ppSDL_Renderer )
   {
      SDL_DestroyRenderer( *ppSDL_Renderer );

      *ppSDL_Renderer = NULL;
   }
}

static const HB_GC_FUNCS s_gc_sdl_renderer_Funcs =
{
   hb_sdl_renderer_Destructor,
   hb_gcDummyMark
};

SDL_Renderer *hb_sdl_renderer_Param( int iParam )
{
   SDL_Renderer **ppSDL_Renderer = ( SDL_Renderer ** ) hb_parptrGC( &s_gc_sdl_renderer_Funcs, iParam );

   if( ppSDL_Renderer && *ppSDL_Renderer )
   {
      return *ppSDL_Renderer;
   }
   else
   {
      HB_ERR_ARGS();
      return NULL;
   }
}

PHB_ITEM hb_sdl_renderer_ItemPut( PHB_ITEM pItem, SDL_Renderer *pSDL_Renderer )
{
   SDL_Renderer **ppSDL_Renderer = ( SDL_Renderer ** ) hb_gcAllocate( sizeof( SDL_Renderer * ), &s_gc_sdl_renderer_Funcs );

   *ppSDL_Renderer = pSDL_Renderer;
   return hb_itemPutPtrGC( pItem, ppSDL_Renderer );
}

void hb_sdl_renderer_Return( SDL_Renderer *pSDL_Renderer )
{
   if( pSDL_Renderer )
   {
      hb_sdl_renderer_ItemPut( hb_param( -1, HB_IT_ANY ), pSDL_Renderer );
   }
   else
   {
      hb_ret();
   }
}

/* -------------------------------------------------------------------------
Garbage Collector SDL_Event
------------------------------------------------------------------------- */
static HB_GARBAGE_FUNC( hb_sdl_event_Destructor )
{
   SDL_Event **ppEvent = ( SDL_Event ** ) Cargo;

   if( ppEvent && *ppEvent )
   {
      hb_xfree( *ppEvent );
      *ppEvent = NULL;
   }
}

static const HB_GC_FUNCS s_gc_sdl_event_Funcs =
{
   hb_sdl_event_Destructor,
   hb_gcDummyMark
};

SDL_Event *hb_sdl_event_Param( int iParam )
{
   SDL_Event **ppSDL_Event = ( SDL_Event ** ) hb_parptrGC( &s_gc_sdl_event_Funcs, iParam );

   if( ppSDL_Event && *ppSDL_Event )
      return *ppSDL_Event;

   HB_ERR_ARGS();
   return NULL;
}

SDL_Event *hb_sdl_event_ParamGet( int iParam )
{
   SDL_Event **ppSDL_Event = ( SDL_Event ** ) hb_parptrGC( &s_gc_sdl_event_Funcs, iParam );

   return ppSDL_Event ? *ppSDL_Event : NULL;
}

SDL_Event *hb_sdl_event_ItemGet( PHB_ITEM pItem )
{
   SDL_Event **ppSDL_Event = ( SDL_Event ** ) hb_itemGetPtrGC( pItem, &s_gc_sdl_event_Funcs );

   return ppSDL_Event ? *ppSDL_Event : NULL;
}

PHB_ITEM hb_sdl_event_ItemPut( PHB_ITEM pItem, SDL_Event *pSDL_Event )
{
   SDL_Event **ppSDL_Event = ( SDL_Event ** ) hb_gcAllocate( sizeof( SDL_Event * ), &s_gc_sdl_event_Funcs );

   *ppSDL_Event = pSDL_Event;
   return hb_itemPutPtrGC( pItem, ppSDL_Event );
}

void hb_sdl_event_ItemClear( PHB_ITEM pItem )
{
   SDL_Event **ppSDL_Event = ( SDL_Event ** ) hb_itemGetPtrGC( pItem, &s_gc_sdl_event_Funcs );

   if( ppSDL_Event )
      *ppSDL_Event = NULL;
}

void hb_sdl_event_StorPtr( SDL_Event *pSDL_Event, int iParam )
{
   SDL_Event **ppSDL_Event = ( SDL_Event ** ) hb_gcAllocate( sizeof( SDL_Event * ), &s_gc_sdl_event_Funcs );

   *ppSDL_Event = pSDL_Event;
   hb_storptrGC( ppSDL_Event, iParam );
}

void hb_sdl_event_Return( SDL_Event *pSDL_Event )
{
   if( pSDL_Event )
      hb_sdl_event_ItemPut( hb_param( -1, HB_IT_ANY ), pSDL_Event );
   else
      hb_ret();
}

void hb_sdl_event_Return( SDL_Event *pSDL_Event ) __attribute__( ( unused ) );

/* -------------------------------------------------------------------------
Garbage Collector for nk_sdl_shutdown
------------------------------------------------------------------------- */
static HB_GARBAGE_FUNC( hb_nuklear_sdl_renderer_Destructor )
{
   void **pp = ( void ** ) Cargo;

   if( pp && *pp )
   {
      nk_sdl_shutdown();
      *pp = NULL;
   }
}

__attribute__( ( unused ) ) static const HB_GC_FUNCS s_gc_hb_nuklear_sdl_renderer_Funcs =
{
   hb_nuklear_sdl_renderer_Destructor,
   hb_gcDummyMark
};

/* -------------------------------------------------------------------------
SDL API
------------------------------------------------------------------------- */
// SDL_Window *SDL_CreateWindow( const char *title, int x, int y, int w, int h, Uint32 flags );
HB_FUNC( SDL_CREATEWINDOW )
{
   if( hb_param( 1, HB_IT_STRING ) != NULL &&
       hb_param( 2, HB_IT_NUMERIC ) != NULL &&
       hb_param( 3, HB_IT_NUMERIC ) != NULL &&
       hb_param( 4, HB_IT_NUMERIC ) != NULL &&
       hb_param( 5, HB_IT_NUMERIC ) != NULL &&
       hb_param( 6, HB_IT_NUMERIC ) != NULL )
   {
      hb_sdl_window_Return( SDL_CreateWindow( hb_parc( 1 ), hb_parni( 2 ), hb_parni( 3 ), hb_parni( 4 ), hb_parni( 5 ), ( Uint32 ) hb_parni( 6 ) ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// void SDL_GetWindowSize( SDL_Window *window, int *w, int *h );
HB_FUNC( SDL_GETWINDOWSIZE )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_BYREF ) != NULL && hb_param( 3, HB_IT_BYREF ) != NULL )
   {
      SDL_Window *pWindow = hb_sdl_window_Param( 1 );
      int w, h;
      SDL_GetWindowSize( pWindow, &w, &h );
      hb_stornint( w, 2 );
      hb_stornint( h, 3 );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// int SDLCALL SDL_Init( Uint32 flags );
HB_FUNC( SDL_INIT )
{
   if( hb_param( 1, HB_IT_NUMERIC ) != NULL )
   {
      hb_retni( SDL_Init( ( Uint32 ) hb_parnl( 1 ) ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// int SDL_WaitEvent( SDL_Event *event )
HB_FUNC( SDL_WAITEVENT )
{
   if( hb_param( 1, HB_IT_BYREF ) != NULL )
   {
      SDL_Event *pEvent = ( SDL_Event * ) hb_xgrab( sizeof( SDL_Event ) );

      if( SDL_WaitEvent( pEvent ) )
      {
         hb_retl( T );
         hb_sdl_event_StorPtr( pEvent, 1 );
      }
      else
      {
         hb_xfree( pEvent );
         hb_retl( F );
      }
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// int SDL_PollEvent( SDL_Event *event );
HB_FUNC( SDL_POLLEVENT )
{
   if( hb_param( 1, HB_IT_BYREF ) != NULL )
   {
      SDL_Event *pEvent = ( SDL_Event * ) hb_xgrab( sizeof( SDL_Event ) );

      if( SDL_PollEvent( pEvent ) )
      {
         hb_retl( T );
         hb_sdl_event_StorPtr( pEvent, 1 );
      }
      else
      {
         hb_xfree( pEvent );
         hb_retl( F );
      }
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// int sdl_Event_Type( SDL_Event *event )
HB_FUNC( SDL_EVENT_TYPE )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      SDL_Event *pEvent = hb_sdl_event_Param( 1 );

      hb_retni( pEvent->type );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// int sdl_event_window_data1( SDL_Event *event )
HB_FUNC( SDL_EVENT_WINDOW_DATA1 )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      SDL_Event *pEvent = hb_sdl_event_Param( 1 );

      hb_retni( pEvent->window.data1 );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// int sdl_event_window_data2( SDL_Event *event )
HB_FUNC( SDL_EVENT_WINDOW_DATA2 )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      SDL_Event *pEvent = hb_sdl_event_Param( 1 );

      hb_retni( pEvent->window.data2 );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// int sdl_EventWindowEvent( SDL_Event *pEvent )
HB_FUNC( SDL_EVENTWINDOWEVENT )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      SDL_Event *pEvent = hb_sdl_event_Param( 1 );

      hb_retni( pEvent->window.event );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// SDL_bool SDLCALL SDL_SetHint( const char *name, const char *value );
HB_FUNC( SDL_SETHINT )
{
   if( hb_param( 1, HB_IT_STRING ) != NULL && hb_param( 2, HB_IT_STRING ) != NULL )
   {
      hb_retl( SDL_SetHint( hb_parc( 1 ), hb_parc( 2 ) ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// SDL_Renderer *SDL_CreateRenderer( SDL_Window *window, int index, Uint32 flags );
HB_FUNC( SDL_CREATERENDERER )
{
    if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_NUMERIC ) != NULL && hb_param( 3, HB_IT_NUMERIC ) != NULL )
   {
      SDL_Window *pWindow = hb_sdl_window_Param( 1 );
      int index = hb_parni( 2 );
      Uint32 flags = hb_parni( 3 );

      hb_sdl_renderer_Return( SDL_CreateRenderer( pWindow, index, flags ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// int SDL_GetRendererOutputSize( SDL_Renderer *renderer, int *w, int *h );
HB_FUNC( SDL_GETRENDEREROUTPUTSIZE )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_BYREF ) != NULL && hb_param( 3, HB_IT_BYREF ) != NULL )
   {
      SDL_Renderer *pRenderer = hb_sdl_renderer_Param( 1 );
      int w, h;
      hb_retni( SDL_GetRendererOutputSize( pRenderer, &w, &h ) );
      hb_stornint( w, 2 );
      hb_stornint( h, 3 );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// int SDL_RenderSetScale( SDL_Renderer *renderer, float scaleX, float scaleY );
HB_FUNC( SDL_RENDERSETSCALE )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_NUMERIC ) != NULL && hb_param( 3, HB_IT_NUMERIC ) != NULL )
   {
      SDL_Renderer *pRenderer = hb_sdl_renderer_Param( 1 );
      float scaleX = hb_parnd( 2 );
      float scaleY = hb_parnd( 3 );

      hb_retni( SDL_RenderSetScale( pRenderer, scaleX, scaleY ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

//int SDL_SetRenderDrawColor( SDL_Renderer * renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a );
HB_FUNC( SDL_SETRENDERDRAWCOLOR )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_NUMERIC ) != NULL && hb_param( 3, HB_IT_NUMERIC ) != NULL && hb_param( 4, HB_IT_NUMERIC ) != NULL && hb_param( 5, HB_IT_NUMERIC ) != NULL )
   {
      SDL_Renderer *pRenderer = hb_sdl_renderer_Param( 1 );
      Uint8 r = ( Uint8 ) hb_parni( 2 );
      Uint8 g = ( Uint8 ) hb_parni( 3 );
      Uint8 b = ( Uint8 ) hb_parni( 4 );
      Uint8 a = ( Uint8 ) hb_parni( 5 );

      hb_retni( SDL_SetRenderDrawColor( pRenderer, r, g, b, a ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// int SDL_RenderClear( SDL_Renderer * renderer );
HB_FUNC( SDL_RENDERCLEAR )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      SDL_Renderer *pRenderer = hb_sdl_renderer_Param( 1 );

      hb_retni( SDL_RenderClear( pRenderer ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// void SDL_RenderPresent( SDL_Renderer *renderer );
HB_FUNC( SDL_RENDERPRESENT )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      SDL_Renderer *pRenderer = hb_sdl_renderer_Param( 1 );

      SDL_RenderPresent( pRenderer );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// const char* SDL_GetError( void );
HB_FUNC( SDL_GETERROR )
{
   hb_retc( SDL_GetError() );
}

/* -------------------------------------------------------------------------
API nuklear sdl renderer
------------------------------------------------------------------------- */
// struct nk_context *nk_sdl_init( SDL_Window *win, SDL_Renderer *renderer );
HB_FUNC( NK_SDL_INIT )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_POINTER ) != NULL )
   {
      SDL_Window *pWindow = hb_sdl_window_Param( 1 );
      SDL_Renderer *pRenderer = hb_sdl_renderer_Param( 2 );

      hb_nk_context_Return( nk_sdl_init( pWindow, pRenderer ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// ---
// void nk_sdl_font_stash_begin( struct nk_font_atlas **atlas );
// Implementation in Harbour function hbnk_LoadFonts
// ---
// void nk_sdl_font_stash_end( void );
// Implementation in Harbour function hbnk_LoadFonts
// ---

// int nk_sdl_handle_event( SDL_Event *event );
HB_FUNC( NK_SDL_HANDLE_EVENT )
{
   SDL_Event *pEvent = hb_sdl_event_Param( 1 );

   if( pEvent )
   {
      hb_retni( nk_sdl_handle_event( pEvent ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// void nk_sdl_render( enum nk_anti_aliasing );
HB_FUNC( NK_SDL_RENDER )
{
   if( hb_param( 1, HB_IT_NUMERIC ) != NULL )
   {
      nk_sdl_render( hb_parni( 1 ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// void nk_sdl_shutdown( void )
HB_FUNC( NK_SDL_SHUTDOWN )
{
   nk_sdl_shutdown();
}

// void nk_sdl_handle_grab( void );
HB_FUNC( NK_SDL_HANDLE_GRAB )
{
   nk_sdl_handle_grab();
}
