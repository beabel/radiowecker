/**
 * Sparse 1 bpp supplement for lv_font_montserrat_14: Latin letters beyond ASCII
 * (DE/FR/ES/NL: Umlaute, Akzente, ñ, ¿¡, «», œ/Œ, …) — metrics aligned with Montserrat 14.
 * Hinweis: „ “ ” (U+201C/U+201D) sind hier nicht enthalten; im Web-Editor per Tastatur.
 */
#define LV_CONF_INCLUDE_SIMPLE
#include "lv_conf.h"
#include <lvgl.h>

extern const lv_font_t lv_font_cyrl_supp_14;
extern const lv_font_t lv_font_cyrl_supp_18;

#ifndef LV_FONT_LATIN_SUPP_14
#define LV_FONT_LATIN_SUPP_14 1
#endif

#if LV_FONT_LATIN_SUPP_14

/* 8 px breit, MSB = linke Spalte; Unicode aufsteigend sortiert (Sparse-CMap) */
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+00A1 ¡ */
    0x00, 0x00, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    /* U+00AB « */
    0x00, 0x36, 0x6c, 0xd8, 0xd8, 0x6c, 0x36, 0x00, 0x00, 0x00, 0x00,
    /* U+00BB » */
    0x00, 0xd8, 0x6c, 0x36, 0x36, 0x6c, 0xd8, 0x00, 0x00, 0x00, 0x00,
    /* U+00BF ¿ */
    0x00, 0x18, 0x00, 0x18, 0x18, 0x08, 0x04, 0x0c, 0x18, 0x10, 0x00,
    /* U+00C0 À */
    0x30, 0x18, 0x00, 0x18, 0x3c, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00,
    /* U+00C2 Â */
    0x18, 0x3c, 0x00, 0x18, 0x3c, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00,
    /* U+00C4 Ä */
    0x66, 0x66, 0x00, 0x18, 0x3c, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00,
    /* U+00C6 Æ */
    0x00, 0x1e, 0x36, 0x36, 0x36, 0x1e, 0x36, 0x36, 0x36, 0x36, 0x36, 0x00,
    /* U+00C7 Ç */
    0x3e, 0x63, 0x63, 0x60, 0x60, 0x60, 0x63, 0x3e, 0x0c, 0x0c, 0x18, 0x00,
    /* U+00C8 È */
    0x30, 0x18, 0x00, 0x7e, 0xc0, 0xc0, 0xf8, 0xc0, 0xc0, 0x7e, 0x00, 0x00,
    /* U+00C9 É */
    0x0c, 0x18, 0x00, 0x7e, 0xc0, 0xc0, 0xf8, 0xc0, 0xc0, 0x7e, 0x00, 0x00,
    /* U+00CA Ê */
    0x18, 0x3c, 0x00, 0x7e, 0xc0, 0xc0, 0xf8, 0xc0, 0xc0, 0x7e, 0x00, 0x00,
    /* U+00CB Ë */
    0x66, 0x66, 0x00, 0x7e, 0xc0, 0xc0, 0xf8, 0xc0, 0xc0, 0x7e, 0x00, 0x00,
    /* U+00CE Î */
    0x18, 0x3c, 0x00, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00,
    /* U+00CF Ï */
    0x66, 0x66, 0x00, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00,
    /* U+00D1 Ñ */
    0x76, 0xdc, 0x00, 0x66, 0x66, 0x6e, 0x76, 0x66, 0x66, 0x66, 0x66, 0x00,
    /* U+00D4 Ô */
    0x18, 0x3c, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00, 0x00,
    /* U+00D6 Ö */
    0x66, 0x66, 0x00, 0x7e, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00,
    /* U+00D9 Ù */
    0x30, 0x18, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00, 0x00,
    /* U+00DB Û */
    0x18, 0x3c, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00, 0x00,
    /* U+00DC Ü */
    0x66, 0x66, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00,
    /* U+00DF ß */
    0x3c, 0x66, 0x66, 0x6c, 0x6c, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0x00,
    /* U+00E0 à */
    0x30, 0x18, 0x00, 0x78, 0x8c, 0x0c, 0x7c, 0xcc, 0xcc, 0x7c, 0x00,
    /* U+00E1 á */
    0x0c, 0x18, 0x00, 0x78, 0x8c, 0x0c, 0x7c, 0xcc, 0xcc, 0x7c, 0x00,
    /* U+00E2 â */
    0x18, 0x3c, 0x00, 0x78, 0x8c, 0x0c, 0x7c, 0xcc, 0xcc, 0x7c, 0x00,
    /* U+00E4 ä */
    0x66, 0x66, 0x00, 0x78, 0x8c, 0x0c, 0x7c, 0xcc, 0xcc, 0x7c, 0x00,
    /* U+00E6 æ */
    0x00, 0x78, 0xcc, 0xc0, 0x7c, 0xcc, 0xcc, 0xdc, 0xcc, 0x76, 0x00,
    /* U+00E7 ç */
    0x3c, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3c, 0x08, 0x08, 0x30, 0x00,
    /* U+00E8 è */
    0x30, 0x18, 0x00, 0x7c, 0xc6, 0xfe, 0xc0, 0xc6, 0x7c, 0x00, 0x00,
    /* U+00E9 é */
    0x0c, 0x18, 0x00, 0x7c, 0xc6, 0xfe, 0xc0, 0xc6, 0x7c, 0x00, 0x00,
    /* U+00EA ê */
    0x18, 0x3c, 0x00, 0x7c, 0xc6, 0xfe, 0xc0, 0xc6, 0x7c, 0x00, 0x00,
    /* U+00EB ë */
    0x66, 0x66, 0x00, 0x7c, 0xc6, 0xfe, 0xc0, 0xc6, 0x7c, 0x00, 0x00,
    /* U+00ED í */
    0x0c, 0x18, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00,
    /* U+00EE î */
    0x18, 0x3c, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00,
    /* U+00EF ï */
    0x66, 0x66, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00,
    /* U+00F1 ñ */
    0x36, 0x6d, 0x00, 0x6c, 0x6e, 0x76, 0x66, 0x66, 0x66, 0x66, 0x00,
    /* U+00F3 ó */
    0x0c, 0x18, 0x00, 0x7e, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00,
    /* U+00F4 ô */
    0x18, 0x3c, 0x00, 0x7e, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00,
    /* U+00F6 ö */
    0x66, 0x66, 0x00, 0x7e, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00,
    /* U+00F9 ù */
    0x30, 0x18, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00,
    /* U+00FA ú */
    0x0c, 0x18, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00,
    /* U+00FB û */
    0x18, 0x3c, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00,
    /* U+00FC ü */
    0x66, 0x66, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00,
    /* U+0152 Œ */
    0x00, 0x3c, 0x66, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x7e, 0x00,
    /* U+0153 œ */
    0x00, 0x00, 0x7a, 0xce, 0xc8, 0xdc, 0xce, 0xc6, 0xc6, 0x7a, 0x00,
    /* U+2026 … */
    0x00, 0x00, 0x00, 0x00, 0x6c, 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00,
};

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 130, .box_w = 8, .box_h = 11, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 11, .adv_w = 150, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 22, .adv_w = 150, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 33, .adv_w = 130, .box_w = 8, .box_h = 11, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 44, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 56, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 68, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 80, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 92, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 104, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 116, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 128, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 140, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 152, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 176, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 188, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 200, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 212, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 224, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 236, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 248, .adv_w = 155, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 259, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 270, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 292, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 303, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 314, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 325, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 336, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 347, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 358, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 369, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 380, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 391, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 402, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 413, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 424, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 435, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 446, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 457, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 468, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 479, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 490, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 502, .adv_w = 160, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 513, .adv_w = 200, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
};

static const uint16_t unicode_list_latn[] = {
    161, 171, 187, 191, 192, 194, 196, 198, 199, 200, 201, 202, 203, 206, 207, 209, 212, 214, 217, 219, 220, 223, 224, 225, 226, 228, 230, 231, 232, 233, 234, 235, 237, 238, 239, 241, 243, 244, 246, 249, 250, 251, 252, 338, 339, 8230,
};

static const lv_font_fmt_txt_cmap_t cmaps_latn[] = {
    {.range_start = 0,
     .range_length = 65535,
     .glyph_id_start = 1,
     .unicode_list = unicode_list_latn,
     .glyph_id_ofs_list = NULL,
     .list_length = 46,
     .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY},
};

#if LVGL_VERSION_MAJOR >= 9

static const lv_font_fmt_txt_dsc_t font_dsc_latin_supp = {
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps_latn,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
    .stride = 0,
};

const lv_font_t lv_font_latin_supp_14 = {
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,
    .release_glyph = NULL,
    .line_height = 16,
    .base_line = 3,
    .subpx = LV_FONT_SUBPX_NONE,
    .kerning = LV_FONT_KERNING_NORMAL,
    .static_bitmap = 1,
    .underline_position = -1,
    .underline_thickness = 1,
    .dsc = &font_dsc_latin_supp,
    .fallback = &lv_font_cyrl_supp_14,
    .user_data = NULL,
};

/* Latin wie oben, Zeilenmetrik wie lv_font_montserrat_18; Kyrillika größer — nur fürs Datums-Label (font_clock_date). */
const lv_font_t lv_font_latin_supp_14_for_m18 = {
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,
    .release_glyph = NULL,
    .line_height = 21,
    .base_line = 4,
    .subpx = LV_FONT_SUBPX_NONE,
    .kerning = LV_FONT_KERNING_NORMAL,
    .static_bitmap = 1,
    .underline_position = -1,
    .underline_thickness = 1,
    .dsc = &font_dsc_latin_supp,
    .fallback = &lv_font_cyrl_supp_18,
    .user_data = NULL,
};

#else /* LVGL 8.x */

#if LV_VERSION_CHECK(8, 0, 0)
static lv_font_fmt_txt_glyph_cache_t cache_latin_supp;
#endif

static const lv_font_fmt_txt_dsc_t font_dsc_latin_supp = {
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps_latn,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache_latin_supp,
#endif
};

const lv_font_t lv_font_latin_supp_14 = {
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,
    .line_height = 16,
    .base_line = 3,
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
#if LV_VERSION_CHECK(8, 3, 0)
    .fallback = &lv_font_cyrl_supp_14,
#endif
    .dsc = &font_dsc_latin_supp};

#if LV_VERSION_CHECK(8, 3, 0)
const lv_font_t lv_font_latin_supp_14_for_m18 = {
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,
    .line_height = 21,
    .base_line = 4,
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .fallback = &lv_font_cyrl_supp_18,
    .dsc = &font_dsc_latin_supp};
#endif

#endif /* LVGL_VERSION_MAJOR >= 9 */

#endif /* LV_FONT_LATIN_SUPP_14 */
