/*
 * Harbour Nuklear (HBNK) Project
 * Copyright 2014 - 2024 Rafał Jopek
 * Website: https://harbour.pl
 *
 */

#define NK_IMPLEMENTATION
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_BUTTON_TRIGGER_ON_RELEASE
#define NK_INCLUDE_STANDARD_IO

/* -------------------------------------------------------------------------
HARBOUR IMPLEMENTATION
------------------------------------------------------------------------- */
#if defined( HBMK_HAS_SDL2 )
   #define NK_SDL_RENDERER_IMPLEMENTATION
   #include "hb_nuklear_sdl_renderer.h"

static const nk_rune *hbnk_set_codepage( const char *codepage );

HB_FUNC( HBNK_LOADFONTS )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 3, HB_IT_NUMERIC ) != NULL )
   {
      struct nk_context *ctx = hb_nk_context_Param( 1 );
      const char *file_path = hb_parc( 2 );
      float height = ( float ) hb_parnd( 3 );
      const char *codepage = hb_parc( 4 );

      struct nk_font_atlas *atlas = NULL;
      struct nk_font_config config = nk_font_config( 0 );
      struct nk_font *font = NULL;

      const nk_rune *glyph_ranges = hbnk_set_codepage( codepage );
      if( !glyph_ranges )
      {
         printf("Error: Invalid codepage.\n");
         hb_retl( F );
         return;
      }
      config.range = glyph_ranges;

      nk_sdl_font_stash_begin( &atlas );

      if( hb_param( 2, HB_IT_NIL ) != NULL || !hb_param( 2, HB_IT_STRING ) )
      {
         font = nk_font_atlas_add_default( atlas, height, &config );
      }
      else
      {
         font = nk_font_atlas_add_from_file( atlas, file_path, height, &config );
      }

      nk_sdl_font_stash_end();

      if( font )
      {
         font->handle.height /= 1.0f;
         nk_style_set_font( ctx, &font->handle );
         hb_retl( T );
      }
      else
      {
         hb_retl( F );
      }
   }
   else
   {
      HB_ERR_ARGS();
   }
}
#endif

static const nk_rune *hbnk_set_codepage( const char *codepage )
{
   if( !codepage )
   {
      printf( "Error: No codepage specified.\n" );
      return NULL;
   }

   /* TODO */
   static const nk_rune cp852_ranges[] = {
      0x0020, 0x007F, // ASCII
      0x0080, 0x00FF, // Latin-1 Supplement
      0x0100, 0x017F, // Latin-Extended-A
      0
   };

   /* TODO */
   static const nk_rune utf8ex_ranges[] =
   {
      0x0020, 0x007F, // ASCII
      0x0080, 0x00FF, // Latin-1 Supplement
      0x0100, 0x017F, // Latin-Extended-A
      0x0180, 0x024F, // Latin-Extended-B
      0
   };

   const nk_rune *glyph_ranges = NULL;

    // Wybór kodowania na podstawie nazwy
   if( strcmp( codepage, "CP852" ) == 0 )
   {
      glyph_ranges = cp852_ranges;
   }
   else if( strcmp( codepage, "UTF8EX" ) == 0 )
   {
      glyph_ranges = utf8ex_ranges;
   }
   else
   {
      printf("Unsupported codepage: %s\n", codepage);
     return NULL;
   }

   return glyph_ranges;
}

//float hbnk_input_mouse_pos_x( struct nk_context *ctx )
HB_FUNC( HBNK_INPUT_MOUSE_POS_X )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      struct nk_context *ctx = hb_nk_context_Param( 1 );
      hb_retnd( ( float ) ctx->input.mouse.pos.x );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

//float hbnk_input_mouse_pos_y( struct nk_context *ctx )
HB_FUNC( HBNK_INPUT_MOUSE_POS_Y )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      struct nk_context *ctx = hb_nk_context_Param( 1 );
      hb_retnd( ( float ) ctx->input.mouse.pos.y );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

/* -------------------------------------------------------------------------
Garbage Collector Nuklear context
------------------------------------------------------------------------- */
static HB_GARBAGE_FUNC( hb_nk_context_Destructor )
{
   struct nk_context **ppContext = ( struct nk_context ** ) Cargo;

   if( ppContext && *ppContext )
   {
      nk_free( *ppContext );

      *ppContext = NULL;
   }
}

static const HB_GC_FUNCS s_gc_nk_context_Funcs =
{
   hb_nk_context_Destructor,
   hb_gcDummyMark
};

struct nk_context *hb_nk_context_Param( int iParam )
{
   struct nk_context **ppContext = ( struct nk_context ** ) hb_parptrGC( &s_gc_nk_context_Funcs, iParam );

   if( ppContext && *ppContext )
      return *ppContext;

   HB_ERR_ARGS();
   return NULL;
}

struct nk_context *hb_nk_context_ParamGet( int iParam )
{
   struct nk_context **ppContext = ( struct nk_context ** ) hb_parptrGC( &s_gc_nk_context_Funcs, iParam );

   return ppContext ? *ppContext : NULL;
}

struct nk_context *hb_nk_context_ItemGet( PHB_ITEM pItem )
{
   struct nk_context **ppContext = ( struct nk_context ** ) hb_itemGetPtrGC( pItem, &s_gc_nk_context_Funcs );

   return ppContext ? *ppContext : NULL;
}

PHB_ITEM hb_nk_context_ItemPut( PHB_ITEM pItem, struct nk_context *pContext )
{
   struct nk_context **ppContext = ( struct nk_context ** ) hb_gcAllocate( sizeof( struct nk_context * ), &s_gc_nk_context_Funcs );

   *ppContext = pContext;
   return hb_itemPutPtrGC( pItem, ppContext );
}

void hb_nk_context_ItemClear( PHB_ITEM pItem )
{
   struct nk_context **ppContext = ( struct nk_context ** ) hb_itemGetPtrGC( pItem, &s_gc_nk_context_Funcs );

   if( ppContext )
      *ppContext = NULL;
}

void hb_nk_context_StorPtr( struct nk_context *pContext, int iParam )
{
   struct nk_context **ppContext = ( struct nk_context ** ) hb_gcAllocate( sizeof( struct nk_context * ), &s_gc_nk_context_Funcs );

   *ppContext = pContext;
   hb_storptrGC( ppContext, iParam );
}

void hb_nk_context_Return( struct nk_context *pContext )
{
   if( pContext )
   {
      hb_nk_context_ItemPut( hb_param( -1, HB_IT_ANY ), pContext );
   }
   else
   {
      hb_ret();
   }
}

/* -------------------------------------------------------------------------
Nuklear API
------------------------------------------------------------------------- */

// nk_bool nk_init_default(struct nk_context*, const struct nk_user_font*);
// nk_bool nk_init_fixed(struct nk_context*, void *memory, nk_size size, const struct nk_user_font*);
// nk_bool nk_init(struct nk_context*, struct nk_allocator*, const struct nk_user_font*);
// nk_bool nk_init_custom(struct nk_context*, struct nk_buffer *cmds, struct nk_buffer *pool, const struct nk_user_font*);

// void nk_clear( struct nk_context *);
HB_FUNC( NK_CLEAR )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      nk_clear( hb_nk_context_Param( 1 ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// ---
// void nk_free(struct nk_context*);
// Implementation in Harbour function HB_GARBAGE_FUNC( hb_nk_context_Destructor )
// ---
// void nk_set_user_data(struct nk_context*, nk_handle handle);

// void nk_input_begin( struct nk_context *);
HB_FUNC( NK_INPUT_BEGIN )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      nk_input_begin( hb_nk_context_Param( 1 ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// void nk_input_motion( struct nk_context*, int x, int y );
HB_FUNC( NK_INPUT_MOTION )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_NUMERIC ) != NULL && hb_param( 3, HB_IT_NUMERIC ) != NULL )
   {
      nk_input_motion( hb_nk_context_Param( 1 ), hb_parni( 2 ), hb_parni( 3 ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// void nk_input_key(struct nk_context*, enum nk_keys, nk_bool down);
// void nk_input_button(struct nk_context*, enum nk_buttons, int x, int y, nk_bool down);
// void nk_input_scroll(struct nk_context*, struct nk_vec2 val);
// void nk_input_char(struct nk_context*, char);
// void nk_input_glyph(struct nk_context*, const nk_glyph);
// void nk_input_unicode(struct nk_context*, nk_rune);

// void nk_input_end(struct nk_context*);
HB_FUNC( NK_INPUT_END )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      nk_input_end( hb_nk_context_Param( 1 ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// const struct nk_command* nk__begin(struct nk_context*);
// const struct nk_command* nk__next(struct nk_context*, const struct nk_command*);
// nk_flags nk_convert(struct nk_context*, struct nk_buffer *cmds, struct nk_buffer *vertices, struct nk_buffer *elements, const struct nk_convert_config*);
// const struct nk_draw_command* nk__draw_begin(const struct nk_context*, const struct nk_buffer*);
// const struct nk_draw_command* nk__draw_end(const struct nk_context*, const struct nk_buffer*);
// const struct nk_draw_command* nk__draw_next(const struct nk_draw_command*, const struct nk_buffer*, const struct nk_context*);

// nk_bool nk_begin( struct nk_context *ctx, const char *title, struct nk_rect bounds, nk_flags flags );
HB_FUNC( NK_BEGIN )
{
   PHB_ITEM pArray;

   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_STRING ) != NULL && ( pArray = hb_param( 3, HB_IT_ARRAY ) ) != NULL && hb_arrayLen( pArray ) == 4 && hb_param( 4, HB_IT_NUMERIC ) != NULL )
   {
      struct nk_rect bounds;
      bounds.x = hb_arrayGetNI( pArray, 1 );
      bounds.y = hb_arrayGetNI( pArray, 2 );
      bounds.w = hb_arrayGetNI( pArray, 3 );
      bounds.h = hb_arrayGetNI( pArray, 4 );

      hb_retl( nk_begin( hb_nk_context_Param( 1 ), hb_parc( 2 ), bounds, ( nk_flags ) hb_parni( 4 ) ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// nk_bool nk_begin_titled(struct nk_context *ctx, const char *name, const char *title, struct nk_rect bounds, nk_flags flags);

// void nk_end( struct nk_context *ctx );
HB_FUNC( NK_END )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      nk_end( hb_nk_context_Param( 1 ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// struct nk_window *nk_window_find(struct nk_context *ctx, const char *name);
// struct nk_rect nk_window_get_bounds(const struct nk_context *ctx);
// struct nk_vec2 nk_window_get_position(const struct nk_context *ctx);
// struct nk_vec2 nk_window_get_size(const struct nk_context*);
// float nk_window_get_width(const struct nk_context*);
// float nk_window_get_height(const struct nk_context*);
// struct nk_panel* nk_window_get_panel(struct nk_context*);
// struct nk_rect nk_window_get_content_region(struct nk_context*);
// struct nk_vec2 nk_window_get_content_region_min(struct nk_context*);
// struct nk_vec2 nk_window_get_content_region_max(struct nk_context*);
// struct nk_vec2 nk_window_get_content_region_size(struct nk_context*);
// struct nk_command_buffer* nk_window_get_canvas(struct nk_context*);
// void nk_window_get_scroll(struct nk_context*, nk_uint *offset_x, nk_uint *offset_y);
// nk_bool nk_window_has_focus(const struct nk_context*);
// nk_bool nk_window_is_hovered(struct nk_context*);
// nk_bool nk_window_is_collapsed(struct nk_context *ctx, const char *name);
// nk_bool nk_window_is_closed(struct nk_context*, const char*);

// nk_bool nk_window_is_hidden( struct nk_context*, const char * );
HB_FUNC( NK_WINDOW_IS_HIDDEN )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_STRING ) != NULL )
   {
      hb_retl( nk_window_is_hidden( hb_nk_context_Param( 1 ), hb_parc( 2 ) ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// nk_bool nk_window_is_active(struct nk_context*, const char*);
// nk_bool nk_window_is_any_hovered(struct nk_context*);
// nk_bool nk_item_is_any_active(struct nk_context*);

// void nk_window_set_bounds( struct nk_context *, const char *name, struct nk_rect bounds );
HB_FUNC( NK_WINDOW_SET_BOUNDS )
{
   PHB_ITEM pArray;

   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_STRING ) != NULL && ( pArray = hb_param( 3, HB_IT_ARRAY ) ) != NULL && hb_arrayLen( pArray ) == 4 )
   {
      struct nk_rect bounds;

      bounds.x = ( float ) hb_arrayGetND( pArray, 1 );
      bounds.y = ( float ) hb_arrayGetND( pArray, 2 );
      bounds.w = ( float ) hb_arrayGetND( pArray, 3 );
      bounds.h = ( float ) hb_arrayGetND( pArray, 4 );

      nk_window_set_bounds( hb_nk_context_Param( 1 ), hb_parc( 2 ), bounds );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// void nk_window_set_position(struct nk_context*, const char *name, struct nk_vec2 pos);
// void nk_window_set_size(struct nk_context*, const char *name, struct nk_vec2);
// void nk_window_set_focus(struct nk_context*, const char *name);
// void nk_window_set_scroll(struct nk_context*, nk_uint offset_x, nk_uint offset_y);
// void nk_window_close(struct nk_context *ctx, const char *name);
// void nk_window_collapse(struct nk_context*, const char *name, enum nk_collapse_states state);
// void nk_window_collapse_if(struct nk_context*, const char *name, enum nk_collapse_states, int cond);
// void nk_window_show(struct nk_context*, const char *name, enum nk_show_states);
// void nk_window_show_if(struct nk_context*, const char *name, enum nk_show_states, int cond);
// void nk_layout_set_min_row_height(struct nk_context*, float height);
// void nk_layout_reset_min_row_height(struct nk_context*);
// struct nk_rect nk_layout_widget_bounds(struct nk_context*);
// float nk_layout_ratio_from_pixel(struct nk_context*, float pixel_width);

// void nk_layout_row_dynamic( struct nk_context *ctx, float height, int cols );
HB_FUNC( NK_LAYOUT_ROW_DYNAMIC )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_NUMERIC ) != NULL && hb_param( 3, HB_IT_NUMERIC ) != NULL )
   {
      nk_layout_row_dynamic( hb_nk_context_Param( 1 ), ( float ) hb_parnd( 2 ), hb_parni( 3 ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// void nk_layout_row_static( struct nk_context *ctx, float height, int item_width, int cols );
HB_FUNC( NK_LAYOUT_ROW_STATIC )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_NUMERIC ) != NULL && hb_param( 3, HB_IT_NUMERIC ) != NULL && hb_param( 4, HB_IT_NUMERIC ) != NULL )
   {
      nk_layout_row_static( hb_nk_context_Param( 1 ), ( float ) hb_parnd( 2 ), hb_parni( 3 ), hb_parni( 4 ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// void nk_layout_row_begin(struct nk_context *ctx, enum nk_layout_format fmt, float row_height, int cols);
// void nk_layout_row_push(struct nk_context*, float value);
// void nk_layout_row_end(struct nk_context*);
// void nk_layout_row(struct nk_context*, enum nk_layout_format, float height, int cols, const float *ratio);
// void nk_layout_row_template_begin(struct nk_context*, float row_height);
// void nk_layout_row_template_push_dynamic(struct nk_context*);
// void nk_layout_row_template_push_variable(struct nk_context*, float min_width);
// void nk_layout_row_template_push_static(struct nk_context*, float width);
// void nk_layout_row_template_end(struct nk_context*);
// void nk_layout_space_begin(struct nk_context*, enum nk_layout_format, float height, int widget_count);
// void nk_layout_space_push(struct nk_context*, struct nk_rect bounds);
// void nk_layout_space_end(struct nk_context*);
// struct nk_rect nk_layout_space_bounds(struct nk_context*);
// struct nk_vec2 nk_layout_space_to_screen(struct nk_context*, struct nk_vec2);
// struct nk_vec2 nk_layout_space_to_local(struct nk_context*, struct nk_vec2);
// struct nk_rect nk_layout_space_rect_to_screen(struct nk_context*, struct nk_rect);
// struct nk_rect nk_layout_space_rect_to_local(struct nk_context*, struct nk_rect);
// void nk_spacer(struct nk_context* );
// nk_bool nk_group_begin(struct nk_context*, const char *title, nk_flags);
// nk_bool nk_group_begin_titled(struct nk_context*, const char *name, const char *title, nk_flags);
// void nk_group_end(struct nk_context*);
// nk_bool nk_group_scrolled_offset_begin(struct nk_context*, nk_uint *x_offset, nk_uint *y_offset, const char *title, nk_flags flags);
// nk_bool nk_group_scrolled_begin(struct nk_context*, struct nk_scroll *off, const char *title, nk_flags);
// void nk_group_scrolled_end(struct nk_context*);
// void nk_group_get_scroll(struct nk_context*, const char *id, nk_uint *x_offset, nk_uint *y_offset);
// void nk_group_set_scroll(struct nk_context*, const char *id, nk_uint x_offset, nk_uint y_offset);
// nk_bool nk_tree_push_hashed(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states initial_state, const char *hash, int len,int seed);
// nk_bool nk_tree_image_push_hashed(struct nk_context*, enum nk_tree_type, struct nk_image, const char *title, enum nk_collapse_states initial_state, const char *hash, int len,int seed);
// void nk_tree_pop(struct nk_context*);
// nk_bool nk_tree_state_push(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states *state);
// nk_bool nk_tree_state_image_push(struct nk_context*, enum nk_tree_type, struct nk_image, const char *title, enum nk_collapse_states *state);
// void nk_tree_state_pop(struct nk_context*);
// nk_bool nk_tree_element_push_hashed(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states initial_state, nk_bool *selected, const char *hash, int len, int seed);
// nk_bool nk_tree_element_image_push_hashed(struct nk_context*, enum nk_tree_type, struct nk_image, const char *title, enum nk_collapse_states initial_state, nk_bool *selected, const char *hash, int len,int seed);
// void nk_tree_element_pop(struct nk_context*);
// nk_bool nk_list_view_begin(struct nk_context*, struct nk_list_view *out, const char *id, nk_flags, int row_height, int row_count);
// void nk_list_view_end(struct nk_list_view*);
// enum nk_widget_layout_states nk_widget(struct nk_rect*, const struct nk_context*);
// enum nk_widget_layout_states nk_widget_fitting(struct nk_rect*, struct nk_context*, struct nk_vec2);

// struct nk_rect nk_widget_bounds( struct nk_context * );
HB_FUNC( NK_WIDGET_BOUNDS )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      struct nk_rect rect = nk_widget_bounds( hb_nk_context_Param( 1 ) );

      PHB_ITEM pArray = hb_itemArrayNew( 4 );

      hb_arraySetND( pArray, 1, ( float ) rect.x );
      hb_arraySetND( pArray, 2, ( float ) rect.y );
      hb_arraySetND( pArray, 3, ( float ) rect.w );
      hb_arraySetND( pArray, 4, ( float ) rect.h );

      hb_itemReturnRelease( pArray );

   }
   else
   {
      HB_ERR_ARGS();
   }
}

// struct nk_vec2 nk_widget_position(struct nk_context*);
// struct nk_vec2 nk_widget_size(struct nk_context*);

// float nk_widget_width( struct nk_context* );
HB_FUNC( NK_WIDGET_WIDTH )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      hb_retnd( ( float ) nk_widget_width( hb_nk_context_Param( 1 ) ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// float nk_widget_height(struct nk_context*);
// nk_bool nk_widget_is_hovered(struct nk_context*);
// nk_bool nk_widget_is_mouse_clicked(struct nk_context*, enum nk_buttons);
// nk_bool nk_widget_has_mouse_click_down(struct nk_context*, enum nk_buttons, nk_bool down);
// void nk_spacing(struct nk_context*, int cols);
// void nk_text(struct nk_context*, const char*, int, nk_flags);
// void nk_text_colored(struct nk_context*, const char*, int, nk_flags, struct nk_color);
// void nk_text_wrap(struct nk_context*, const char*, int);
// void nk_text_wrap_colored(struct nk_context*, const char*, int, struct nk_color);

// void nk_label( struct nk_context*, const char*, nk_flags align );
HB_FUNC( NK_LABEL )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_STRING ) != NULL && hb_param( 3, HB_IT_NUMERIC ) != NULL )
   {
      nk_label( hb_nk_context_Param( 1 ), hb_parc( 2 ), hb_parni( 3 ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// void nk_label_colored(struct nk_context*, const char*, nk_flags align, struct nk_color);
// void nk_label_wrap(struct nk_context*, const char*);
// void nk_label_colored_wrap(struct nk_context*, const char*, struct nk_color);
// void nk_image(struct nk_context*, struct nk_image);
// void nk_image_color(struct nk_context*, struct nk_image, struct nk_color);
// void nk_labelf(struct nk_context*, nk_flags, NK_PRINTF_FORMAT_STRING const char*, ...) NK_PRINTF_VARARG_FUNC(3);
// void nk_labelf_colored(struct nk_context*, nk_flags, struct nk_color, NK_PRINTF_FORMAT_STRING const char*,...) NK_PRINTF_VARARG_FUNC(4);
// void nk_labelf_wrap(struct nk_context*, NK_PRINTF_FORMAT_STRING const char*,...) NK_PRINTF_VARARG_FUNC(2);
// void nk_labelf_colored_wrap(struct nk_context*, struct nk_color, NK_PRINTF_FORMAT_STRING const char*,...) NK_PRINTF_VARARG_FUNC(3);
// void nk_labelfv(struct nk_context*, nk_flags, NK_PRINTF_FORMAT_STRING const char*, va_list) NK_PRINTF_VALIST_FUNC(3);
// void nk_labelfv_colored(struct nk_context*, nk_flags, struct nk_color, NK_PRINTF_FORMAT_STRING const char*, va_list) NK_PRINTF_VALIST_FUNC(4);
// void nk_labelfv_wrap(struct nk_context*, NK_PRINTF_FORMAT_STRING const char*, va_list) NK_PRINTF_VALIST_FUNC(2);
// void nk_labelfv_colored_wrap(struct nk_context*, struct nk_color, NK_PRINTF_FORMAT_STRING const char*, va_list) NK_PRINTF_VALIST_FUNC(3);
// void nk_value_bool(struct nk_context*, const char *prefix, int);
// void nk_value_int(struct nk_context*, const char *prefix, int);
// void nk_value_uint(struct nk_context*, const char *prefix, unsigned int);
// void nk_value_float(struct nk_context*, const char *prefix, float);
// void nk_value_color_byte(struct nk_context*, const char *prefix, struct nk_color);
// void nk_value_color_float(struct nk_context*, const char *prefix, struct nk_color);
// void nk_value_color_hex(struct nk_context*, const char *prefix, struct nk_color);

/* -------------------------------------------------------------------------
BUTTON
------------------------------------------------------------------------- */
// nk_bool nk_button_text(struct nk_context*, const char *title, int len);

// nk_bool nk_button_label(struct nk_context*, const char *title);
HB_FUNC( NK_BUTTON_LABEL )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_STRING ) != NULL )
   {
      hb_retl( nk_button_label( hb_nk_context_Param( 1 ), hb_parc( 2 ) ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// nk_bool nk_button_color(struct nk_context*, struct nk_color);
// nk_bool nk_button_symbol(struct nk_context*, enum nk_symbol_type);
// nk_bool nk_button_image(struct nk_context*, struct nk_image img);
// nk_bool nk_button_symbol_label(struct nk_context*, enum nk_symbol_type, const char*, nk_flags text_alignment);
// nk_bool nk_button_symbol_text(struct nk_context*, enum nk_symbol_type, const char*, int, nk_flags alignment);
// nk_bool nk_button_image_label(struct nk_context*, struct nk_image img, const char*, nk_flags text_alignment);
// nk_bool nk_button_image_text(struct nk_context*, struct nk_image img, const char*, int, nk_flags alignment);
// nk_bool nk_button_text_styled(struct nk_context*, const struct nk_style_button*, const char *title, int len);
// nk_bool nk_button_label_styled(struct nk_context*, const struct nk_style_button*, const char *title);
// nk_bool nk_button_symbol_styled(struct nk_context*, const struct nk_style_button*, enum nk_symbol_type);
// nk_bool nk_button_image_styled(struct nk_context*, const struct nk_style_button*, struct nk_image img);
// nk_bool nk_button_symbol_text_styled(struct nk_context*,const struct nk_style_button*, enum nk_symbol_type, const char*, int, nk_flags alignment);
// nk_bool nk_button_symbol_label_styled(struct nk_context *ctx, const struct nk_style_button *style, enum nk_symbol_type symbol, const char *title, nk_flags align);
// nk_bool nk_button_image_label_styled(struct nk_context*,const struct nk_style_button*, struct nk_image img, const char*, nk_flags text_alignment);
// nk_bool nk_button_image_text_styled(struct nk_context*,const struct nk_style_button*, struct nk_image img, const char*, int, nk_flags alignment);
// void nk_button_set_behavior(struct nk_context*, enum nk_button_behavior);
// nk_bool nk_button_push_behavior(struct nk_context*, enum nk_button_behavior);
// nk_bool nk_button_pop_behavior(struct nk_context*);

// nk_bool nk_check_label(struct nk_context*, const char*, nk_bool active);
// nk_bool nk_check_text(struct nk_context*, const char*, int, nk_bool active);
// unsigned nk_check_flags_label(struct nk_context*, const char*, unsigned int flags, unsigned int value);
// unsigned nk_check_flags_text(struct nk_context*, const char*, int, unsigned int flags, unsigned int value);
// nk_bool nk_checkbox_label(struct nk_context*, const char*, nk_bool *active);
// nk_bool nk_checkbox_text(struct nk_context*, const char*, int, nk_bool *active);
// nk_bool nk_checkbox_flags_label(struct nk_context*, const char*, unsigned int *flags, unsigned int value);
// nk_bool nk_checkbox_flags_text(struct nk_context*, const char*, int, unsigned int *flags, unsigned int value);
// nk_bool nk_radio_label(struct nk_context*, const char*, nk_bool *active);
// nk_bool nk_radio_text(struct nk_context*, const char*, int, nk_bool *active);

// nk_bool nk_option_label(struct nk_context*, const char*, nk_bool active);
HB_FUNC( NK_OPTION_LABEL )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_STRING ) != NULL && hb_param( 3, HB_IT_LOGICAL ) != NULL )
   {
      hb_retl( nk_option_label( hb_nk_context_Param( 1 ), hb_parc( 2 ), hb_parl( 3 ) ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// nk_bool nk_option_text( struct nk_context *, const char *, int, nk_bool active );
HB_FUNC( NK_OPTION_TEXT )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_STRING ) != NULL && hb_param( 3, HB_IT_LOGICAL ) != NULL )
   {
      hb_retl( nk_option_text( hb_nk_context_Param( 1 ), hb_parc( 2 ), hb_parni( 3 ), hb_parl( 4 ) ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// nk_bool nk_selectable_label(struct nk_context*, const char*, nk_flags align, nk_bool *value);
// nk_bool nk_selectable_text(struct nk_context*, const char*, int, nk_flags align, nk_bool *value);
// nk_bool nk_selectable_image_label(struct nk_context*,struct nk_image,  const char*, nk_flags align, nk_bool *value);
// nk_bool nk_selectable_image_text(struct nk_context*,struct nk_image, const char*, int, nk_flags align, nk_bool *value);
// nk_bool nk_selectable_symbol_label(struct nk_context*,enum nk_symbol_type,  const char*, nk_flags align, nk_bool *value);
// nk_bool nk_selectable_symbol_text(struct nk_context*,enum nk_symbol_type, const char*, int, nk_flags align, nk_bool *value);
// nk_bool nk_select_label(struct nk_context*, const char*, nk_flags align, nk_bool value);
// nk_bool nk_select_text(struct nk_context*, const char*, int, nk_flags align, nk_bool value);
// nk_bool nk_select_image_label(struct nk_context*, struct nk_image,const char*, nk_flags align, nk_bool value);
// nk_bool nk_select_image_text(struct nk_context*, struct nk_image,const char*, int, nk_flags align, nk_bool value);
// nk_bool nk_select_symbol_label(struct nk_context*,enum nk_symbol_type,  const char*, nk_flags align, nk_bool value);
// nk_bool nk_select_symbol_text(struct nk_context*,enum nk_symbol_type, const char*, int, nk_flags align, nk_bool value);
// float nk_slide_float(struct nk_context*, float min, float val, float max, float step);
// int nk_slide_int(struct nk_context*, int min, int val, int max, int step);
// nk_bool nk_slider_float(struct nk_context*, float min, float *val, float max, float step);
// nk_bool nk_slider_int(struct nk_context*, int min, int *val, int max, int step);
// nk_bool nk_progress(struct nk_context*, nk_size *cur, nk_size max, nk_bool modifyable);
// nk_size nk_prog(struct nk_context*, nk_size cur, nk_size max, nk_bool modifyable);

// struct nk_colorf nk_color_picker(struct nk_context*, struct nk_colorf, enum nk_color_format);
HB_FUNC( NK_COLOR_PICKER )
{
   PHB_ITEM pArrayPar;

   if( hb_param( 1, HB_IT_POINTER ) != NULL && ( pArrayPar = hb_param( 2, HB_IT_ARRAY ) ) != NULL && hb_param( 3, HB_IT_NUMERIC ) != NULL )
   {
      struct nk_colorf colorPar;
      colorPar.r = ( float ) hb_arrayGetND( pArrayPar, 1 );
      colorPar.g = ( float ) hb_arrayGetND( pArrayPar, 2 );
      colorPar.b = ( float ) hb_arrayGetND( pArrayPar, 3 );
      colorPar.a = ( float ) hb_arrayGetND( pArrayPar, 4 );

      struct nk_colorf colorRet = nk_color_picker( hb_nk_context_Param( 1 ), colorPar, hb_parni( 3 ) );

      PHB_ITEM pArrayRet = hb_itemArrayNew( 4 );

      hb_arraySetND( pArrayRet, 1, ( float ) colorRet.r );
      hb_arraySetND( pArrayRet, 2, ( float ) colorRet.g );
      hb_arraySetND( pArrayRet, 3, ( float ) colorRet.b );
      hb_arraySetND( pArrayRet, 4, ( float ) colorRet.a );

      hb_itemReturnRelease( pArrayRet );
   }
   else
   {
      HB_ERR_ARGS();
   }
}
// nk_bool nk_color_pick(struct nk_context*, struct nk_colorf*, enum nk_color_format);

// void nk_property_int(struct nk_context*, const char *name, int min, int *val, int max, int step, float inc_per_pixel);
HB_FUNC( NK_PROPERTY_INT )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_STRING ) != NULL && hb_param( 3, HB_IT_NUMERIC ) != NULL && hb_param( 4, HB_IT_BYREF ) != NULL &&
              hb_param( 5, HB_IT_NUMERIC ) != NULL && hb_param( 6, HB_IT_NUMERIC ) != NULL && hb_param( 7, HB_IT_NUMERIC ) != NULL )
   {
      int val = 0;
      nk_property_int( hb_nk_context_Param( 1 ), hb_parc( 2 ), hb_parni( 3 ), &val, hb_parni( 5 ), hb_parni( 6 ), ( float ) hb_parnd( 7 ) );
      hb_storni( val, 4 );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// void nk_property_float(struct nk_context*, const char *name, float min, float *val, float max, float step, float inc_per_pixel);
// void nk_property_double(struct nk_context*, const char *name, double min, double *val, double max, double step, float inc_per_pixel);
// int nk_propertyi(struct nk_context*, const char *name, int min, int val, int max, int step, float inc_per_pixel);

// float nk_propertyf(struct nk_context*, const char *name, float min, float val, float max, float step, float inc_per_pixel);
HB_FUNC( NK_PROPERTYF )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_STRING ) != NULL && hb_param( 3, HB_IT_NUMERIC ) != NULL && hb_param( 4, HB_IT_NUMERIC ) != NULL &&
      hb_param( 5, HB_IT_NUMERIC ) != NULL && hb_param( 6, HB_IT_NUMERIC ) != NULL && hb_param( 7, HB_IT_NUMERIC ) != NULL )
   {
      hb_retnd( ( float ) nk_propertyf( hb_nk_context_Param( 1 ), hb_parc( 2 ), ( float ) hb_parnd( 3 ), ( float ) hb_parnd( 4 ), ( float ) hb_parnd( 5 ), ( float ) hb_parnd( 6 ), ( float ) hb_parnd( 7 ) ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// double nk_propertyd(struct nk_context*, const char *name, double min, double val, double max, double step, float inc_per_pixel);
// nk_flags nk_edit_string(struct nk_context*, nk_flags, char *buffer, int *len, int max, nk_plugin_filter);
// nk_flags nk_edit_string_zero_terminated(struct nk_context*, nk_flags, char *buffer, int max, nk_plugin_filter);
// nk_flags nk_edit_buffer(struct nk_context*, nk_flags, struct nk_text_edit*, nk_plugin_filter);
// void nk_edit_focus(struct nk_context*, nk_flags flags);
// void nk_edit_unfocus(struct nk_context*);
// nk_bool nk_chart_begin(struct nk_context*, enum nk_chart_type, int num, float min, float max);
// nk_bool nk_chart_begin_colored(struct nk_context*, enum nk_chart_type, struct nk_color, struct nk_color active, int num, float min, float max);
// void nk_chart_add_slot(struct nk_context *ctx, const enum nk_chart_type, int count, float min_value, float max_value);
// void nk_chart_add_slot_colored(struct nk_context *ctx, const enum nk_chart_type, struct nk_color, struct nk_color active, int count, float min_value, float max_value);
// nk_flags nk_chart_push(struct nk_context*, float);
// nk_flags nk_chart_push_slot(struct nk_context*, float, int);
// void nk_chart_end(struct nk_context*);
// void nk_plot(struct nk_context*, enum nk_chart_type, const float *values, int count, int offset);
// void nk_plot_function(struct nk_context*, enum nk_chart_type, void *userdata, float(*value_getter)(void* user, int index), int count, int offset);
// nk_bool nk_popup_begin(struct nk_context*, enum nk_popup_type, const char*, nk_flags, struct nk_rect bounds);
// void nk_popup_close(struct nk_context*);
// void nk_popup_end(struct nk_context*);
// void nk_popup_get_scroll(struct nk_context*, nk_uint *offset_x, nk_uint *offset_y);
// void nk_popup_set_scroll(struct nk_context*, nk_uint offset_x, nk_uint offset_y);
// int nk_combo(struct nk_context*, const char **items, int count, int selected, int item_height, struct nk_vec2 size);
// int nk_combo_separator(struct nk_context*, const char *items_separated_by_separator, int separator, int selected, int count, int item_height, struct nk_vec2 size);
// int nk_combo_string(struct nk_context*, const char *items_separated_by_zeros, int selected, int count, int item_height, struct nk_vec2 size);
// int nk_combo_callback(struct nk_context*, void(*item_getter)(void*, int, const char**), void *userdata, int selected, int count, int item_height, struct nk_vec2 size);
// void nk_combobox(struct nk_context*, const char **items, int count, int *selected, int item_height, struct nk_vec2 size);
// void nk_combobox_string(struct nk_context*, const char *items_separated_by_zeros, int *selected, int count, int item_height, struct nk_vec2 size);
// void nk_combobox_separator(struct nk_context*, const char *items_separated_by_separator, int separator, int *selected, int count, int item_height, struct nk_vec2 size);
// void nk_combobox_callback(struct nk_context*, void(*item_getter)(void*, int, const char**), void*, int *selected, int count, int item_height, struct nk_vec2 size);
// nk_bool nk_combo_begin_text(struct nk_context*, const char *selected, int, struct nk_vec2 size);
// nk_bool nk_combo_begin_label(struct nk_context*, const char *selected, struct nk_vec2 size);

// nk_bool nk_combo_begin_color(struct nk_context*, struct nk_color color, struct nk_vec2 size);
HB_FUNC( NK_COMBO_BEGIN_COLOR )
{
   PHB_ITEM pArray1;
   PHB_ITEM pArray2;

   if( hb_param( 1, HB_IT_POINTER ) != NULL && ( pArray1 = hb_param( 2, HB_IT_ARRAY ) ) != NULL && ( pArray2 = hb_param( 3, HB_IT_ARRAY ) ) != NULL )
   {
      struct nk_color color;
      color.r = ( unsigned char ) hb_arrayGetNI( pArray1, 1 );
      color.g = ( unsigned char ) hb_arrayGetNI( pArray1, 2 );
      color.b = ( unsigned char ) hb_arrayGetNI( pArray1, 3 );
      color.a = ( unsigned char ) hb_arrayGetNI( pArray1, 4 );

      struct nk_vec2 size;
      size.x = ( float ) hb_arrayGetND( pArray2, 1 );
      size.y = ( float ) hb_arrayGetND( pArray2, 2 );

      hb_retl( nk_combo_begin_color( hb_nk_context_Param( 1 ), color, size ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// nk_bool nk_combo_begin_symbol(struct nk_context*,  enum nk_symbol_type,  struct nk_vec2 size);
// nk_bool nk_combo_begin_symbol_label(struct nk_context*, const char *selected, enum nk_symbol_type, struct nk_vec2 size);
// nk_bool nk_combo_begin_symbol_text(struct nk_context*, const char *selected, int, enum nk_symbol_type, struct nk_vec2 size);
// nk_bool nk_combo_begin_image(struct nk_context*, struct nk_image img,  struct nk_vec2 size);
// nk_bool nk_combo_begin_image_label(struct nk_context*, const char *selected, struct nk_image, struct nk_vec2 size);
// nk_bool nk_combo_begin_image_text(struct nk_context*,  const char *selected, int, struct nk_image, struct nk_vec2 size);
// nk_bool nk_combo_item_label(struct nk_context*, const char*, nk_flags alignment);
// nk_bool nk_combo_item_text(struct nk_context*, const char*,int, nk_flags alignment);
// nk_bool nk_combo_item_image_label(struct nk_context*, struct nk_image, const char*, nk_flags alignment);
// nk_bool nk_combo_item_image_text(struct nk_context*, struct nk_image, const char*, int,nk_flags alignment);
// nk_bool nk_combo_item_symbol_label(struct nk_context*, enum nk_symbol_type, const char*, nk_flags alignment);
// nk_bool nk_combo_item_symbol_text(struct nk_context*, enum nk_symbol_type, const char*, int, nk_flags alignment);
// void nk_combo_close(struct nk_context*);

// void nk_combo_end(struct nk_context*);
HB_FUNC( NK_COMBO_END )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      nk_combo_end( hb_nk_context_Param( 1 ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// nk_bool nk_contextual_begin(struct nk_context*, nk_flags, struct nk_vec2, struct nk_rect trigger_bounds);
// nk_bool nk_contextual_item_text(struct nk_context*, const char*, int,nk_flags align);
// nk_bool nk_contextual_item_label(struct nk_context*, const char*, nk_flags align);
// nk_bool nk_contextual_item_image_label(struct nk_context*, struct nk_image, const char*, nk_flags alignment);
// nk_bool nk_contextual_item_image_text(struct nk_context*, struct nk_image, const char*, int len, nk_flags alignment);
// nk_bool nk_contextual_item_symbol_label(struct nk_context*, enum nk_symbol_type, const char*, nk_flags alignment);
// nk_bool nk_contextual_item_symbol_text(struct nk_context*, enum nk_symbol_type, const char*, int, nk_flags alignment);
// void nk_contextual_close(struct nk_context*);
// void nk_contextual_end(struct nk_context*);
// void nk_tooltip(struct nk_context*, const char*);
// void nk_tooltipf(struct nk_context*, NK_PRINTF_FORMAT_STRING const char*, ...) NK_PRINTF_VARARG_FUNC(2);
// void nk_tooltipfv(struct nk_context*, NK_PRINTF_FORMAT_STRING const char*, va_list) NK_PRINTF_VALIST_FUNC(2);
// nk_bool nk_tooltip_begin(struct nk_context*, float width);
// void nk_tooltip_end(struct nk_context*);

// void nk_menubar_begin( struct nk_context * );
HB_FUNC( NK_MENUBAR_BEGIN )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      nk_menubar_begin( hb_nk_context_Param( 1 ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// void nk_menubar_end( struct nk_context * );
HB_FUNC( NK_MENUBAR_END )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      nk_menubar_end( hb_nk_context_Param( 1 ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// nk_bool nk_menu_begin_text(struct nk_context*, const char* title, int title_len, nk_flags align, struct nk_vec2 size);

// nk_bool nk_menu_begin_label( struct nk_context *, const char *, nk_flags align, struct nk_vec2 size );
HB_FUNC( NK_MENU_BEGIN_LABEL )
{
   PHB_ITEM pArray;

   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_STRING ) != NULL && hb_param( 3, HB_IT_NUMERIC ) != NULL &&
      ( pArray = hb_param( 4, HB_IT_ARRAY ) ) != NULL && hb_arrayLen( pArray ) == 2 )
   {
      struct nk_vec2 size;
      size.x = ( float ) hb_arrayGetND( pArray, 1 );
      size.y = ( float ) hb_arrayGetND( pArray, 2 );

      hb_retl( nk_menu_begin_label( hb_nk_context_Param( 1 ), hb_parc( 2 ), hb_parni( 3 ), size ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// nk_bool nk_menu_begin_image(struct nk_context*, const char*, struct nk_image, struct nk_vec2 size);
// nk_bool nk_menu_begin_image_text(struct nk_context*, const char*, int,nk_flags align,struct nk_image, struct nk_vec2 size);
// nk_bool nk_menu_begin_image_label(struct nk_context*, const char*, nk_flags align,struct nk_image, struct nk_vec2 size);
// nk_bool nk_menu_begin_symbol(struct nk_context*, const char*, enum nk_symbol_type, struct nk_vec2 size);
// nk_bool nk_menu_begin_symbol_text(struct nk_context*, const char*, int,nk_flags align,enum nk_symbol_type, struct nk_vec2 size);
// nk_bool nk_menu_begin_symbol_label(struct nk_context*, const char*, nk_flags align,enum nk_symbol_type, struct nk_vec2 size);
// nk_bool nk_menu_item_text(struct nk_context*, const char*, int,nk_flags align);

// nk_bool nk_menu_item_label( struct nk_context *, const char *, nk_flags alignment );
HB_FUNC( NK_MENU_ITEM_LABEL )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL && hb_param( 2, HB_IT_STRING ) != NULL && hb_param( 3, HB_IT_NUMERIC ) != NULL )
   {
      hb_retl( nk_menu_item_label( hb_nk_context_Param( 1 ), hb_parc( 2 ), hb_parni( 3 ) ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// nk_bool nk_menu_item_image_label(struct nk_context*, struct nk_image, const char*, nk_flags alignment);
// nk_bool nk_menu_item_image_text(struct nk_context*, struct nk_image, const char*, int len, nk_flags alignment);
// nk_bool nk_menu_item_symbol_text(struct nk_context*, enum nk_symbol_type, const char*, int, nk_flags alignment);
// nk_bool nk_menu_item_symbol_label(struct nk_context*, enum nk_symbol_type, const char*, nk_flags alignment);
// void nk_menu_close(struct nk_context*);

// void nk_menu_end( struct nk_context * );
HB_FUNC( NK_MENU_END )
{
   if( hb_param( 1, HB_IT_POINTER ) != NULL )
   {
      nk_menu_end( hb_nk_context_Param( 1 ) );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// void nk_style_default(struct nk_context*);
// void nk_style_from_table(struct nk_context*, const struct nk_color*);
// void nk_style_load_cursor(struct nk_context*, enum nk_style_cursor, const struct nk_cursor*);
// void nk_style_load_all_cursors(struct nk_context*, struct nk_cursor*);
// const char* nk_style_get_color_by_name(enum nk_style_colors);
// ---
// void nk_style_set_font( struct nk_context*, const struct nk_user_font * );
// Implementation in Harbour function hbnk_LoadFonts
// ---
// nk_bool nk_style_set_cursor(struct nk_context*, enum nk_style_cursor);
// void nk_style_show_cursor(struct nk_context*);
// void nk_style_hide_cursor(struct nk_context*);
// nk_bool nk_style_push_font(struct nk_context*, const struct nk_user_font*);
// nk_bool nk_style_push_float(struct nk_context*, float*, float);
// nk_bool nk_style_push_vec2(struct nk_context*, struct nk_vec2*, struct nk_vec2);
// nk_bool nk_style_push_style_item(struct nk_context*, struct nk_style_item*, struct nk_style_item);
// nk_bool nk_style_push_flags(struct nk_context*, nk_flags*, nk_flags);
// nk_bool nk_style_push_color(struct nk_context*, struct nk_color*, struct nk_color);
// nk_bool nk_style_pop_font(struct nk_context*);
// nk_bool nk_style_pop_float(struct nk_context*);
// nk_bool nk_style_pop_vec2(struct nk_context*);
// nk_bool nk_style_pop_style_item(struct nk_context*);
// nk_bool nk_style_pop_flags(struct nk_context*);
// nk_bool nk_style_pop_color(struct nk_context*);

// struct nk_color nk_rgb(int r, int g, int b);
HB_FUNC( NK_RGB )
{
   if( hb_param( 1, HB_IT_NUMERIC ) != NULL &&
       hb_param( 2, HB_IT_NUMERIC ) != NULL &&
       hb_param( 3, HB_IT_NUMERIC ) != NULL )
   {
      struct nk_color color = nk_rgb( hb_parni( 1 ), hb_parni( 2 ), hb_parni( 3 ) );

      PHB_ITEM pArray = hb_itemArrayNew( 3 );

      hb_arraySetNI( pArray, 1, ( nk_byte ) color.r );
      hb_arraySetNI( pArray, 2, ( nk_byte ) color.g );
      hb_arraySetNI( pArray, 3, ( nk_byte ) color.b );

      hb_itemReturnRelease( pArray );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// struct nk_color nk_rgb_iv(const int *rgb);
// struct nk_color nk_rgb_bv(const nk_byte* rgb);
// struct nk_color nk_rgb_f(float r, float g, float b);
// struct nk_color nk_rgb_fv(const float *rgb);

// struct nk_color nk_rgb_cf(struct nk_colorf c);
HB_FUNC( NK_RGB_CF )
{
   PHB_ITEM pArrayPar;

   if( ( pArrayPar = hb_param( 1, HB_IT_ARRAY ) ) != NULL )
   {
      struct nk_colorf colorPar;
      colorPar.r = ( float ) hb_arrayGetND( pArrayPar, 1 );
      colorPar.g = ( float ) hb_arrayGetND( pArrayPar, 2 );
      colorPar.b = ( float ) hb_arrayGetND( pArrayPar, 3 );
      colorPar.a = ( float ) hb_arrayGetND( pArrayPar, 4 );

      struct nk_color colorRet = nk_rgb_cf( colorPar );

      PHB_ITEM pArrayRet = hb_itemArrayNew( 3 );

      hb_arraySetNI( pArrayRet, 1, ( unsigned char ) colorRet.r );
      hb_arraySetNI( pArrayRet, 2, ( unsigned char ) colorRet.g );
      hb_arraySetNI( pArrayRet, 3, ( unsigned char ) colorRet.b );

      hb_itemReturnRelease( pArrayRet );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// struct nk_color nk_rgb_hex(const char *rgb);
// struct nk_color nk_rgba(int r, int g, int b, int a);
// struct nk_color nk_rgba_u32(nk_uint);
// struct nk_color nk_rgba_iv(const int *rgba);
// struct nk_color nk_rgba_bv(const nk_byte *rgba);
// struct nk_color nk_rgba_f(float r, float g, float b, float a);
// struct nk_color nk_rgba_fv(const float *rgba);
// struct nk_color nk_rgba_cf(struct nk_colorf c);
// struct nk_color nk_rgba_hex(const char *rgb);
// struct nk_colorf nk_hsva_colorf(float h, float s, float v, float a);
// struct nk_colorf nk_hsva_colorfv(float *c);
// void nk_colorf_hsva_f(float *out_h, float *out_s, float *out_v, float *out_a, struct nk_colorf in);
// void nk_colorf_hsva_fv(float *hsva, struct nk_colorf in);
// struct nk_color nk_hsv(int h, int s, int v);
// struct nk_color nk_hsv_iv(const int *hsv);
// struct nk_color nk_hsv_bv(const nk_byte *hsv);
// struct nk_color nk_hsv_f(float h, float s, float v);
// struct nk_color nk_hsv_fv(const float *hsv);
// struct nk_color nk_hsva(int h, int s, int v, int a);
// struct nk_color nk_hsva_iv(const int *hsva);
// struct nk_color nk_hsva_bv(const nk_byte *hsva);
// struct nk_color nk_hsva_f(float h, float s, float v, float a);
// struct nk_color nk_hsva_fv(const float *hsva);
// void nk_color_f(float *r, float *g, float *b, float *a, struct nk_color);
// void nk_color_fv(float *rgba_out, struct nk_color);
// struct nk_colorf nk_color_cf(struct nk_color);
// void nk_color_d(double *r, double *g, double *b, double *a, struct nk_color);
// void nk_color_dv(double *rgba_out, struct nk_color);
// nk_uint nk_color_u32(struct nk_color);
// void nk_color_hex_rgba(char *output, struct nk_color);
// void nk_color_hex_rgb(char *output, struct nk_color);
// void nk_color_hsv_i(int *out_h, int *out_s, int *out_v, struct nk_color);
// void nk_color_hsv_b(nk_byte *out_h, nk_byte *out_s, nk_byte *out_v, struct nk_color);
// void nk_color_hsv_iv(int *hsv_out, struct nk_color);
// void nk_color_hsv_bv(nk_byte *hsv_out, struct nk_color);
// void nk_color_hsv_f(float *out_h, float *out_s, float *out_v, struct nk_color);
// void nk_color_hsv_fv(float *hsv_out, struct nk_color);
// void nk_color_hsva_i(int *h, int *s, int *v, int *a, struct nk_color);
// void nk_color_hsva_b(nk_byte *h, nk_byte *s, nk_byte *v, nk_byte *a, struct nk_color);
// void nk_color_hsva_iv(int *hsva_out, struct nk_color);
// void nk_color_hsva_bv(nk_byte *hsva_out, struct nk_color);
// void nk_color_hsva_f(float *out_h, float *out_s, float *out_v, float *out_a, struct nk_color);
// void nk_color_hsva_fv(float *hsva_out, struct nk_color);
// nk_handle nk_handle_ptr(void*);
// nk_handle nk_handle_id(int);
// struct nk_image nk_image_handle(nk_handle);
// struct nk_image nk_image_ptr(void*);
// struct nk_image nk_image_id(int);
// nk_bool nk_image_is_subimage(const struct nk_image* img);
// struct nk_image nk_subimage_ptr(void*, nk_ushort w, nk_ushort h, struct nk_rect sub_region);
// struct nk_image nk_subimage_id(int, nk_ushort w, nk_ushort h, struct nk_rect sub_region);
// struct nk_image nk_subimage_handle(nk_handle, nk_ushort w, nk_ushort h, struct nk_rect sub_region);
// struct nk_nine_slice nk_nine_slice_handle(nk_handle, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b);
// struct nk_nine_slice nk_nine_slice_ptr(void*, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b);
// struct nk_nine_slice nk_nine_slice_id(int, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b);
// int nk_nine_slice_is_sub9slice(const struct nk_nine_slice* img);
// struct nk_nine_slice nk_sub9slice_ptr(void*, nk_ushort w, nk_ushort h, struct nk_rect sub_region, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b);
// struct nk_nine_slice nk_sub9slice_id(int, nk_ushort w, nk_ushort h, struct nk_rect sub_region, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b);
// struct nk_nine_slice nk_sub9slice_handle(nk_handle, nk_ushort w, nk_ushort h, struct nk_rect sub_region, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b);
// nk_hash nk_murmur_hash(const void *key, int len, nk_hash seed);
// void nk_triangle_from_direction(struct nk_vec2 *result, struct nk_rect r, float pad_x, float pad_y, enum nk_heading);

// struct nk_vec2 nk_vec2( float x, float y );
HB_FUNC( NK_VEC2 )
{
   if( hb_param( 1, HB_IT_NUMERIC ) != NULL && hb_param( 2, HB_IT_NUMERIC ) != NULL )
   {
      struct nk_vec2 size = nk_vec2( ( float ) hb_parnd( 1 ), ( float ) hb_parnd( 2 ) );

      PHB_ITEM pArray = hb_itemArrayNew( 2 );

      hb_arraySetND( pArray, 1, ( float ) size.x );
      hb_arraySetND( pArray, 2, ( float ) size.y );

      hb_itemReturnRelease( pArray );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// struct nk_vec2 nk_vec2i(int x, int y);
// struct nk_vec2 nk_vec2v(const float *xy);
// struct nk_vec2 nk_vec2iv(const int *xy);
// struct nk_rect nk_get_null_rect(void);

// struct nk_rect nk_rect( float x, float y, float w, float h );
HB_FUNC( NK_RECT )
{
   if( hb_param( 1, HB_IT_NUMERIC ) != NULL &&
       hb_param( 2, HB_IT_NUMERIC ) != NULL &&
       hb_param( 3, HB_IT_NUMERIC ) != NULL &&
       hb_param( 4, HB_IT_NUMERIC ) != NULL )
   {
      struct nk_rect rect = nk_rect( ( float ) hb_parnd( 1 ), ( float ) hb_parnd( 2 ), ( float ) hb_parnd( 3 ), ( float ) hb_parnd( 4 ) );

      PHB_ITEM pArray = hb_itemArrayNew( 4 );

      hb_arraySetND( pArray, 1, ( float ) rect.x );
      hb_arraySetND( pArray, 2, ( float ) rect.y );
      hb_arraySetND( pArray, 3, ( float ) rect.w );
      hb_arraySetND( pArray, 4, ( float ) rect.h );

      hb_itemReturnRelease( pArray );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// struct nk_rect nk_recti(int x, int y, int w, int h);
// struct nk_rect nk_recta(struct nk_vec2 pos, struct nk_vec2 size);
// struct nk_rect nk_rectv(const float *xywh);
// struct nk_rect nk_rectiv(const int *xywh);
// struct nk_vec2 nk_rect_pos(struct nk_rect);
// struct nk_vec2 nk_rect_size(struct nk_rect);
// int nk_strlen(const char *str);
// int nk_stricmp(const char *s1, const char *s2);
// int nk_stricmpn(const char *s1, const char *s2, int n);
// int nk_strtoi(const char *str, const char **endptr);
// float nk_strtof(const char *str, const char **endptr);
// double nk_strtod(const char *str, const char **endptr);
// int nk_strfilter(const char *text, const char *regexp);
// int nk_strmatch_fuzzy_string(char const *str, char const *pattern, int *out_score);
// int nk_strmatch_fuzzy_text(const char *txt, int txt_len, const char *pattern, int *out_score);
// int nk_utf_decode(const char*, nk_rune*, int);
// int nk_utf_encode(nk_rune, char*, int);
// int nk_utf_len(const char*, int byte_len);
// const char* nk_utf_at(const char *buffer, int length, int index, nk_rune *unicode, int *len);
// const nk_rune *nk_font_default_glyph_ranges(void);
// const nk_rune *nk_font_chinese_glyph_ranges(void);
// const nk_rune *nk_font_cyrillic_glyph_ranges(void);
// const nk_rune *nk_font_korean_glyph_ranges(void);
// void nk_font_atlas_init_default(struct nk_font_atlas*);
// void nk_font_atlas_init(struct nk_font_atlas*, struct nk_allocator*);
// void nk_font_atlas_init_custom(struct nk_font_atlas*, struct nk_allocator *persistent, struct nk_allocator *transient);
// void nk_font_atlas_begin(struct nk_font_atlas*);
// ---
// struct nk_font_config nk_font_config(float pixel_height);
// Implementation in Harbour function hbnk_LoadFonts
// ---
// struct nk_font *nk_font_atlas_add(struct nk_font_atlas*, const struct nk_font_config*);
// ---
// struct nk_font* nk_font_atlas_add_default(struct nk_font_atlas*, float height, const struct nk_font_config*);
// Implementation in Harbour function hbnk_LoadFonts
// ---
// struct nk_font* nk_font_atlas_add_from_memory(struct nk_font_atlas *atlas, void *memory, nk_size size, float height, const struct nk_font_config *config);
// ---
// struct nk_font* nk_font_atlas_add_from_file(struct nk_font_atlas *atlas, const char *file_path, float height, const struct nk_font_config*);
// Implementation in Harbour function hbnk_LoadFonts
// ---
// struct nk_font *nk_font_atlas_add_compressed(struct nk_font_atlas*, void *memory, nk_size size, float height, const struct nk_font_config*);
// struct nk_font* nk_font_atlas_add_compressed_base85(struct nk_font_atlas*, const char *data, float height, const struct nk_font_config *config);
// const void* nk_font_atlas_bake(struct nk_font_atlas*, int *width, int *height, enum nk_font_atlas_format);
// void nk_font_atlas_end(struct nk_font_atlas*, nk_handle tex, struct nk_draw_null_texture*);
// const struct nk_font_glyph* nk_font_find_glyph(struct nk_font*, nk_rune unicode);
// void nk_font_atlas_cleanup(struct nk_font_atlas *atlas);
// void nk_font_atlas_clear(struct nk_font_atlas*);
// void nk_buffer_init_default(struct nk_buffer*);
// void nk_buffer_init(struct nk_buffer*, const struct nk_allocator*, nk_size size);
// void nk_buffer_init_fixed(struct nk_buffer*, void *memory, nk_size size);
// void nk_buffer_info(struct nk_memory_status*, struct nk_buffer*);
// void nk_buffer_push(struct nk_buffer*, enum nk_buffer_allocation_type type, const void *memory, nk_size size, nk_size align);
// void nk_buffer_mark(struct nk_buffer*, enum nk_buffer_allocation_type type);
// void nk_buffer_reset(struct nk_buffer*, enum nk_buffer_allocation_type type);
// void nk_buffer_clear(struct nk_buffer*);
// void nk_buffer_free(struct nk_buffer*);
// void *nk_buffer_memory(struct nk_buffer*);
// const void *nk_buffer_memory_const(const struct nk_buffer*);
// nk_size nk_buffer_total(struct nk_buffer*);
// void nk_str_init_default(struct nk_str*);
// void nk_str_init(struct nk_str*, const struct nk_allocator*, nk_size size);
// void nk_str_init_fixed(struct nk_str*, void *memory, nk_size size);
// void nk_str_clear(struct nk_str*);
// void nk_str_free(struct nk_str*);
// int nk_str_append_text_char(struct nk_str*, const char*, int);
// int nk_str_append_str_char(struct nk_str*, const char*);
// int nk_str_append_text_utf8(struct nk_str*, const char*, int);
// int nk_str_append_str_utf8(struct nk_str*, const char*);
// int nk_str_append_text_runes(struct nk_str*, const nk_rune*, int);
// int nk_str_append_str_runes(struct nk_str*, const nk_rune*);
// int nk_str_insert_at_char(struct nk_str*, int pos, const char*, int);
// int nk_str_insert_at_rune(struct nk_str*, int pos, const char*, int);
// int nk_str_insert_text_char(struct nk_str*, int pos, const char*, int);
// int nk_str_insert_str_char(struct nk_str*, int pos, const char*);
// int nk_str_insert_text_utf8(struct nk_str*, int pos, const char*, int);
// int nk_str_insert_str_utf8(struct nk_str*, int pos, const char*);
// int nk_str_insert_text_runes(struct nk_str*, int pos, const nk_rune*, int);
// int nk_str_insert_str_runes(struct nk_str*, int pos, const nk_rune*);
// void nk_str_remove_chars(struct nk_str*, int len);
// void nk_str_remove_runes(struct nk_str *str, int len);
// void nk_str_delete_chars(struct nk_str*, int pos, int len);
// void nk_str_delete_runes(struct nk_str*, int pos, int len);
// char *nk_str_at_char(struct nk_str*, int pos);
// char *nk_str_at_rune(struct nk_str*, int pos, nk_rune *unicode, int *len);
// nk_rune nk_str_rune_at(const struct nk_str*, int pos);
// const char *nk_str_at_char_const(const struct nk_str*, int pos);
// const char *nk_str_at_const(const struct nk_str*, int pos, nk_rune *unicode, int *len);
// char *nk_str_get(struct nk_str*);
// const char *nk_str_get_const(const struct nk_str*);
// int nk_str_len(struct nk_str*);
// int nk_str_len_char(struct nk_str*);
// nk_bool nk_filter_default(const struct nk_text_edit*, nk_rune unicode);
// nk_bool nk_filter_ascii(const struct nk_text_edit*, nk_rune unicode);
// nk_bool nk_filter_float(const struct nk_text_edit*, nk_rune unicode);
// nk_bool nk_filter_decimal(const struct nk_text_edit*, nk_rune unicode);
// nk_bool nk_filter_hex(const struct nk_text_edit*, nk_rune unicode);
// nk_bool nk_filter_oct(const struct nk_text_edit*, nk_rune unicode);
// nk_bool nk_filter_binary(const struct nk_text_edit*, nk_rune unicode);
// void nk_textedit_init_default(struct nk_text_edit*);
// void nk_textedit_init(struct nk_text_edit*, struct nk_allocator*, nk_size size);
// void nk_textedit_init_fixed(struct nk_text_edit*, void *memory, nk_size size);
// void nk_textedit_free(struct nk_text_edit*);
// void nk_textedit_text(struct nk_text_edit*, const char*, int total_len);
// void nk_textedit_delete(struct nk_text_edit*, int where, int len);
// void nk_textedit_delete_selection(struct nk_text_edit*);
// void nk_textedit_select_all(struct nk_text_edit*);
// nk_bool nk_textedit_cut(struct nk_text_edit*);
// nk_bool nk_textedit_paste(struct nk_text_edit*, char const*, int len);
// void nk_textedit_undo(struct nk_text_edit*);
// void nk_textedit_redo(struct nk_text_edit*);
// void nk_stroke_line(struct nk_command_buffer *b, float x0, float y0, float x1, float y1, float line_thickness, struct nk_color);
// void nk_stroke_curve(struct nk_command_buffer*, float, float, float, float, float, float, float, float, float line_thickness, struct nk_color);
// void nk_stroke_rect(struct nk_command_buffer*, struct nk_rect, float rounding, float line_thickness, struct nk_color);
// void nk_stroke_circle(struct nk_command_buffer*, struct nk_rect, float line_thickness, struct nk_color);
// void nk_stroke_arc(struct nk_command_buffer*, float cx, float cy, float radius, float a_min, float a_max, float line_thickness, struct nk_color);
// void nk_stroke_triangle(struct nk_command_buffer*, float, float, float, float, float, float, float line_thichness, struct nk_color);
// void nk_stroke_polyline(struct nk_command_buffer*, float *points, int point_count, float line_thickness, struct nk_color col);
// void nk_stroke_polygon(struct nk_command_buffer*, float*, int point_count, float line_thickness, struct nk_color);
// void nk_fill_rect(struct nk_command_buffer*, struct nk_rect, float rounding, struct nk_color);
// void nk_fill_rect_multi_color(struct nk_command_buffer*, struct nk_rect, struct nk_color left, struct nk_color top, struct nk_color right, struct nk_color bottom);
// void nk_fill_circle(struct nk_command_buffer*, struct nk_rect, struct nk_color);
// void nk_fill_arc(struct nk_command_buffer*, float cx, float cy, float radius, float a_min, float a_max, struct nk_color);
// void nk_fill_triangle(struct nk_command_buffer*, float x0, float y0, float x1, float y1, float x2, float y2, struct nk_color);
// void nk_fill_polygon(struct nk_command_buffer*, float*, int point_count, struct nk_color);
// void nk_draw_image(struct nk_command_buffer*, struct nk_rect, const struct nk_image*, struct nk_color);
// void nk_draw_nine_slice(struct nk_command_buffer*, struct nk_rect, const struct nk_nine_slice*, struct nk_color);
// void nk_draw_text(struct nk_command_buffer*, struct nk_rect, const char *text, int len, const struct nk_user_font*, struct nk_color, struct nk_color);
// void nk_push_scissor(struct nk_command_buffer*, struct nk_rect);
// void nk_push_custom(struct nk_command_buffer*, struct nk_rect, nk_command_custom_callback, nk_handle usr);
// nk_bool nk_input_has_mouse_click(const struct nk_input*, enum nk_buttons);
// nk_bool nk_input_has_mouse_click_in_rect(const struct nk_input*, enum nk_buttons, struct nk_rect);
// nk_bool nk_input_has_mouse_click_down_in_rect(const struct nk_input*, enum nk_buttons, struct nk_rect, nk_bool down);
// nk_bool nk_input_is_mouse_click_in_rect(const struct nk_input*, enum nk_buttons, struct nk_rect);
// nk_bool nk_input_is_mouse_click_down_in_rect(const struct nk_input *i, enum nk_buttons id, struct nk_rect b, nk_bool down);
// nk_bool nk_input_any_mouse_click_in_rect(const struct nk_input*, struct nk_rect);
// nk_bool nk_input_is_mouse_prev_hovering_rect(const struct nk_input*, struct nk_rect);

// nk_bool nk_input_is_mouse_hovering_rect( const struct nk_input *, struct nk_rect );


// nk_bool nk_input_mouse_clicked(const struct nk_input*, enum nk_buttons, struct nk_rect);
// nk_bool nk_input_is_mouse_down(const struct nk_input*, enum nk_buttons);
// nk_bool nk_input_is_mouse_pressed(const struct nk_input*, enum nk_buttons);
// nk_bool nk_input_is_mouse_released(const struct nk_input*, enum nk_buttons);
// nk_bool nk_input_is_key_pressed(const struct nk_input*, enum nk_keys);
// nk_bool nk_input_is_key_released(const struct nk_input*, enum nk_keys);
// nk_bool nk_input_is_key_down(const struct nk_input*, enum nk_keys);
// void nk_draw_list_init(struct nk_draw_list*);
// void nk_draw_list_setup(struct nk_draw_list*, const struct nk_convert_config*, struct nk_buffer *cmds, struct nk_buffer *vertices, struct nk_buffer *elements, enum nk_anti_aliasing line_aa,enum nk_anti_aliasing shape_aa);
// const struct nk_draw_command* nk__draw_list_begin(const struct nk_draw_list*, const struct nk_buffer*);
// const struct nk_draw_command* nk__draw_list_next(const struct nk_draw_command*, const struct nk_buffer*, const struct nk_draw_list*);
// const struct nk_draw_command* nk__draw_list_end(const struct nk_draw_list*, const struct nk_buffer*);
// void nk_draw_list_path_clear(struct nk_draw_list*);
// void nk_draw_list_path_line_to(struct nk_draw_list*, struct nk_vec2 pos);
// void nk_draw_list_path_arc_to_fast(struct nk_draw_list*, struct nk_vec2 center, float radius, int a_min, int a_max);
// void nk_draw_list_path_arc_to(struct nk_draw_list*, struct nk_vec2 center, float radius, float a_min, float a_max, unsigned int segments);
// void nk_draw_list_path_rect_to(struct nk_draw_list*, struct nk_vec2 a, struct nk_vec2 b, float rounding);
// void nk_draw_list_path_curve_to(struct nk_draw_list*, struct nk_vec2 p2, struct nk_vec2 p3, struct nk_vec2 p4, unsigned int num_segments);
// void nk_draw_list_path_fill(struct nk_draw_list*, struct nk_color);
// void nk_draw_list_path_stroke(struct nk_draw_list*, struct nk_color, enum nk_draw_list_stroke closed, float thickness);
// void nk_draw_list_stroke_line(struct nk_draw_list*, struct nk_vec2 a, struct nk_vec2 b, struct nk_color, float thickness);
// void nk_draw_list_stroke_rect(struct nk_draw_list*, struct nk_rect rect, struct nk_color, float rounding, float thickness);
// void nk_draw_list_stroke_triangle(struct nk_draw_list*, struct nk_vec2 a, struct nk_vec2 b, struct nk_vec2 c, struct nk_color, float thickness);
// void nk_draw_list_stroke_circle(struct nk_draw_list*, struct nk_vec2 center, float radius, struct nk_color, unsigned int segs, float thickness);
// void nk_draw_list_stroke_curve(struct nk_draw_list*, struct nk_vec2 p0, struct nk_vec2 cp0, struct nk_vec2 cp1, struct nk_vec2 p1, struct nk_color, unsigned int segments, float thickness);
// void nk_draw_list_stroke_poly_line(struct nk_draw_list*, const struct nk_vec2 *pnts, const unsigned int cnt, struct nk_color, enum nk_draw_list_stroke, float thickness, enum nk_anti_aliasing);
// void nk_draw_list_fill_rect(struct nk_draw_list*, struct nk_rect rect, struct nk_color, float rounding);
// void nk_draw_list_fill_rect_multi_color(struct nk_draw_list*, struct nk_rect rect, struct nk_color left, struct nk_color top, struct nk_color right, struct nk_color bottom);
// void nk_draw_list_fill_triangle(struct nk_draw_list*, struct nk_vec2 a, struct nk_vec2 b, struct nk_vec2 c, struct nk_color);
// void nk_draw_list_fill_circle(struct nk_draw_list*, struct nk_vec2 center, float radius, struct nk_color col, unsigned int segs);
// void nk_draw_list_fill_poly_convex(struct nk_draw_list*, const struct nk_vec2 *points, const unsigned int count, struct nk_color, enum nk_anti_aliasing);
// void nk_draw_list_add_image(struct nk_draw_list*, struct nk_image texture, struct nk_rect rect, struct nk_color);
// void nk_draw_list_add_text(struct nk_draw_list*, const struct nk_user_font*, struct nk_rect, const char *text, int len, float font_height, struct nk_color);
// void nk_draw_list_push_userdata(struct nk_draw_list*, nk_handle userdata);
// struct nk_style_item nk_style_item_color(struct nk_color);
// struct nk_style_item nk_style_item_image(struct nk_image img);
// struct nk_style_item nk_style_item_nine_slice(struct nk_nine_slice slice);
// struct nk_style_item nk_style_item_hide(void);
