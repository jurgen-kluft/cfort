#ifndef __CFORT_LIB_H__
#define __CFORT_LIB_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "ccore/c_debug.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

namespace ncore
{
    namespace nfort
    {

#define FT_SUCCESS 0
#define FT_MEMORY_ERROR -1
#define FT_EINVAL -2
#define FT_INTERN_ERROR -3
#define FT_GEN_ERROR -4

#define FT_IS_SUCCESS(arg) ((arg) >= 0)
#define FT_IS_ERROR(arg) ((arg) < 0)

        struct ft_table_t;

        ft_table_t* ft_create_table(void);
        void        ft_destroy_table(ft_table_t* table);
        ft_table_t* ft_copy_table(ft_table_t* table);

        s32  ft_ln(ft_table_t* table);
        s32  ft_cur_row(const ft_table_t* table);
        s32  ft_cur_col(const ft_table_t* table);
        void ft_set_cur_cell(ft_table_t* table, s32 row, s32 col);
        s32  ft_is_empty(const ft_table_t* table);
        s32  ft_row_count(const ft_table_t* table);
        s32  ft_col_count(const ft_table_t* table);
        s32  ft_erase_range(ft_table_t* table, s32 top_left_row, s32 top_left_col, s32 bottom_right_row, s32 bottom_right_col);

        void ft_set_default_printf_field_separator(char separator);

        s32 ft_nwrite(ft_table_t* table, s32 count, const char* cell_content, ...);
        s32 ft_nwrite_ln(ft_table_t* table, s32 count, const char* cell_content, ...);

        s32 ft_row_write(ft_table_t* table, s32 cols, const char* row_cells[]);
        s32 ft_row_write_ln(ft_table_t* table, s32 cols, const char* row_cells[]);
        s32 ft_table_write(ft_table_t* table, s32 rows, s32 cols, const char* table_cells[]);
        s32 ft_table_write_ln(ft_table_t* table, s32 rows, s32 cols, const char* table_cells[]);
        s32 ft_add_separator(ft_table_t* table);

        const char* ft_to_string(const ft_table_t* table);

        struct ft_border_chars
        {
            const char* top_border_ch;
            const char* separator_ch;
            const char* bottom_border_ch;
            const char* side_border_ch;
            const char* out_intersect_ch;
            const char* in_intersect_ch;
        };

        struct ft_border_style
        {
            struct ft_border_chars border_chs;
            struct ft_border_chars header_border_chs;
            const char*            hor_separator_char;
        };

        extern const struct ft_border_style* const FT_BASIC_STYLE;
        extern const struct ft_border_style* const FT_BASIC2_STYLE;
        extern const struct ft_border_style* const FT_SIMPLE_STYLE;
        extern const struct ft_border_style* const FT_PLAIN_STYLE;
        extern const struct ft_border_style* const FT_DOT_STYLE;
        extern const struct ft_border_style* const FT_EMPTY_STYLE;
        extern const struct ft_border_style* const FT_EMPTY2_STYLE;
        extern const struct ft_border_style* const FT_SOLID_STYLE;
        extern const struct ft_border_style* const FT_SOLID_ROUND_STYLE;
        extern const struct ft_border_style* const FT_NICE_STYLE;
        extern const struct ft_border_style* const FT_DOUBLE_STYLE;
        extern const struct ft_border_style* const FT_DOUBLE2_STYLE;
        extern const struct ft_border_style* const FT_BOLD_STYLE;
        extern const struct ft_border_style* const FT_BOLD2_STYLE;
        extern const struct ft_border_style* const FT_FRAME_STYLE;

        s32 ft_set_default_border_style(const struct ft_border_style* style);
        s32 ft_set_border_style(ft_table_t* table, const struct ft_border_style* style);

/**
 * @name Special macros to define cell position (row and column).
 * @{
 */
#define FT_ANY_COLUMN (UINT_MAX)     /**< Any column (can be used to refer to all cells in a row)*/
#define FT_CUR_COLUMN (UINT_MAX - 1) /**< Current column */
#define FT_ANY_ROW (UINT_MAX)        /**< Any row (can be used to refer to all cells in a column)*/
#define FT_CUR_ROW (UINT_MAX - 1)    /**< Current row */
        /** @} */

#define FT_MAX_ROW_INDEX (UINT_MAX - 2)
#define FT_MAX_COL_INDEX (UINT_MAX - 2)

/**
 * @name Cell properties identifiers.
 * @{
 */
#define FT_CPROP_MIN_WIDTH (0x01U << 0)        /**< Minimum width */
#define FT_CPROP_TEXT_ALIGN (0x01U << 1)       /**< Text alignment */
#define FT_CPROP_TOP_PADDING (0x01U << 2)      /**< Top padding for cell content */
#define FT_CPROP_BOTTOM_PADDING (0x01U << 3)   /**< Bottom padding for cell content */
#define FT_CPROP_LEFT_PADDING (0x01U << 4)     /**< Left padding for cell content */
#define FT_CPROP_RIGHT_PADDING (0x01U << 5)    /**< Right padding for cell content */
#define FT_CPROP_EMPTY_STR_HEIGHT (0x01U << 6) /**< Height of empty cell */
#define FT_CPROP_ROW_TYPE (0x01U << 7)         /**< Row type */
#define FT_CPROP_CONT_FG_COLOR (0x01U << 8)    /**< Cell content foreground text color */
#define FT_CPROP_CELL_BG_COLOR (0x01U << 9)    /**< Cell background color */
#define FT_CPROP_CONT_BG_COLOR (0x01U << 10)   /**< Cell content background color */
#define FT_CPROP_CELL_TEXT_STYLE (0x01U << 11) /**< Cell text style */
#define FT_CPROP_CONT_TEXT_STYLE (0x01U << 12) /**< Cell content text style */
        /** @} */

        /**
         * Colors.
         */
        enum ft_color
        {
            FT_COLOR_DEFAULT       = 0,  /**< Default color */
            FT_COLOR_BLACK         = 1,  /**< Black color*/
            FT_COLOR_RED           = 2,  /**< Red color */
            FT_COLOR_GREEN         = 3,  /**< Green color */
            FT_COLOR_YELLOW        = 4,  /**< Yellow color */
            FT_COLOR_BLUE          = 5,  /**< Blue color */
            FT_COLOR_MAGENTA       = 6,  /**< Magenta color */
            FT_COLOR_CYAN          = 7,  /**< Cyan color */
            FT_COLOR_LIGHT_GRAY    = 8,  /**< Light gray color */
            FT_COLOR_DARK_GRAY     = 9,  /**< Dark gray color */
            FT_COLOR_LIGHT_RED     = 10, /**< Light red color */
            FT_COLOR_LIGHT_GREEN   = 11, /**< Light green color */
            FT_COLOR_LIGHT_YELLOW  = 12, /**< Light yellow color */
            FT_COLOR_LIGHT_BLUE    = 13, /**< Light blue color */
            FT_COLOR_LIGHT_MAGENTA = 14, /**< Light magenta color */
            FT_COLOR_LIGHT_CYAN    = 15, /**< Light cyan color */
            FT_COLOR_LIGHT_WHYTE   = 16  /**< Light whyte color */
        };

        enum ft_text_style
        {
            FT_TSTYLE_DEFAULT    = (1U << 0), /**< Default style */
            FT_TSTYLE_BOLD       = (1U << 1), /**< Bold */
            FT_TSTYLE_DIM        = (1U << 2), /**< Dim */
            FT_TSTYLE_ITALIC     = (1U << 3), /**< Italic */
            FT_TSTYLE_UNDERLINED = (1U << 4), /**< Underlined */
            FT_TSTYLE_BLINK      = (1U << 5), /**< Blink */
            FT_TSTYLE_INVERTED   = (1U << 6), /**< Reverse (invert the foreground and background colors) */
            FT_TSTYLE_HIDDEN     = (1U << 7)  /**< Hidden (useful for passwords)  */
        };

        enum ft_text_alignment
        {
            FT_ALIGNED_LEFT = 0, /**< Align left */
            FT_ALIGNED_CENTER,   /**< Align center */
            FT_ALIGNED_RIGHT     /**< Align right */
        };

        enum ft_row_type
        {
            FT_ROW_COMMON = 0, /**< Common row */
            FT_ROW_HEADER      /**< Header row */
        };

        s32 ft_set_default_cell_prop(uint32_t property, s32 value);
        s32 ft_set_cell_prop(ft_table_t* table, s32 row, s32 col, uint32_t property, s32 value);

/**
 * @name Table properties identifiers.
 * @{
 */
#define FT_TPROP_LEFT_MARGIN (0x01U << 0)
#define FT_TPROP_TOP_MARGIN (0x01U << 1)
#define FT_TPROP_RIGHT_MARGIN (0x01U << 2)
#define FT_TPROP_BOTTOM_MARGIN (0x01U << 3)
#define FT_TPROP_ADDING_STRATEGY (0x01U << 4)
        /** @} */

        enum ft_adding_strategy
        {
            FT_STRATEGY_REPLACE = 0, /**< Replace old content. */
            FT_STRATEGY_INSERT       /**< Insert new conten. Old content is shifted. */
        };

        s32  ft_set_default_tbl_prop(uint32_t property, s32 value);
        s32  ft_set_tbl_prop(ft_table_t* table, uint32_t property, s32 value);
        s32  ft_set_cell_span(ft_table_t* table, s32 row, s32 col, s32 hor_span);
        void ft_set_memory_funcs(void* (*f_malloc)(s32 size), void (*f_free)(void* ptr));

        const char* ft_strerror(s32 error_code);

    } // namespace nfort
} // namespace ncore

#endif