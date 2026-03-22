/**
 * Deutsche Umlaute / ß — 1 bpp, Metrik an lv_font_montserrat_14 angeglichen
 * (adv_w / box / ofs_x wie a,o,A; keine ofs_y-Verschiebung nach oben).
 */
#define LV_CONF_INCLUDE_SIMPLE
#include "lv_conf.h"
#include <lvgl.h>

#ifndef LV_FONT_DE_SUPP_14
#define LV_FONT_DE_SUPP_14 1
#endif

#if LV_FONT_DE_SUPP_14

/* 8 px breit, MSB = linke Spalte; Großbuchstaben 12 Zeilen, Klein 11, ß 11 */
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+00C4 Ä */
    0x66, 0x66, 0x00, 0x18, 0x3c, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00,
    /* U+00D6 Ö */
    0x66, 0x66, 0x00, 0x7e, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00,
    /* U+00DC Ü */
    0x66, 0x66, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00,
    /* U+00DF ß */
    0x3c, 0x66, 0x66, 0x6c, 0x6c, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0x00,
    /* U+00E4 ä */
    0x66, 0x66, 0x00, 0x78, 0x8c, 0x0c, 0x7c, 0xcc, 0xcc, 0x7c, 0x00,
    /* U+00F6 ö */
    0x66, 0x66, 0x00, 0x7e, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00,
    /* U+00FC ü */
    0x66, 0x66, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00,
};

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 12, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 24, .adv_w = 170, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 36, .adv_w = 155, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 47, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 58, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 69, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
};

static const uint16_t unicode_list_de[] = {196, 214, 220, 223, 228, 246, 252};

static const lv_font_fmt_txt_cmap_t cmaps_de[] = {
    {.range_start = 0,
     .range_length = 1023,
     .glyph_id_start = 1,
     .unicode_list = unicode_list_de,
     .glyph_id_ofs_list = NULL,
     .list_length = 7,
     .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY},
};

#if LVGL_VERSION_MAJOR >= 9

static const lv_font_fmt_txt_dsc_t font_dsc_de = {
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps_de,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
    .stride = 0,
};

const lv_font_t lv_font_de_supp_14 = {
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
    .dsc = &font_dsc_de,
    .fallback = NULL,
    .user_data = NULL,
};

#else /* LVGL 8.x */

#if LV_VERSION_CHECK(8, 0, 0)
static lv_font_fmt_txt_glyph_cache_t cache_de_supp;
#endif

static const lv_font_fmt_txt_dsc_t font_dsc_de = {
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps_de,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache_de_supp,
#endif
};

const lv_font_t lv_font_de_supp_14 = {
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
    .dsc = &font_dsc_de};

#endif /* LVGL_VERSION_MAJOR >= 9 */

#endif /* LV_FONT_DE_SUPP_14 */
