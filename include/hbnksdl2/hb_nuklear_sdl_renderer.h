/*
 *
 */

#ifndef HB_NUKLEAR_SDL_RENDERER_H_
#define HB_NUKLEAR_SDL_RENDERER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include <time.h>

#include "hbapi.h"
#include "hbapierr.h"
#include "hbapiitm.h"
#include "hbapistr.h"

#include <SDL2/SDL.h>

#include "../nuklear.h"
#include "nuklear_sdl_renderer.h"

typedef enum _bool bool;

enum _bool
{
   F = 0,
   T = ( ! 0 )
};

HB_EXTERN_BEGIN
extern HB_EXPORT struct nk_context *hb_nk_context_Param( int iParam );
extern HB_EXPORT struct nk_context *hb_nk_context_ParamGet( int iParam );
extern HB_EXPORT struct nk_context *hb_nk_context_ItemGet( PHB_ITEM pItem );
extern HB_EXPORT PHB_ITEM           hb_nk_context_ItemPut( PHB_ITEM pItem, struct nk_context *pContext );
extern HB_EXPORT void               hb_nk_context_ItemClear( PHB_ITEM pItem );
extern HB_EXPORT void               hb_nk_context_StorPtr( struct nk_context *pContext, int iParam );
extern HB_EXPORT void               hb_nk_context_Return( struct nk_context *pContext );

extern HB_EXPORT struct nk_input   *hb_nk_input_Param( int iParam );
extern HB_EXPORT PHB_ITEM           hb_nk_input_ItemPut( PHB_ITEM pItem, struct nk_input *pInput );
extern HB_EXPORT void               hb_nk_input_StorPtr( struct nk_input *pInput, int iParam );
extern HB_EXPORT void               hb_nk_input_Return( struct nk_input *pInput );
HB_EXTERN_END

#define HB_ERR_ARGS() ( hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS ) )

#define MAX( a, b ) \
   ( { typeof( a ) _a = ( a ); typeof( b ) _b = ( b ); _a < _b ? _b : _a; } )
#define MIN( a, b ) \
   ( { typeof( a ) _a = ( a ); typeof( b ) _b = ( b ); _a < _b ? _a : _b; } )
#define UNUSED( n ) ( ( void )( n ) )
#define LEN( n ) ( sizeof( n ) / sizeof( ( n )[ 0 ] ) )
#define IIF( condition, trueValue, falseValue ) ( ( condition ) ? ( trueValue ) : ( falseValue ) )

#endif /* End HB_NUKLEAR_SDL_RENDERER_H_ */
