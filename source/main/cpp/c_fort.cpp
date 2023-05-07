
#define FT_AMALGAMED_SOURCE /* Macros to make internal libfort functions static */

#ifndef FORT_IMPL_H
#define FORT_IMPL_H

#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS /* To disable warnings for unsafe functions */
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "cfort/c_fort.h"

namespace ncore
{
    namespace nfort
    {

#ifdef FT_AMALGAMED_SOURCE
#define FT_INTERNAL static
#else
#define FT_INTERNAL
#endif /* FT_AMALGAMED_SORCE */

#define FORT_DEFAULT_COL_SEPARATOR '|'
        extern char g_col_separator;

#define FORT_COL_SEPARATOR_LENGTH 1

#define FORT_UNUSED __attribute__((unused))

#define F_MALLOC fort_malloc
#define F_FREE fort_free
#define F_CALLOC fort_calloc
#define F_REALLOC fort_realloc
#define F_STRDUP fort_strdup
#define F_WCSDUP fort_wcsdup
/* @todo: replace with custom impl !!!*/
#define F_UTF8DUP utf8dup

#define F_CREATE(type) ((type*)F_CALLOC(sizeof(type), 1))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define FT_NEWLINE "\n"
#define FT_SPACE " "

/*****************************************************************************
 *               DEFAULT_SIZES
 * ***************************************************************************/
#define DEFAULT_STR_BUF_SIZE 1024
#define DEFAULT_VECTOR_CAPACITY 10

        /*****************************************************************************
         *               DATA TYPES
         * ***************************************************************************/

        enum f_get_policy
        {
            CREATE_ON_NULL,
            DONT_CREATE_ON_NULL
        };

        enum f_bool
        {
            F_FALSE = 0,
            F_TRUE  = 1
        };

        enum f_cell_type
        {
            COMMON_CELL,
            GROUP_MASTER_CELL,
            GROUP_SLAVE_CELL
        };

        enum f_geometry_type
        {
            VISIBLE_GEOMETRY,
            INTERN_REPR_GEOMETRY
        };

        enum f_string_type
        {
            CHAR_BUF,
            UTF32_BUF,
        };

        struct f_string_view
        {
            union
            {
                const char* cstr;
                const void* data;
            } u;
            enum f_string_type type;
        };
        typedef struct f_string_view f_string_view_t;

#define FT_STR_2_CAT_(arg1, arg2) arg1##arg2
#define FT_STR_2_CAT(arg1, arg2) FT_STR_2_CAT_(arg1, arg2)

#define UNIQUE_NAME_(prefix) FT_STR_2_CAT(prefix, __COUNTER__)
#define UNIQUE_NAME(prefix) UNIQUE_NAME_(prefix)

        typedef int f_status;

        struct f_table_properties;
        struct f_row;
        struct f_vector;
        struct f_cell;
        struct f_string_buffer;
        struct f_separator
        {
            int enabled;
        };

        typedef struct f_table_properties f_table_properties_t;
        typedef struct f_vector           f_vector_t;
        typedef struct f_cell             f_cell_t;
        typedef struct f_string_buffer    f_string_buffer_t;
        typedef struct f_row              f_row_t;
        typedef struct f_separator        f_separator_t;

        struct f_context
        {
            f_table_properties_t* table_properties;
            s32                   row;
            s32                   column;
        };
        typedef struct f_context f_context_t;

        struct f_conv_context
        {
            union
            {
                char* buf;
            } u;
            s32                raw_avail;
            struct f_context*  cntx;
            enum f_string_type b_type;
        };
        typedef struct f_conv_context f_conv_context_t;

        /*****************************************************************************
         *               LIBFORT helpers
         *****************************************************************************/

        extern void* (*fort_malloc)(s32 size);
        extern void (*fort_free)(void* ptr);
        extern void* (*fort_calloc)(s32 nmemb, s32 size);
        extern void* (*fort_realloc)(void* ptr, s32 size);

        FT_INTERNAL
        void set_memory_funcs(void* (*f_malloc)(s32 size), void (*f_free)(void* ptr));

        FT_INTERNAL
        char* fort_strdup(const char* str);

        FT_INTERNAL
        s32 number_of_columns_in_format_string(const f_string_view_t* fmt);

        FT_INTERNAL
        s32 number_of_columns_in_format_buffer(const f_string_buffer_t* fmt);

        FT_INTERNAL
        int print_n_strings(f_conv_context_t* cntx, s32 n, const char* str);

        FT_INTERNAL
        int ft_nprint(f_conv_context_t* cntx, const char* str, s32 strlen);

/*#define PRINT_DEBUG_INFO fprintf(stderr, "error in %s(%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);*/
#define PRINT_DEBUG_INFO

#define FT_CHECK(statement)  \
    do                       \
    {                        \
        tmp = statement;     \
        if (tmp < 0)         \
        {                    \
            PRINT_DEBUG_INFO \
            goto clear;      \
        }                    \
    } while (0)

#define CHCK_RSLT_ADD_TO_WRITTEN(statement) \
    do                                      \
    {                                       \
        tmp = statement;                    \
        if (tmp < 0)                        \
        {                                   \
            PRINT_DEBUG_INFO                \
            goto clear;                     \
        }                                   \
        written += (s32)tmp;                \
    } while (0)

#define CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(statement) \
    do                                                \
    {                                                 \
        tmp = statement;                              \
        if (tmp < 0)                                  \
        {                                             \
            PRINT_DEBUG_INFO                          \
            goto clear;                               \
        }                                             \
        invisible_written += (s32)tmp;                \
    } while (0)

#define CHECK_NOT_NEGATIVE(x) \
    do                        \
    {                         \
        if ((x) < 0)          \
            goto fort_fail;   \
    } while (0)

#endif /* FORT_IMPL_H */

        /********************************************************
           End of file "fort_utils.h"
         ********************************************************/

        /********************************************************
           Begin of file "vector.h"
         ********************************************************/

#ifndef VECTOR_H
#define VECTOR_H

        /* #include "fort_utils.h" */ /* Commented by amalgamation script */

#define INVALID_VEC_INDEX ((s32)-1)

        FT_INTERNAL
        f_vector_t* create_vector(s32 item_size, s32 capacity);

        FT_INTERNAL
        void destroy_vector(f_vector_t*);

        FT_INTERNAL
        s32 vector_size(const f_vector_t*);

        FT_INTERNAL
        s32 vector_capacity(const f_vector_t*);

        FT_INTERNAL
        int vector_push(f_vector_t*, const void* item);

        FT_INTERNAL
        int vector_insert(f_vector_t*, const void* item, s32 pos);

        FT_INTERNAL
        f_vector_t* vector_split(f_vector_t*, s32 pos);

        FT_INTERNAL
        const void* vector_at_c(const f_vector_t* vector, s32 index);

        FT_INTERNAL
        void* vector_at(f_vector_t*, s32 index);

        FT_INTERNAL
        f_status vector_swap(f_vector_t* cur_vec, f_vector_t* mv_vec, s32 pos);

        FT_INTERNAL
        void vector_clear(f_vector_t*);

        FT_INTERNAL
        int vector_erase(f_vector_t*, s32 index);

#ifdef FT_TEST_BUILD
        f_vector_t* copy_vector(f_vector_t*);
        s32         vector_index_of(const f_vector_t*, const void* item);
#endif

#define VECTOR_AT(vector, pos, data_type) *(data_type*)vector_at((vector), (pos))
#define VECTOR_AT_C(vector, pos, const_data_type) *(const_data_type*)vector_at_c((vector), (pos))

#endif /* VECTOR_H */

        /********************************************************
           End of file "vector.h"
         ********************************************************/

        /********************************************************
           Begin of file "string_buffer.h"
         ********************************************************/

#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

        /*****************************************************************************
         *               STRING BUFFER
         * ***************************************************************************/

        struct f_string_buffer
        {
            union
            {
                char* cstr;
                void* data;
            } str;
            s32                data_sz;
            enum f_string_type type;
        };

        FT_INTERNAL
        f_string_buffer_t* create_string_buffer(s32 number_of_chars, enum f_string_type type);

        FT_INTERNAL
        void destroy_string_buffer(f_string_buffer_t* buffer);

        FT_INTERNAL
        f_string_buffer_t* copy_string_buffer(const f_string_buffer_t* buffer);

        FT_INTERNAL
        f_status realloc_string_buffer_without_copy(f_string_buffer_t* buffer);

        FT_INTERNAL
        f_status fill_buffer_from_string(f_string_buffer_t* buffer, const char* str);

        FT_INTERNAL
        s32 buffer_text_visible_width(const f_string_buffer_t* buffer);

        FT_INTERNAL
        s32 buffer_text_visible_height(const f_string_buffer_t* buffer);

        FT_INTERNAL
        s32 string_buffer_cod_width_capacity(const f_string_buffer_t* buffer);

        FT_INTERNAL
        s32 string_buffer_raw_capacity(const f_string_buffer_t* buffer);

        FT_INTERNAL
        s32 string_buffer_width_capacity(const f_string_buffer_t* buffer);

        FT_INTERNAL
        void* buffer_get_data(f_string_buffer_t* buffer);

        FT_INTERNAL
        int buffer_check_align(f_string_buffer_t* buffer);

        FT_INTERNAL
        int buffer_printf(f_string_buffer_t* buffer, s32 buffer_row, f_conv_context_t* cntx, s32 cod_width, const char* content_style_tag, const char* reset_content_style_tag);

#endif /* STRING_BUFFER_H */

        /********************************************************
           End of file "string_buffer.h"
         ********************************************************/

        /********************************************************
           Begin of file "properties.h"
         ********************************************************/

#ifndef PROPERTIES_H
#define PROPERTIES_H

#define PROP_IS_SET(ft_props, property) ((ft_props) & (property))
#define PROP_SET(ft_props, property) ((ft_props) |= (property))
#define PROP_UNSET(ft_props, property) ((ft_props) &= ~((uint32_t)(property)))

#define TEXT_STYLE_TAG_MAX_SIZE (64 * 2)

        FT_INTERNAL
        void get_style_tag_for_cell(const f_table_properties_t* props, s32 row, s32 col, char* style_tag, s32 sz);

        FT_INTERNAL
        void get_reset_style_tag_for_cell(const f_table_properties_t* props, s32 row, s32 col, char* style_tag, s32 sz);

        FT_INTERNAL
        void get_style_tag_for_content(const f_table_properties_t* props, s32 row, s32 col, char* style_tag, s32 sz);

        FT_INTERNAL
        void get_reset_style_tag_for_content(const f_table_properties_t* props, s32 row, s32 col, char* style_tag, s32 sz);

        struct f_cell_props
        {
            s32      cell_row;
            s32      cell_col;
            uint32_t properties_flags;

            unsigned int           col_min_width;
            enum ft_text_alignment align;
            unsigned int           cell_padding_top;
            unsigned int           cell_padding_bottom;
            unsigned int           cell_padding_left;
            unsigned int           cell_padding_right;
            unsigned int           cell_empty_string_height;
            enum ft_row_type       row_type;
            unsigned int           content_fg_color_number;
            unsigned int           content_bg_color_number;
            unsigned int           cell_bg_color_number;
            enum ft_text_style     cell_text_style;
            enum ft_text_style     content_text_style;
        };

        typedef struct f_cell_props f_cell_props_t;
        typedef f_vector_t          f_cell_prop_container_t;

        FT_INTERNAL
        f_cell_prop_container_t* create_cell_prop_container(void);

        FT_INTERNAL
        void destroy_cell_prop_container(f_cell_prop_container_t* cont);

        FT_INTERNAL
        const f_cell_props_t* cget_cell_prop(const f_cell_prop_container_t* cont, s32 row, s32 col);

        FT_INTERNAL
        f_cell_props_t* get_cell_prop_and_create_if_not_exists(f_cell_prop_container_t* cont, s32 row, s32 col);

        FT_INTERNAL
        f_status set_cell_property(f_cell_prop_container_t* cont, s32 row, s32 col, uint32_t property, int value);

        FT_INTERNAL
        int get_cell_property_hierarchically(const f_table_properties_t* properties, s32 row, s32 column, uint32_t property);

        FT_INTERNAL
        f_status set_default_cell_property(uint32_t property, int value);

        /*         TABLE BORDER DESСRIPTION
         *
         *
         *   TL TT TT TT TV TT TT TT TT TT TT TT TR
         *   LL          IV                      RR
         *   LL          IV                      RR
         *   LH IH IH IH II IH IH IH TI IH IH IH RH
         *   LL          IV          IV          RR
         *   LL          IV          IV          RR
         *   LL          LI IH IH IH RI          RH
         *   LL          IV          IV          RR
         *   LL          IV          IV          RR
         *   LH IH IH IH BI IH IH IH II IH IH IH RH
         *   LL                      IV          RR
         *   LL                      IV          RR
         *   BL BB BB BB BV BB BB BB BV BB BB BB BR
         */

        /*      HORIZONTAL SEPARATOR DESCRIPTION
         *
         *
         *   TL TT TT TT TV TT TT TT TV TT TT TT TR        <----- TOP_SEPARATOR
         *   LL          IV          IV          RR
         *   LH IH IH IH II IH IH IH II IH IH IH RH        <----- INSIDE_SEPARATOR
         *   LL          IV          IV          RR
         *   BL BB BB BB BV BB BB BB BV BB BB BB BR        <----- BOTTOM_SEPARATOR
         */

        enum f_hor_separator_pos
        {
            TOP_SEPARATOR,
            INSIDE_SEPARATOR,
            BOTTOM_SEPARATOR
        };

        enum f_border_item_pos
        {
            TL_bip = 0,
            TT_bip = 1,
            TV_bip = 2,
            TR_bip = 3,

            LL_bip = 4,
            IV_bip = 5,
            RR_bip = 6,

            LH_bip = 7,
            IH_bip = 8,
            II_bip = 9,
            RH_bip = 10,

            BL_bip = 11,
            BB_bip = 12,
            BV_bip = 13,
            BR_bip = 14,

            LI_bip = 15,
            TI_bip = 16,
            RI_bip = 17,
            BI_bip = 18,

            BORDER_ITEM_POS_SIZE
        };

        enum f_separator_item_pos
        {
            LH_sip = 0,
            IH_sip = 1,
            II_sip = 2,
            RH_sip = 3,

            TI_sip = 4,
            BI_sip = 5,

            SEPARATOR_ITEM_POS_SIZE
        };

        struct fort_border_style
        {
            const char* border_chars[BORDER_ITEM_POS_SIZE];
            const char* header_border_chars[BORDER_ITEM_POS_SIZE];
            const char* separator_chars[SEPARATOR_ITEM_POS_SIZE];
        };
        extern struct fort_border_style FORT_BASIC_STYLE;
        extern struct fort_border_style FORT_BASIC2_STYLE;
        extern struct fort_border_style FORT_SIMPLE_STYLE;
        extern struct fort_border_style FORT_PLAIN_STYLE;
        extern struct fort_border_style FORT_DOT_STYLE;
        extern struct fort_border_style FORT_EMPTY_STYLE;
        extern struct fort_border_style FORT_EMPTY2_STYLE;
        extern struct fort_border_style FORT_SOLID_STYLE;
        extern struct fort_border_style FORT_SOLID_ROUND_STYLE;
        extern struct fort_border_style FORT_NICE_STYLE;
        extern struct fort_border_style FORT_DOUBLE_STYLE;
        extern struct fort_border_style FORT_DOUBLE2_STYLE;
        extern struct fort_border_style FORT_BOLD_STYLE;
        extern struct fort_border_style FORT_BOLD2_STYLE;
        extern struct fort_border_style FORT_FRAME_STYLE;

        struct fort_entire_table_properties
        {
            unsigned int            left_margin;
            unsigned int            top_margin;
            unsigned int            right_margin;
            unsigned int            bottom_margin;
            enum ft_adding_strategy add_strategy;
        };
        typedef struct fort_entire_table_properties fort_entire_table_properties_t;
        extern fort_entire_table_properties_t       g_entire_table_properties;

        FT_INTERNAL
        f_status set_entire_table_property(f_table_properties_t* table_properties, uint32_t property, int value);

        FT_INTERNAL
        f_status set_default_entire_table_property(uint32_t property, int value);

        struct f_table_properties
        {
            struct fort_border_style       border_style;
            f_cell_prop_container_t*       cell_properties;
            fort_entire_table_properties_t entire_table_properties;
        };
        extern f_table_properties_t g_table_properties;

        FT_INTERNAL
        s32 max_border_elem_strlen(struct f_table_properties*);

        FT_INTERNAL
        f_table_properties_t* create_table_properties(void);

        FT_INTERNAL
        void destroy_table_properties(f_table_properties_t* properties);

        FT_INTERNAL
        f_table_properties_t* copy_table_properties(const f_table_properties_t* property);

#endif /* PROPERTIES_H */

        /********************************************************
           End of file "properties.h"
         ********************************************************/

        /********************************************************
           Begin of file "cell.h"
         ********************************************************/

#ifndef CELL_H
#define CELL_H

        /* #include "fort_utils.h" */ /* Commented by amalgamation script */

        FT_INTERNAL
        f_cell_t* create_cell(void);

        FT_INTERNAL
        void destroy_cell(f_cell_t* cell);

        FT_INTERNAL
        f_cell_t* copy_cell(f_cell_t* cell);

        FT_INTERNAL
        s32 cell_vis_width(const f_cell_t* cell, const f_context_t* context);

        FT_INTERNAL
        s32 cell_invis_codes_width(const f_cell_t* cell, const f_context_t* context);

        FT_INTERNAL
        s32 hint_height_cell(const f_cell_t* cell, const f_context_t* context);

        FT_INTERNAL
        void set_cell_type(f_cell_t* cell, enum f_cell_type type);

        FT_INTERNAL
        enum f_cell_type get_cell_type(const f_cell_t* cell);

        FT_INTERNAL
        int cell_printf(f_cell_t* cell, s32 row, f_conv_context_t* cntx, s32 cod_width);

        FT_INTERNAL
        f_status fill_cell_from_string(f_cell_t* cell, const char* str);

        FT_INTERNAL
        f_status fill_cell_from_buffer(f_cell_t* cell, const f_string_buffer_t* buf);

        FT_INTERNAL
        f_string_buffer_t* cell_get_string_buffer(f_cell_t* cell);

#endif /* CELL_H */

        /********************************************************
           End of file "cell.h"
         ********************************************************/

        /********************************************************
           Begin of file "row.h"
         ********************************************************/

#ifndef ROW_H
#define ROW_H

        FT_INTERNAL
        f_row_t* create_row(void);

        FT_INTERNAL
        void destroy_row(f_row_t* row);

        FT_INTERNAL
        f_row_t* copy_row(f_row_t* row);

        FT_INTERNAL
        f_row_t* split_row(f_row_t* row, s32 pos);

        // Delete range [left; right] of cells (both ends included)
        FT_INTERNAL
        int ft_row_erase_range(f_row_t* row, s32 left, s32 right);

        FT_INTERNAL
        f_row_t* create_row_from_string(const char* str);

        FT_INTERNAL
        f_row_t* create_row_from_fmt_string(const struct f_string_view* fmt, va_list* va_args);

        FT_INTERNAL
        s32 columns_in_row(const f_row_t* row);

        FT_INTERNAL
        f_cell_t* get_cell(f_row_t* row, s32 col);

        FT_INTERNAL
        const f_cell_t* get_cell_c(const f_row_t* row, s32 col);

        FT_INTERNAL
        f_cell_t* get_cell_and_create_if_not_exists(f_row_t* row, s32 col);

        FT_INTERNAL
        f_cell_t* create_cell_in_position(f_row_t* row, s32 col);

        FT_INTERNAL
        f_status swap_row(f_row_t* cur_row, f_row_t* ins_row, s32 pos);

        FT_INTERNAL
        f_status insert_row(f_row_t* cur_row, f_row_t* ins_row, s32 pos);

        FT_INTERNAL
        s32 group_cell_number(const f_row_t* row, s32 master_cell_col);

        FT_INTERNAL
        int get_row_cell_types(const f_row_t* row, enum f_cell_type* types, s32 types_sz);

        FT_INTERNAL
        f_status row_set_cell_span(f_row_t* row, s32 cell_column, s32 hor_span);

        FT_INTERNAL
        int print_row_separator(f_conv_context_t* cntx, const s32* col_width_arr, s32 cols, const f_row_t* upper_row, const f_row_t* lower_row, enum f_hor_separator_pos separatorPos, const f_separator_t* sep);

        FT_INTERNAL
        int snprintf_row(const f_row_t* row, f_conv_context_t* cntx, s32* col_width_arr, s32 col_width_arr_sz, s32 row_height);

#endif /* ROW_H */

        /********************************************************
           End of file "row.h"
         ********************************************************/

        /********************************************************
           Begin of file "table.h"
         ********************************************************/

#ifndef TABLE_H
#define TABLE_H

        struct ft_table_t
        {
            f_vector_t*           rows;
            f_table_properties_t* properties;
            f_string_buffer_t*    conv_buffer;
            s32                   cur_row;
            s32                   cur_col;
            f_vector_t*           separators;
        };

        FT_INTERNAL
        f_separator_t* create_separator(int enabled);

        FT_INTERNAL
        void destroy_separator(f_separator_t* sep);

        FT_INTERNAL
        f_separator_t* copy_separator(f_separator_t* sep);

        FT_INTERNAL
        f_status get_table_sizes(const ft_table_t* table, s32* rows, s32* cols);

        FT_INTERNAL
        f_row_t* get_row(ft_table_t* table, s32 row);

        FT_INTERNAL
        const f_row_t* get_row_c(const ft_table_t* table, s32 row);

        FT_INTERNAL
        f_row_t* get_row_and_create_if_not_exists(ft_table_t* table, s32 row);

        FT_INTERNAL
        f_string_buffer_t* get_cur_str_buffer_and_create_if_not_exists(ft_table_t* table);

        FT_INTERNAL
        f_status table_rows_and_cols_geometry(const ft_table_t* table, s32** col_width_arr_p, s32* col_width_arr_sz, s32** row_height_arr_p, s32* row_height_arr_sz, enum f_geometry_type geom);

        FT_INTERNAL
        f_status table_geometry(const ft_table_t* table, s32* height, s32* width);

        /*
         * Returns geometry in codepoints(characters) (include codepoints of invisible
         * elements: e.g. styles tags).
         */
        FT_INTERNAL
        f_status table_internal_codepoints_geometry(const ft_table_t* table, s32* height, s32* width);

#endif /* TABLE_H */

        /********************************************************
           End of file "table.h"
         ********************************************************/

        /********************************************************
           Begin of file "cell.c"
         ********************************************************/

        struct f_cell
        {
            f_string_buffer_t* str_buffer;
            enum f_cell_type   cell_type;
        };

        FT_INTERNAL
        f_cell_t* create_cell(void)
        {
            f_cell_t* cell = (f_cell_t*)F_CALLOC(sizeof(f_cell_t), 1);
            if (cell == NULL)
                return NULL;
            cell->str_buffer = create_string_buffer(DEFAULT_STR_BUF_SIZE, CHAR_BUF);
            if (cell->str_buffer == NULL)
            {
                F_FREE(cell);
                return NULL;
            }
            cell->cell_type = COMMON_CELL;
            return cell;
        }

        FT_INTERNAL
        void destroy_cell(f_cell_t* cell)
        {
            if (cell == NULL)
                return;
            destroy_string_buffer(cell->str_buffer);
            F_FREE(cell);
        }

        FT_INTERNAL
        f_cell_t* copy_cell(f_cell_t* cell)
        {
            assert(cell);

            f_cell_t* result = create_cell();
            if (result == NULL)
                return NULL;
            destroy_string_buffer(result->str_buffer);
            result->str_buffer = copy_string_buffer(cell->str_buffer);
            if (result->str_buffer == NULL)
            {
                destroy_cell(result);
                return NULL;
            }
            result->cell_type = cell->cell_type;
            return result;
        }

        FT_INTERNAL
        void set_cell_type(f_cell_t* cell, enum f_cell_type type)
        {
            assert(cell);
            cell->cell_type = type;
        }

        FT_INTERNAL
        enum f_cell_type get_cell_type(const f_cell_t* cell)
        {
            assert(cell);
            return cell->cell_type;
        }

        FT_INTERNAL
        s32 cell_vis_width(const f_cell_t* cell, const f_context_t* context)
        {
            /* todo:
             * At the moment min width includes paddings. Maybe it is better that min width weren't include
             * paddings but be min width of the cell content without padding
             */

            assert(cell);
            assert(context);

            f_table_properties_t* properties = context->table_properties;
            s32                   row        = context->row;
            s32                   column     = context->column;

            s32 padding_left  = get_cell_property_hierarchically(properties, row, column, FT_CPROP_LEFT_PADDING);
            s32 padding_right = get_cell_property_hierarchically(properties, row, column, FT_CPROP_RIGHT_PADDING);
            s32 result        = padding_left + padding_right;
            if (cell->str_buffer && cell->str_buffer->str.data)
            {
                result += buffer_text_visible_width(cell->str_buffer);
            }
            result = MAX(result, (s32)get_cell_property_hierarchically(properties, row, column, FT_CPROP_MIN_WIDTH));
            return result;
        }

        FT_INTERNAL
        s32 cell_invis_codes_width(const f_cell_t* cell, const f_context_t* context)
        {
            assert(cell);
            assert(context);

            f_table_properties_t* properties = context->table_properties;
            s32                   row        = context->row;
            s32                   column     = context->column;

            s32  result = 0;
            char cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
            get_style_tag_for_cell(properties, row, column, cell_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
            result += strlen(cell_style_tag);

            char reset_cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
            get_reset_style_tag_for_cell(properties, row, column, reset_cell_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
            result += strlen(reset_cell_style_tag);

            char content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
            get_style_tag_for_content(properties, row, column, content_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
            result += strlen(content_style_tag);

            char reset_content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
            get_reset_style_tag_for_content(properties, row, column, reset_content_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
            result += strlen(reset_content_style_tag);
            return result;
        }

        FT_INTERNAL
        s32 hint_height_cell(const f_cell_t* cell, const f_context_t* context)
        {
            assert(cell);
            assert(context);
            f_table_properties_t* properties = context->table_properties;
            s32                   row        = context->row;
            s32                   column     = context->column;

            s32 padding_top         = get_cell_property_hierarchically(properties, row, column, FT_CPROP_TOP_PADDING);
            s32 padding_bottom      = get_cell_property_hierarchically(properties, row, column, FT_CPROP_BOTTOM_PADDING);
            s32 empty_string_height = get_cell_property_hierarchically(properties, row, column, FT_CPROP_EMPTY_STR_HEIGHT);

            s32 result = padding_top + padding_bottom;
            if (cell->str_buffer && cell->str_buffer->str.data)
            {
                s32 text_height = buffer_text_visible_height(cell->str_buffer);
                result += text_height == 0 ? empty_string_height : text_height;
            }
            return result;
        }

        FT_INTERNAL
        int cell_printf(f_cell_t* cell, s32 row, f_conv_context_t* cntx, s32 vis_width)
        {
            const f_context_t* context = cntx->cntx;
            s32                buf_len = vis_width;

            if (cell == NULL || (vis_width < cell_vis_width(cell, context)))
            {
                return -1;
            }

            f_table_properties_t* properties    = context->table_properties;
            unsigned int          padding_top   = get_cell_property_hierarchically(properties, context->row, context->column, FT_CPROP_TOP_PADDING);
            unsigned int          padding_left  = get_cell_property_hierarchically(properties, context->row, context->column, FT_CPROP_LEFT_PADDING);
            unsigned int          padding_right = get_cell_property_hierarchically(properties, context->row, context->column, FT_CPROP_RIGHT_PADDING);

            s32 written           = 0;
            s32 invisible_written = 0;
            int tmp               = 0;

            /* todo: Dirty hack with changing buf_len! need refactoring. */
            /* Also maybe it is better to move all struff with colors to buffers? */
            char cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
            get_style_tag_for_cell(properties, context->row, context->column, cell_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
            buf_len += strlen(cell_style_tag);

            char reset_cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
            get_reset_style_tag_for_cell(properties, context->row, context->column, reset_cell_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
            buf_len += strlen(reset_cell_style_tag);

            char content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
            get_style_tag_for_content(properties, context->row, context->column, content_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
            buf_len += strlen(content_style_tag);

            char reset_content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
            get_reset_style_tag_for_content(properties, context->row, context->column, reset_content_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
            buf_len += strlen(reset_content_style_tag);

            /*    CELL_STYLE_T   LEFT_PADDING   CONTENT_STYLE_T  CONTENT   RESET_CONTENT_STYLE_T    RIGHT_PADDING   RESET_CELL_STYLE_T
             *  |              |              |                |         |                       |                |                    |
             *        L1                                                                                                    R1
             *                     L2                                                                   R2
             *                                     L3                               R3
             */

            s32 L2 = padding_left;

            s32 R2 = padding_right;
            s32 R3 = strlen(reset_cell_style_tag);

#define TOTAL_WRITTEN (written + invisible_written)
#define RIGHT (padding_right + extra_right)

#define WRITE_CELL_STYLE_TAG CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(print_n_strings(cntx, 1, cell_style_tag))
#define WRITE_RESET_CELL_STYLE_TAG CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(print_n_strings(cntx, 1, reset_cell_style_tag))
#define WRITE_CONTENT_STYLE_TAG CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(print_n_strings(cntx, 1, content_style_tag))
#define WRITE_RESET_CONTENT_STYLE_TAG CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(print_n_strings(cntx, 1, reset_content_style_tag))

            if (row >= hint_height_cell(cell, context) || row < padding_top || row >= (padding_top + buffer_text_visible_height(cell->str_buffer)))
            {
                WRITE_CELL_STYLE_TAG;
                WRITE_CONTENT_STYLE_TAG;
                WRITE_RESET_CONTENT_STYLE_TAG;
                CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, buf_len - TOTAL_WRITTEN - R3, FT_SPACE));
                WRITE_RESET_CELL_STYLE_TAG;

                return (int)TOTAL_WRITTEN;
            }

            WRITE_CELL_STYLE_TAG;
            CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, L2, FT_SPACE));
            if (cell->str_buffer)
            {
                CHCK_RSLT_ADD_TO_WRITTEN(buffer_printf(cell->str_buffer, row - padding_top, cntx, vis_width - L2 - R2, content_style_tag, reset_content_style_tag));
            }
            else
            {
                WRITE_CONTENT_STYLE_TAG;
                WRITE_RESET_CONTENT_STYLE_TAG;
                CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, vis_width - L2 - R2, FT_SPACE));
            }
            CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, R2, FT_SPACE));
            WRITE_RESET_CELL_STYLE_TAG;

            return (int)TOTAL_WRITTEN;

        clear:
            return -1;
#undef WRITE_CELL_STYLE_TAG
#undef WRITE_RESET_CELL_STYLE_TAG
#undef WRITE_CONTENT_STYLE_TAG
#undef WRITE_RESET_CONTENT_STYLE_TAG
#undef TOTAL_WRITTEN
#undef RIGHT
        }

        FT_INTERNAL
        f_status fill_cell_from_string(f_cell_t* cell, const char* str)
        {
            assert(str);
            assert(cell);

            return fill_buffer_from_string(cell->str_buffer, str);
        }

        FT_INTERNAL
        f_string_buffer_t* cell_get_string_buffer(f_cell_t* cell)
        {
            assert(cell);
            assert(cell->str_buffer);
            return cell->str_buffer;
        }

        FT_INTERNAL
        f_status fill_cell_from_buffer(f_cell_t* cell, const f_string_buffer_t* buffer)
        {
            assert(cell);
            assert(buffer);
            switch (buffer->type)
            {
                case CHAR_BUF: return fill_cell_from_string(cell, buffer->str.cstr);
                default: assert(0); return FT_GEN_ERROR;
            }
        }

        /********************************************************
           End of file "cell.c"
         ********************************************************/

        ft_table_t* ft_create_table(void)
        {
            ft_table_t* result = (ft_table_t*)F_CALLOC(1, sizeof(ft_table_t));
            if (result == NULL)
                return NULL;

            result->rows = create_vector(sizeof(f_row_t*), DEFAULT_VECTOR_CAPACITY);
            if (result->rows == NULL)
            {
                F_FREE(result);
                return NULL;
            }
            result->separators = create_vector(sizeof(f_separator_t*), DEFAULT_VECTOR_CAPACITY);
            if (result->separators == NULL)
            {
                destroy_vector(result->rows);
                F_FREE(result);
                return NULL;
            }

            result->properties = create_table_properties();
            if (result->properties == NULL)
            {
                destroy_vector(result->separators);
                destroy_vector(result->rows);
                F_FREE(result);
                return NULL;
            }
            result->conv_buffer = NULL;
            result->cur_row     = 0;
            result->cur_col     = 0;
            return result;
        }

        void ft_destroy_table(ft_table_t* table)
        {
            s32 i = 0;

            if (table == NULL)
                return;

            if (table->rows)
            {
                s32 row_n = vector_size(table->rows);
                for (i = 0; i < row_n; ++i)
                {
                    destroy_row(VECTOR_AT(table->rows, i, f_row_t*));
                }
                destroy_vector(table->rows);
            }
            if (table->separators)
            {
                s32 row_n = vector_size(table->separators);
                for (i = 0; i < row_n; ++i)
                {
                    destroy_separator(VECTOR_AT(table->separators, i, f_separator_t*));
                }
                destroy_vector(table->separators);
            }
            destroy_table_properties(table->properties);
            destroy_string_buffer(table->conv_buffer);
            F_FREE(table);
        }

        ft_table_t* ft_copy_table(ft_table_t* table)
        {
            if (table == NULL)
                return NULL;

            ft_table_t* result = ft_create_table();
            if (result == NULL)
                return NULL;

            s32 i      = 0;
            s32 rows_n = vector_size(table->rows);
            for (i = 0; i < rows_n; ++i)
            {
                f_row_t* row     = VECTOR_AT(table->rows, i, f_row_t*);
                f_row_t* new_row = copy_row(row);
                if (new_row == NULL)
                {
                    ft_destroy_table(result);
                    return NULL;
                }
                vector_push(result->rows, &new_row);
            }

            s32 sep_sz = vector_size(table->separators);
            for (i = 0; i < sep_sz; ++i)
            {
                f_separator_t* sep     = VECTOR_AT(table->separators, i, f_separator_t*);
                f_separator_t* new_sep = copy_separator(sep);
                if (new_sep == NULL)
                {
                    ft_destroy_table(result);
                    return NULL;
                }
                vector_push(result->separators, &new_sep);
            }

            /* note: by default new table has allocated default properties, so we
             * have to destroy them first.
             */
            if (result->properties)
            {
                destroy_table_properties(result->properties);
            }
            result->properties = copy_table_properties(table->properties);
            if (result->properties == NULL)
            {
                ft_destroy_table(result);
                return NULL;
            }

            /* todo: copy conv_buffer  ??  */

            result->cur_row = table->cur_row;
            result->cur_col = table->cur_col;
            return result;
        }

        static int split_cur_row(ft_table_t* table, f_row_t** tail_of_cur_row)
        {
            if (table->cur_row >= vector_size(table->rows))
            {
                tail_of_cur_row = NULL;
                return 0;
            }

            f_row_t* row = VECTOR_AT(table->rows, table->cur_row, f_row_t*);
            if (table->cur_col >= columns_in_row(row))
            {
                tail_of_cur_row = NULL;
                return 0;
            }

            f_row_t* tail = split_row(row, table->cur_col);
            if (!tail)
            {
                tail_of_cur_row = NULL;
                return FT_GEN_ERROR;
            }

            *tail_of_cur_row = tail;
            return 0;
        }

        int ft_ln(ft_table_t* table)
        {
            assert(table);
            fort_entire_table_properties_t* table_props = &table->properties->entire_table_properties;
            switch (table_props->add_strategy)
            {
                case FT_STRATEGY_INSERT:
                {
                    f_row_t* new_row = NULL;
                    if (FT_IS_ERROR(split_cur_row(table, &new_row)))
                    {
                        return FT_GEN_ERROR;
                    }
                    if (new_row)
                    {
                        if (FT_IS_ERROR(vector_insert(table->rows, &new_row, table->cur_row + 1)))
                        {
                            destroy_row(new_row);
                            return FT_GEN_ERROR;
                        }
                    }
                    break;
                }
                case FT_STRATEGY_REPLACE:
                    // do nothing
                    break;
                default: assert(0 && "Unexpected situation inside libfort"); break;
            }
            table->cur_col = 0;
            table->cur_row++;
            return FT_SUCCESS;
        }

        s32 ft_cur_row(const ft_table_t* table)
        {
            assert(table);
            return table->cur_row;
        }

        s32 ft_cur_col(const ft_table_t* table)
        {
            assert(table);
            return table->cur_col;
        }

        void ft_set_cur_cell(ft_table_t* table, s32 row, s32 col)
        {
            assert(table);
            table->cur_row = row;
            table->cur_col = col;
        }

        int ft_is_empty(const ft_table_t* table)
        {
            assert(table);
            return ft_row_count(table) == 0;
        }

        s32 ft_row_count(const ft_table_t* table)
        {
            assert(table && table->rows);
            return vector_size(table->rows);
        }

        s32 ft_col_count(const ft_table_t* table)
        {
            assert(table && table->rows);
            s32 i      = 0;
            s32 cols_n = 0;
            s32 rows_n = vector_size(table->rows);
            for (i = 0; i < rows_n; ++i)
            {
                f_row_t* row   = VECTOR_AT(table->rows, i, f_row_t*);
                s32      ncols = columns_in_row(row);
                cols_n         = MAX(cols_n, ncols);
            }
            return cols_n;
        }

        int ft_erase_range(ft_table_t* table, s32 top_left_row, s32 top_left_col, s32 bottom_right_row, s32 bottom_right_col)
        {
            assert(table && table->rows);
            int status = FT_SUCCESS;

            s32 rows_n = vector_size(table->rows);

            if (top_left_row == FT_CUR_ROW)
                top_left_row = table->cur_row;
            if (bottom_right_row == FT_CUR_ROW)
                bottom_right_row = table->cur_row;

            if (top_left_col == FT_CUR_COLUMN)
                top_left_col = table->cur_row;
            if (bottom_right_col == FT_CUR_COLUMN)
                bottom_right_col = table->cur_row;

            if (top_left_row > bottom_right_row || top_left_col > bottom_right_col)
                return FT_EINVAL;

            f_row_t* row = NULL;
            s32      i   = top_left_row;
            while (i < rows_n && i <= bottom_right_row)
            {
                row    = VECTOR_AT(table->rows, i, f_row_t*);
                status = ft_row_erase_range(row, top_left_col, bottom_right_col);
                if (FT_IS_ERROR(status))
                    return status;
                ++i;
            }

            f_separator_t* separator = NULL;

            s32 n_iterations = MIN(rows_n - 1, bottom_right_row) - top_left_row + 1;
            s32 j            = 0;
            i                = top_left_row;
            for (j = 0; j < n_iterations; ++j)
            {
                row = VECTOR_AT(table->rows, i, f_row_t*);
                if (columns_in_row(row))
                {
                    ++i;
                }
                else
                {
                    destroy_row(row);
                    status = vector_erase(table->rows, i);
                    if (FT_IS_ERROR(status))
                        return status;
                    if (i < vector_size(table->separators))
                    {
                        separator = VECTOR_AT(table->separators, i, f_separator_t*);
                        destroy_separator(separator);
                        vector_erase(table->separators, i);
                    }
                }
            }

            return FT_SUCCESS;
        }

        static int ft_row_printf_impl_(ft_table_t* table, s32 row, const struct f_string_view* fmt, va_list* va)
        {
            s32 i        = 0;
            s32 new_cols = 0;

            if (table == NULL)
                return -1;

            f_row_t* new_row = create_row_from_fmt_string(fmt, va);

            if (new_row == NULL)
            {
                return -1;
            }

            f_row_t** cur_row_p = NULL;
            s32       sz        = vector_size(table->rows);
            if (row >= sz)
            {
                s32 push_n = row - sz + 1;
                for (i = 0; i < push_n; ++i)
                {
                    f_row_t* padding_row = create_row();
                    if (padding_row == NULL)
                        goto clear;

                    if (FT_IS_ERROR(vector_push(table->rows, &padding_row)))
                    {
                        destroy_row(padding_row);
                        goto clear;
                    }
                }
            }
            /* todo: clearing pushed items in case of error ?? */

            new_cols  = columns_in_row(new_row);
            cur_row_p = &VECTOR_AT(table->rows, row, f_row_t*);

            switch (table->properties->entire_table_properties.add_strategy)
            {
                case FT_STRATEGY_INSERT:
                {
                    if (FT_IS_ERROR(insert_row(*cur_row_p, new_row, table->cur_col)))
                        goto clear;
                    break;
                }
                case FT_STRATEGY_REPLACE:
                {
                    if (FT_IS_ERROR(swap_row(*cur_row_p, new_row, table->cur_col)))
                        goto clear;
                    break;
                }
                default: assert(0 && "Unexpected situation inside libfort"); break;
            }

            table->cur_col += new_cols;
            destroy_row(new_row);
            return (int)new_cols;

        clear:
            destroy_row(new_row);
            return -1;
        }

#if defined(FT_CLANG_COMPILER) || defined(FT_GCC_COMPILER)
#define FT_PRINTF ft_printf
#define FT_PRINTF_LN ft_printf_ln
#else
#define FT_PRINTF ft_printf_impl
#define FT_PRINTF_LN ft_printf_ln_impl
#endif

        int FT_PRINTF(ft_table_t* table, const char* fmt, ...)
        {
            assert(table);
            va_list va;
            va_start(va, fmt);

            struct f_string_view fmt_str;
            fmt_str.type   = CHAR_BUF;
            fmt_str.u.cstr = fmt;
            int result     = ft_row_printf_impl_(table, table->cur_row, &fmt_str, &va);
            va_end(va);
            return result;
        }

        int FT_PRINTF_LN(ft_table_t* table, const char* fmt, ...)
        {
            assert(table);
            va_list va;
            va_start(va, fmt);

            struct f_string_view fmt_str;
            fmt_str.type   = CHAR_BUF;
            fmt_str.u.cstr = fmt;
            int result     = ft_row_printf_impl_(table, table->cur_row, &fmt_str, &va);
            if (result >= 0)
            {
                ft_ln(table);
            }
            va_end(va);
            return result;
        }

#undef FT_PRINTF
#undef FT_PRINTF_LN

        void ft_set_default_printf_field_separator(char separator) { g_col_separator = separator; }

        static int ft_write_impl_(ft_table_t* table, const f_string_view_t* cell_content)
        {
            assert(table);
            f_string_buffer_t* buf = get_cur_str_buffer_and_create_if_not_exists(table);
            if (buf == NULL)
                return FT_GEN_ERROR;

            int status = FT_SUCCESS;
            switch (cell_content->type)
            {
                case CHAR_BUF: status = fill_buffer_from_string(buf, cell_content->u.cstr); break;
                default: status = FT_GEN_ERROR;
            }
            if (FT_IS_SUCCESS(status))
            {
                table->cur_col++;
            }
            return status;
        }

        static int ft_write_impl(ft_table_t* table, const char* cell_content)
        {
            f_string_view_t content;
            content.type   = CHAR_BUF;
            content.u.cstr = cell_content;
            return ft_write_impl_(table, &content);
        }

        int ft_nwrite(ft_table_t* table, s32 count, const char* cell_content, ...)
        {
            s32 i = 0;
            assert(table);
            int status = ft_write_impl(table, cell_content);
            if (FT_IS_ERROR(status))
                return status;

            va_list va;
            va_start(va, cell_content);
            --count;
            for (i = 0; i < count; ++i)
            {
                const char* cell = va_arg(va, const char*);
                status           = ft_write_impl(table, cell);
                if (FT_IS_ERROR(status))
                {
                    va_end(va);
                    return status;
                }
            }
            va_end(va);
            return status;
        }

        int ft_nwrite_ln(ft_table_t* table, s32 count, const char* cell_content, ...)
        {
            s32 i = 0;
            assert(table);
            int status = ft_write_impl(table, cell_content);
            if (FT_IS_ERROR(status))
                return status;

            va_list va;
            va_start(va, cell_content);
            --count;
            for (i = 0; i < count; ++i)
            {
                const char* cell = va_arg(va, const char*);
                status           = ft_write_impl(table, cell);
                if (FT_IS_ERROR(status))
                {
                    va_end(va);
                    return status;
                }
            }
            va_end(va);

            ft_ln(table);
            return status;
        }

        int ft_row_write(ft_table_t* table, s32 cols, const char* cells[])
        {
            s32 i = 0;
            assert(table);
            for (i = 0; i < cols; ++i)
            {
                int status = ft_write_impl(table, cells[i]);
                if (FT_IS_ERROR(status))
                {
                    /* todo: maybe current pos in case of error should be equal to the one before function call? */
                    return status;
                }
            }
            return FT_SUCCESS;
        }

        int ft_row_write_ln(ft_table_t* table, s32 cols, const char* cells[])
        {
            assert(table);
            int status = ft_row_write(table, cols, cells);
            if (FT_IS_SUCCESS(status))
            {
                ft_ln(table);
            }
            return status;
        }

        int ft_table_write(ft_table_t* table, s32 rows, s32 cols, const char* table_cells[])
        {
            s32 i = 0;
            assert(table);
            for (i = 0; i < rows; ++i)
            {
                int status = ft_row_write(table, cols, (const char**)&table_cells[i * cols]);
                if (FT_IS_ERROR(status))
                {
                    /* todo: maybe current pos in case of error should be equal
                     * to the one before function call?
                     */
                    return status;
                }
                if (i != rows - 1)
                    ft_ln(table);
            }
            return FT_SUCCESS;
        }

        int ft_table_write_ln(ft_table_t* table, s32 rows, s32 cols, const char* table_cells[])
        {
            assert(table);
            int status = ft_table_write(table, rows, cols, table_cells);
            if (FT_IS_SUCCESS(status))
            {
                ft_ln(table);
            }
            return status;
        }

#ifdef FT_HAVE_WCHAR
        int ft_table_wwrite(ft_table_t* table, s32 rows, s32 cols, const wchar_t* table_cells[])
        {
            s32 i = 0;
            assert(table);
            for (i = 0; i < rows; ++i)
            {
                int status = ft_row_wwrite(table, cols, (const wchar_t**)&table_cells[i * cols]);
                if (FT_IS_ERROR(status))
                {
                    /* todo: maybe current pos in case of error should be equal
                     * to the one before function call?
                     */
                    return status;
                }
                if (i != rows - 1)
                    ft_ln(table);
            }
            return FT_SUCCESS;
        }

        int ft_table_wwrite_ln(ft_table_t* table, s32 rows, s32 cols, const wchar_t* table_cells[])
        {
            assert(table);
            int status = ft_table_wwrite(table, rows, cols, table_cells);
            if (FT_IS_SUCCESS(status))
            {
                ft_ln(table);
            }
            return status;
        }
#endif

        static const char* empty_str_arr[] = {"", (const char*)L"", ""};

        static const void* ft_to_string_impl(const ft_table_t* table, enum f_string_type b_type)
        {
            assert(table);

            const char* result = NULL;

            /* Determine size of table string representation */
            s32 cod_height = 0;
            s32 cod_width  = 0;
            int status     = table_internal_codepoints_geometry(table, &cod_height, &cod_width);
            if (FT_IS_ERROR(status))
            {
                return NULL;
            }
            s32 n_codepoints = cod_height * cod_width + 1;

            /* Allocate string buffer for string representation */
            if (table->conv_buffer == NULL)
            {
                ((ft_table_t*)table)->conv_buffer = create_string_buffer(n_codepoints, b_type);
                if (table->conv_buffer == NULL)
                    return NULL;
            }
            while (string_buffer_cod_width_capacity(table->conv_buffer) < n_codepoints)
            {
                if (FT_IS_ERROR(realloc_string_buffer_without_copy(table->conv_buffer)))
                {
                    return NULL;
                }
            }
            if (!buffer_check_align(table->conv_buffer))
                return NULL;
            char* buffer = (char*)buffer_get_data(table->conv_buffer);

            s32  cols               = 0;
            s32  rows               = 0;
            s32* col_vis_width_arr  = NULL;
            s32* row_vis_height_arr = NULL;
            status                  = table_rows_and_cols_geometry(table, &col_vis_width_arr, &cols, &row_vis_height_arr, &rows, VISIBLE_GEOMETRY);
            if (FT_IS_ERROR(status))
                return NULL;

            if (rows == 0)
            {
                F_FREE(col_vis_width_arr);
                F_FREE(row_vis_height_arr);
                return empty_str_arr[b_type];
            }

            int         tmp = 0;
            s32         i   = 0;
            f_context_t context;
            context.table_properties = (table->properties ? table->properties : &g_table_properties);
            f_row_t*       prev_row  = NULL;
            f_row_t*       cur_row   = NULL;
            f_separator_t* cur_sep   = NULL;
            s32            sep_size  = vector_size(table->separators);

            f_conv_context_t cntx;
            cntx.u.buf     = buffer;
            cntx.raw_avail = string_buffer_raw_capacity(table->conv_buffer);
            cntx.cntx      = &context;
            cntx.b_type    = b_type;

            /* Print top margin */
            for (i = 0; i < context.table_properties->entire_table_properties.top_margin; ++i)
            {
                FT_CHECK(print_n_strings(&cntx, cod_width - 1 /* minus new_line*/, FT_SPACE));
                FT_CHECK(print_n_strings(&cntx, 1, FT_NEWLINE));
            }

            for (i = 0; i < rows; ++i)
            {
                cur_sep                               = (i < sep_size) ? VECTOR_AT(table->separators, i, f_separator_t*) : NULL;
                cur_row                               = VECTOR_AT(table->rows, i, f_row_t*);
                enum f_hor_separator_pos separatorPos = (i == 0) ? TOP_SEPARATOR : INSIDE_SEPARATOR;
                context.row                           = i;
                FT_CHECK(print_row_separator(&cntx, col_vis_width_arr, cols, prev_row, cur_row, separatorPos, cur_sep));
                FT_CHECK(snprintf_row(cur_row, &cntx, col_vis_width_arr, cols, row_vis_height_arr[i]));
                prev_row = cur_row;
            }
            cur_row     = NULL;
            cur_sep     = (i < sep_size) ? VECTOR_AT(table->separators, i, f_separator_t*) : NULL;
            context.row = i;
            FT_CHECK(print_row_separator(&cntx, col_vis_width_arr, cols, prev_row, cur_row, BOTTOM_SEPARATOR, cur_sep));

            /* Print bottom margin */
            for (i = 0; i < context.table_properties->entire_table_properties.bottom_margin; ++i)
            {
                FT_CHECK(print_n_strings(&cntx, cod_width - 1 /* minus new_line*/, FT_SPACE));
                FT_CHECK(print_n_strings(&cntx, 1, FT_NEWLINE));
            }

            result = buffer;

        clear:
            F_FREE(col_vis_width_arr);
            F_FREE(row_vis_height_arr);
            return result;
        }

        const char* ft_to_string(const ft_table_t* table) { return (const char*)ft_to_string_impl(table, CHAR_BUF); }

#ifdef FT_HAVE_WCHAR
        const wchar_t* ft_to_wstring(const ft_table_t* table) { return (const wchar_t*)ft_to_string_impl(table, W_CHAR_BUF); }
#endif

        int ft_add_separator(ft_table_t* table)
        {
            assert(table);
            assert(table->separators);

            while (vector_size(table->separators) <= table->cur_row)
            {
                f_separator_t* sep_p = create_separator(F_FALSE);
                if (sep_p == NULL)
                    return FT_MEMORY_ERROR;
                int status = vector_push(table->separators, &sep_p);
                if (FT_IS_ERROR(status))
                    return status;
            }

            f_separator_t** sep_p = &VECTOR_AT(table->separators, table->cur_row, f_separator_t*);
            if (*sep_p == NULL)
                *sep_p = create_separator(F_TRUE);
            else
                (*sep_p)->enabled = F_TRUE;

            if (*sep_p == NULL)
                return FT_GEN_ERROR;
            return FT_SUCCESS;
        }

        static const struct fort_border_style* built_in_styles[] = {
            &FORT_BASIC_STYLE,       &FORT_BASIC2_STYLE, &FORT_SIMPLE_STYLE, &FORT_PLAIN_STYLE,   &FORT_DOT_STYLE,  &FORT_EMPTY_STYLE, &FORT_EMPTY2_STYLE, &FORT_SOLID_STYLE,
            &FORT_SOLID_ROUND_STYLE, &FORT_NICE_STYLE,   &FORT_DOUBLE_STYLE, &FORT_DOUBLE2_STYLE, &FORT_BOLD_STYLE, &FORT_BOLD2_STYLE, &FORT_FRAME_STYLE,
        };
#define BUILT_IN_STYLES_SZ (sizeof(built_in_styles) / sizeof(built_in_styles[0]))

        /* todo: remove this stupid and dangerous code */
        static const struct ft_border_style built_in_external_styles[BUILT_IN_STYLES_SZ] = {{{"", "", "", "", "", ""}, {"", "", "", "", "", ""}, ""}};

        const struct ft_border_style* const FT_BASIC_STYLE       = &built_in_external_styles[0];
        const struct ft_border_style* const FT_BASIC2_STYLE      = &built_in_external_styles[1];
        const struct ft_border_style* const FT_SIMPLE_STYLE      = &built_in_external_styles[2];
        const struct ft_border_style* const FT_PLAIN_STYLE       = &built_in_external_styles[3];
        const struct ft_border_style* const FT_DOT_STYLE         = &built_in_external_styles[4];
        const struct ft_border_style* const FT_EMPTY_STYLE       = &built_in_external_styles[5];
        const struct ft_border_style* const FT_EMPTY2_STYLE      = &built_in_external_styles[6];
        const struct ft_border_style* const FT_SOLID_STYLE       = &built_in_external_styles[7];
        const struct ft_border_style* const FT_SOLID_ROUND_STYLE = &built_in_external_styles[8];
        const struct ft_border_style* const FT_NICE_STYLE        = &built_in_external_styles[9];
        const struct ft_border_style* const FT_DOUBLE_STYLE      = &built_in_external_styles[10];
        const struct ft_border_style* const FT_DOUBLE2_STYLE     = &built_in_external_styles[11];
        const struct ft_border_style* const FT_BOLD_STYLE        = &built_in_external_styles[12];
        const struct ft_border_style* const FT_BOLD2_STYLE       = &built_in_external_styles[13];
        const struct ft_border_style* const FT_FRAME_STYLE       = &built_in_external_styles[14];

        static void set_border_props_for_props(f_table_properties_t* properties, const struct ft_border_style* style)
        {
            if (style >= built_in_external_styles && style < (built_in_external_styles + BUILT_IN_STYLES_SZ))
            {
                s32 pos = (s32)(style - built_in_external_styles);
                memcpy(&(properties->border_style), built_in_styles[pos], sizeof(struct fort_border_style));
                return;
            }

            const struct ft_border_chars* border_chs        = &(style->border_chs);
            const struct ft_border_chars* header_border_chs = &(style->header_border_chs);

#define BOR_CHARS properties->border_style.border_chars
#define H_BOR_CHARS properties->border_style.header_border_chars
#define SEP_CHARS properties->border_style.separator_chars

            BOR_CHARS[TT_bip] = border_chs->top_border_ch;
            BOR_CHARS[IH_bip] = border_chs->separator_ch;
            BOR_CHARS[BB_bip] = border_chs->bottom_border_ch;
            BOR_CHARS[LL_bip] = BOR_CHARS[IV_bip] = BOR_CHARS[RR_bip] = border_chs->side_border_ch;

            BOR_CHARS[TL_bip] = BOR_CHARS[TV_bip] = BOR_CHARS[TR_bip] = border_chs->out_intersect_ch;
            BOR_CHARS[LH_bip] = BOR_CHARS[RH_bip] = border_chs->out_intersect_ch;
            BOR_CHARS[BL_bip] = BOR_CHARS[BV_bip] = BOR_CHARS[BR_bip] = border_chs->out_intersect_ch;
            BOR_CHARS[II_bip]                                         = border_chs->in_intersect_ch;

            BOR_CHARS[LI_bip] = BOR_CHARS[TI_bip] = BOR_CHARS[RI_bip] = BOR_CHARS[BI_bip] = border_chs->in_intersect_ch;

            if (strlen(border_chs->separator_ch) == 0 && strlen(border_chs->in_intersect_ch) == 0)
            {
                BOR_CHARS[LH_bip] = BOR_CHARS[RH_bip] = "\0";
            }

            H_BOR_CHARS[TT_bip] = header_border_chs->top_border_ch;
            H_BOR_CHARS[IH_bip] = header_border_chs->separator_ch;
            H_BOR_CHARS[BB_bip] = header_border_chs->bottom_border_ch;
            H_BOR_CHARS[LL_bip] = H_BOR_CHARS[IV_bip] = H_BOR_CHARS[RR_bip] = header_border_chs->side_border_ch;

            H_BOR_CHARS[TL_bip] = H_BOR_CHARS[TV_bip] = H_BOR_CHARS[TR_bip] = header_border_chs->out_intersect_ch;
            H_BOR_CHARS[LH_bip] = H_BOR_CHARS[RH_bip] = header_border_chs->out_intersect_ch;
            H_BOR_CHARS[BL_bip] = H_BOR_CHARS[BV_bip] = H_BOR_CHARS[BR_bip] = header_border_chs->out_intersect_ch;
            H_BOR_CHARS[II_bip]                                             = header_border_chs->in_intersect_ch;

            H_BOR_CHARS[LI_bip] = H_BOR_CHARS[TI_bip] = H_BOR_CHARS[RI_bip] = H_BOR_CHARS[BI_bip] = header_border_chs->in_intersect_ch;

            if (strlen(header_border_chs->separator_ch) == 0 && strlen(header_border_chs->in_intersect_ch) == 0)
            {
                BOR_CHARS[LH_bip] = BOR_CHARS[RH_bip] = "\0";
            }

            SEP_CHARS[LH_sip] = SEP_CHARS[RH_sip] = SEP_CHARS[II_sip] = header_border_chs->out_intersect_ch;
            SEP_CHARS[TI_sip] = SEP_CHARS[BI_sip] = header_border_chs->out_intersect_ch;
            SEP_CHARS[IH_sip]                     = style->hor_separator_char;

#undef BOR_CHARS
#undef H_BOR_CHARS
#undef SEP_CHARS
        }

        int ft_set_default_border_style(const struct ft_border_style* style)
        {
            set_border_props_for_props(&g_table_properties, style);
            return FT_SUCCESS;
        }

        int ft_set_border_style(ft_table_t* table, const struct ft_border_style* style)
        {
            assert(table);
            if (table->properties == NULL)
            {
                table->properties = create_table_properties();
                if (table->properties == NULL)
                    return FT_MEMORY_ERROR;
            }
            set_border_props_for_props(table->properties, style);
            return FT_SUCCESS;
        }

        int ft_set_cell_prop(ft_table_t* table, s32 row, s32 col, uint32_t property, int value)
        {
            assert(table);

            if (table->properties == NULL)
            {
                table->properties = create_table_properties();
                if (table->properties == NULL)
                    return FT_MEMORY_ERROR;
            }
            if (table->properties->cell_properties == NULL)
            {
                table->properties->cell_properties = create_cell_prop_container();
                if (table->properties->cell_properties == NULL)
                {
                    return FT_GEN_ERROR;
                }
            }

            if (row == FT_CUR_ROW)
                row = table->cur_row;
            if (col == FT_CUR_COLUMN)
                col = table->cur_col;

            return set_cell_property(table->properties->cell_properties, row, col, property, value);
        }

        int ft_set_default_cell_prop(uint32_t property, int value) { return set_default_cell_property(property, value); }

        int ft_set_default_tbl_prop(uint32_t property, int value) { return set_default_entire_table_property(property, value); }

        int ft_set_tbl_prop(ft_table_t* table, uint32_t property, int value)
        {
            assert(table);

            if (table->properties == NULL)
            {
                table->properties = create_table_properties();
                if (table->properties == NULL)
                    return FT_MEMORY_ERROR;
            }
            return set_entire_table_property(table->properties, property, value);
        }

        void ft_set_memory_funcs(void* (*f_malloc)(s32 size), void (*f_free)(void* ptr)) { set_memory_funcs(f_malloc, f_free); }

        const char* ft_strerror(int error_code)
        {
            switch (error_code)
            {
                case FT_MEMORY_ERROR: return "Out of memory";
                case FT_GEN_ERROR: return "General error";
                case FT_EINVAL: return "Invalid argument";
                case FT_INTERN_ERROR: return "Internal libfort error";
                default:
                    if (error_code < 0)
                        return "Unknown error code";
                    else
                        return "Success";
            }
        }

        int ft_set_cell_span(ft_table_t* table, s32 row, s32 col, s32 hor_span)
        {
            assert(table);
            if (hor_span < 2)
                return FT_EINVAL;

            if (row == FT_CUR_ROW)
                row = table->cur_row;
            if (row == FT_CUR_COLUMN)
                col = table->cur_col;

            f_row_t* row_p = get_row_and_create_if_not_exists(table, row);
            if (row_p == NULL)
                return FT_GEN_ERROR;

            return row_set_cell_span(row_p, col, hor_span);
        }

        /********************************************************
           End of file "fort_impl.c"
         ********************************************************/

        /********************************************************
           Begin of file "fort_utils.c"
         ********************************************************/

        char g_col_separator = FORT_DEFAULT_COL_SEPARATOR;

        /*****************************************************************************
         *               LIBFORT helpers
         *****************************************************************************/

#if defined(FT_GCC_COMPILER) || defined(FT_CLANG_COMPILER)
        void* (*fort_malloc)(s32 size)             = &malloc;
        void (*fort_free)(void* ptr)               = &free;
        void* (*fort_calloc)(s32 nmemb, s32 size)  = &calloc;
        void* (*fort_realloc)(void* ptr, s32 size) = &realloc;
#else
static void* local_malloc(s32 size) { return malloc(size); }
static void  local_free(void* ptr) { free(ptr); }
static void* local_calloc(s32 nmemb, s32 size) { return calloc(nmemb, size); }
static void* local_realloc(void* ptr, s32 size) { return realloc(ptr, size); }

void* (*fort_malloc)(s32 size)             = &local_malloc;
void (*fort_free)(void* ptr)               = &local_free;
void* (*fort_calloc)(s32 nmemb, s32 size)  = &local_calloc;
void* (*fort_realloc)(void* ptr, s32 size) = &local_realloc;
#endif

        static void* custom_fort_calloc(s32 nmemb, s32 size)
        {
            s32   total_size = nmemb * size;
            void* result     = F_MALLOC(total_size);
            if (result != NULL)
                memset(result, 0, total_size);
            return result;
        }

        static void* custom_fort_realloc(void* ptr, s32 size)
        {
            if (ptr == NULL)
                return F_MALLOC(size);
            if (size == 0)
            {
                F_FREE(ptr);
                return NULL;
            }

            void* new_chunk = F_MALLOC(size);
            if (new_chunk == NULL)
                return NULL;

            /*
             * In theory we should copy MIN(size, size allocated for ptr) bytes,
             * but this is rather dummy implementation so we don't care about it
             */
            memcpy(new_chunk, ptr, size);
            F_FREE(ptr);
            return new_chunk;
        }

        FT_INTERNAL
        void set_memory_funcs(void* (*f_malloc)(s32 size), void (*f_free)(void* ptr))
        {
            assert((f_malloc == NULL && f_free == NULL) /* Use std functions */
                   || (f_malloc != NULL && f_free != NULL) /* Use custom functions */);

            if (f_malloc == NULL && f_free == NULL)
            {
#if defined(FT_GCC_COMPILER) || defined(FT_CLANG_COMPILER)
                fort_malloc  = &malloc;
                fort_free    = &free;
                fort_calloc  = &calloc;
                fort_realloc = &realloc;
#else
        fort_malloc  = &local_malloc;
        fort_free    = &local_free;
        fort_calloc  = &local_calloc;
        fort_realloc = &local_realloc;
#endif
            }
            else
            {
                fort_malloc  = f_malloc;
                fort_free    = f_free;
                fort_calloc  = &custom_fort_calloc;
                fort_realloc = &custom_fort_realloc;
            }
        }

        FT_INTERNAL
        char* fort_strdup(const char* str)
        {
            if (str == NULL)
                return NULL;

            s32   sz       = strlen(str);
            char* str_copy = (char*)F_MALLOC((sz + 1) * sizeof(char));
            if (str_copy == NULL)
                return NULL;

            strcpy(str_copy, str);
            return str_copy;
        }

#if defined(FT_HAVE_WCHAR)
        FT_INTERNAL
        wchar_t* fort_wcsdup(const wchar_t* str)
        {
            if (str == NULL)
                return NULL;

            s32      sz       = wcslen(str);
            wchar_t* str_copy = (wchar_t*)F_MALLOC((sz + 1) * sizeof(wchar_t));
            if (str_copy == NULL)
                return NULL;

            wcscpy(str_copy, str);
            return str_copy;
        }
#endif

        static s32 columns_number_in_fmt_string(const char* fmt)
        {
            s32         separator_counter = 0;
            const char* pos               = fmt;
            while (1)
            {
                pos = strchr(pos, g_col_separator);
                if (pos == NULL)
                    break;

                separator_counter++;
                ++pos;
            }
            return separator_counter + 1;
        }

#if defined(FT_HAVE_WCHAR)
        static s32 columns_number_in_fmt_wstring(const wchar_t* fmt)
        {
            s32            separator_counter = 0;
            const wchar_t* pos               = fmt;
            while (1)
            {
                pos = wcschr(pos, g_col_separator);
                if (pos == NULL)
                    break;

                separator_counter++;
                ++pos;
            }
            return separator_counter + 1;
        }
#endif

#if defined(FT_HAVE_UTF8)
        static s32 columns_number_in_fmt_u8string(const void* fmt)
        {
            s32         separator_counter = 0;
            const char* pos               = (const char*)fmt;
            while (1)
            {
                pos = (const char*)utf8chr(pos, g_col_separator);
                if (pos == NULL)
                    break;

                separator_counter++;
                ++pos;
            }
            return separator_counter + 1;
        }
#endif

        FT_INTERNAL
        s32 number_of_columns_in_format_string(const f_string_view_t* fmt)
        {
            switch (fmt->type)
            {
                case CHAR_BUF: return columns_number_in_fmt_string(fmt->u.cstr);
#ifdef FT_HAVE_WCHAR
                case W_CHAR_BUF: return columns_number_in_fmt_wstring(fmt->u.wstr);
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
                case UTF8_BUF: return columns_number_in_fmt_u8string(fmt->u.u8str);
#endif /* FT_HAVE_UTF8 */
                default: assert(0);
            }
            return 0;
        }

        FT_INTERNAL
        s32 number_of_columns_in_format_buffer(const f_string_buffer_t* fmt)
        {
            switch (fmt->type)
            {
                case CHAR_BUF: return columns_number_in_fmt_string(fmt->str.cstr);
#ifdef FT_HAVE_WCHAR
                case W_CHAR_BUF: return columns_number_in_fmt_wstring(fmt->str.wstr);
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
                case UTF8_BUF: return columns_number_in_fmt_u8string(fmt->str.u8str);
#endif /* FT_HAVE_UTF8 */
                default: assert(0);
            }
            return 0;
        }

        static int snprint_n_strings_impl(char* buf, s32 length, s32 n, const char* str)
        {
            s32 str_len = strlen(str);
            if (length <= n * str_len)
                return -1;

            if (n == 0)
                return 0;

            /* To ensure valid return value it is safely not print such big strings */
            if (n * str_len > INT_MAX)
                return -1;

            if (str_len == 0)
                return 0;

            int status = snprintf(buf, length, "%0*d", (int)(n * str_len), 0);
            if (status < 0)
                return status;

            s32 i = 0;
            for (i = 0; i < n; ++i)
            {
                const char* str_p = str;
                while (*str_p)
                    *(buf++) = *(str_p++);
            }
            return (int)(n * str_len);
        }

        static int snprint_n_strings(f_conv_context_t* cntx, s32 n, const char* str)
        {
            int w = snprint_n_strings_impl(cntx->u.buf, cntx->raw_avail, n, str);
            if (w >= 0)
            {
                cntx->u.buf += w;
                cntx->raw_avail -= w;
            }
            return w;
        }

#if defined(FT_HAVE_WCHAR)
        static int wsnprint_n_string(wchar_t* buf, s32 length, s32 n, const char* str);
#endif

#if defined(FT_HAVE_UTF8)
        static int u8nprint_n_strings(void* buf, s32 length, s32 n, const void* str);
#endif

        FT_INTERNAL
        int print_n_strings(f_conv_context_t* cntx, s32 n, const char* str)
        {
            int cod_w;
            int raw_written;

            switch (cntx->b_type)
            {
                case CHAR_BUF:
                    raw_written = snprint_n_strings(cntx, n, str);
                    cod_w       = raw_written;
                    return cod_w;
#ifdef FT_HAVE_WCHAR
                case W_CHAR_BUF:
                    cod_w = wsnprint_n_string(cntx->u.wbuf, cntx->raw_avail, n, str);
                    if (cod_w < 0)
                        return cod_w;
                    raw_written = sizeof(wchar_t) * cod_w;

                    cntx->u.buf += raw_written;
                    cntx->raw_avail -= raw_written;
                    return cod_w;
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
                case UTF8_BUF:
                    /* Everying is very strange and differs with W_CHAR_BUF */
                    raw_written = u8nprint_n_strings(cntx->u.buf, cntx->raw_avail, n, str);
                    if (raw_written < 0)
                    {
                        fprintf(stderr, " raw_written = %d\n", raw_written);
                        return raw_written;
                    }

                    cntx->u.buf += raw_written;
                    cntx->raw_avail -= raw_written;
                    return utf8len(str) * n;
#endif /* FT_HAVE_UTF8 */
                default: assert(0); return -1;
            }
        }

        FT_INTERNAL
        int ft_nprint(f_conv_context_t* cntx, const char* str, s32 strlen)
        {
            if (cntx->raw_avail + 1 /* for 0 */ < strlen)
                return -1;

            memcpy(cntx->u.buf, str, strlen);
            cntx->u.buf += strlen;
            cntx->raw_avail -= strlen;
            *cntx->u.buf = '\0'; /* Do we need this ? */
            return strlen;
        }

#ifdef FT_HAVE_WCHAR
        int ft_nwprint(f_conv_context_t* cntx, const wchar_t* str, s32 strlen)
        {
            if (cntx->raw_avail + 1 /* for 0 */ < strlen)
                return -1;

            s32 raw_len = strlen * sizeof(wchar_t);

            memcpy(cntx->u.buf, str, raw_len);
            cntx->u.buf += raw_len;
            cntx->raw_avail -= raw_len;

            /* Do we need this ? */
            wchar_t end_of_string = L'\0';
            memcpy(cntx->u.buf, &end_of_string, sizeof(wchar_t));
            return strlen;
        }
#endif /* FT_HAVE_WCHAR */

#ifdef FT_HAVE_UTF8
        FT_INTERNAL
        int ft_nu8print(f_conv_context_t* cntx, const void* beg, const void* end)
        {
            const char* bc      = (const char*)beg;
            const char* ec      = (const char*)end;
            s32         raw_len = ec - bc;
            if (cntx->raw_avail + 1 < raw_len)
                return -1;

            memcpy(cntx->u.buf, beg, raw_len);
            cntx->u.buf += raw_len;
            cntx->raw_avail -= raw_len;
            *(cntx->u.buf) = '\0'; /* Do we need this ? */
            return raw_len;        /* what return here ? */
        }
#endif /* FT_HAVE_UTF8 */

#if defined(FT_HAVE_WCHAR)
#define WCS_SIZE 64

        static int wsnprint_n_string(wchar_t* buf, s32 length, s32 n, const char* str)
        {
            s32 str_len = strlen(str);

            /* note: maybe it's, better to return -1 in case of multibyte character
             * strings (not sure this case is done correctly).
             */
            if (str_len > 1)
            {
                const unsigned char* p = (const unsigned char*)str;
                while (*p)
                {
                    if (*p <= 127)
                        p++;
                    else
                    {
                        wchar_t     wcs[WCS_SIZE];
                        const char* ptr = str;
                        s32         wcs_len;
                        mbstate_t   mbst;
                        memset(&mbst, 0, sizeof(mbst));
                        wcs_len = mbsrtowcs(wcs, (const char**)&ptr, WCS_SIZE, &mbst);
                        /* for simplicity */
                        if ((wcs_len == (s32)-1) || wcs_len > 1)
                        {
                            return -1;
                        }
                        else
                        {
                            wcs[wcs_len] = L'\0';
                            s32 k        = n;
                            while (k)
                            {
                                *buf = *wcs;
                                ++buf;
                                --k;
                            }
                            buf[n] = L'\0';
                            return (int)n;
                        }
                    }
                }
            }

            if (length <= n * str_len)
                return -1;

            if (n == 0)
                return 0;

            /* To ensure valid return value it is safely not print such big strings */
            if (n * str_len > INT_MAX)
                return -1;

            if (str_len == 0)
                return 0;

            int status = swprintf(buf, length, L"%0*d", (int)(n * str_len), 0);
            if (status < 0)
                return status;

            s32 i = 0;
            for (i = 0; i < n; ++i)
            {
                const char* str_p = str;
                while (*str_p)
                    *(buf++) = (wchar_t) * (str_p++);
            }
            return (int)(n * str_len);
        }
#endif

#if defined(FT_HAVE_UTF8)
        static int u8nprint_n_strings(void* buf, s32 length, s32 n, const void* str)
        {
            s32 str_size = utf8size(str) - 1; /* str_size - raw size in bytes, excluding \0 */
            if (length <= n * str_size)
                return -1;

            if (n == 0)
                return 0;

            /* To ensure valid return value it is safely not print such big strings */
            if (n * str_size > INT_MAX)
                return -1;

            if (str_size == 0)
                return 0;

            s32 i = n;
            while (i)
            {
                memcpy(buf, str, str_size);
                buf = (char*)buf + str_size;
                --i;
            }
            *(char*)buf = '\0';
            return (int)(n * str_size);
        }
#endif

        /********************************************************
           End of file "fort_utils.c"
         ********************************************************/

        /********************************************************
           Begin of file "properties.c"
         ********************************************************/

#define FT_RESET_COLOR "\033[0m"

        static const char* fg_colors[] = {
            "", "\033[30m", "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m", "\033[37m", "\033[90m", "\033[91m", "\033[92m", "\033[93m", "\033[94m", "\033[95m", "\033[96m", "\033[97m",
        };

        static const char* bg_colors[] = {
            "", "\033[40m", "\033[41m", "\033[42m", "\033[43m", "\033[44m", "\033[45m", "\033[46m", "\033[47m", "\033[100m", "\033[101m", "\033[102m", "\033[103m", "\033[104m", "\033[105m", "\033[106m", "\033[107m",
        };

        static const char* text_styles[] = {
            "", "\033[1m", "\033[2m", "\033[3m", "\033[4m", "\033[5m", "\033[7m", "\033[8m",
        };

#define UNIVERSAL_RESET_TAG "\033[0m"

        static const s32 n_fg_colors = sizeof(fg_colors) / sizeof(fg_colors[0]);
        static const s32 n_bg_colors = sizeof(bg_colors) / sizeof(bg_colors[0]);
        static const s32 n_styles    = sizeof(text_styles) / sizeof(text_styles[0]);

        void get_style_tag_for_cell(const f_table_properties_t* props, s32 row, s32 col, char* style_tag, s32 sz)
        {
            (void)sz;
            s32 i = 0;

            unsigned bg_color_number = get_cell_property_hierarchically(props, row, col, FT_CPROP_CELL_BG_COLOR);
            unsigned text_style      = get_cell_property_hierarchically(props, row, col, FT_CPROP_CELL_TEXT_STYLE);

            style_tag[0] = '\0';

            if (text_style < (1U << n_styles))
            {
                for (i = 0; i < n_styles; ++i)
                {
                    if (text_style & (1 << i))
                    {
                        strcat(style_tag, text_styles[i]);
                    }
                }
            }
            else
            {
                goto error;
            }

            if (bg_color_number < n_bg_colors)
            {
                strcat(style_tag, bg_colors[bg_color_number]);
            }
            else
            {
                goto error;
            }

            return;

        error:
            /* shouldn't be here */
            assert(0);
            style_tag[0] = '\0';
            return;
        }

        void get_reset_style_tag_for_cell(const f_table_properties_t* props, s32 row, s32 col, char* reset_style_tag, s32 sz)
        {
            (void)sz;
            s32 i = 0;

            unsigned bg_color_number = get_cell_property_hierarchically(props, row, col, FT_CPROP_CELL_BG_COLOR);
            unsigned text_style      = get_cell_property_hierarchically(props, row, col, FT_CPROP_CELL_TEXT_STYLE);

            reset_style_tag[0] = '\0';

            if (text_style < (1U << n_styles))
            {
                for (i = 0; i < n_styles; ++i)
                {
                    if (text_style & (1 << i))
                    {
                        if (i != 0) // FT_TSTYLE_DEFAULT
                            goto reset_style;
                    }
                }
            }
            else
            {
                goto error;
            }

            if (bg_color_number < n_bg_colors)
            {
                if (bg_color_number)
                    goto reset_style;
            }
            else
            {
                goto error;
            }

            return;

        reset_style:
            strcat(reset_style_tag, UNIVERSAL_RESET_TAG);
            return;

        error:
            /* shouldn't be here */
            assert(0);
            reset_style_tag[0] = '\0';
            return;
        }

        void get_style_tag_for_content(const f_table_properties_t* props, s32 row, s32 col, char* style_tag, s32 sz)
        {
            (void)sz;
            s32 i = 0;

            unsigned text_style      = get_cell_property_hierarchically(props, row, col, FT_CPROP_CONT_TEXT_STYLE);
            unsigned fg_color_number = get_cell_property_hierarchically(props, row, col, FT_CPROP_CONT_FG_COLOR);
            unsigned bg_color_number = get_cell_property_hierarchically(props, row, col, FT_CPROP_CONT_BG_COLOR);

            style_tag[0] = '\0';

            if (text_style < (1U << n_styles))
            {
                for (i = 0; i < n_styles; ++i)
                {
                    if (text_style & (1 << i))
                    {
                        strcat(style_tag, text_styles[i]);
                    }
                }
            }
            else
            {
                goto error;
            }

            if (fg_color_number < n_fg_colors)
            {
                if (fg_color_number)
                    strcat(style_tag, fg_colors[fg_color_number]);
            }
            else
            {
                goto error;
            }

            if (bg_color_number < n_bg_colors)
            {
                strcat(style_tag, bg_colors[bg_color_number]);
            }
            else
            {
                goto error;
            }

            return;

        error:
            /* shouldn't be here */
            assert(0);
            style_tag[0] = '\0';
            return;
        }

        void get_reset_style_tag_for_content(const f_table_properties_t* props, s32 row, s32 col, char* reset_style_tag, s32 sz)
        {
            (void)sz;
            s32 i   = 0;
            s32 len = 0;

            unsigned text_style      = get_cell_property_hierarchically(props, row, col, FT_CPROP_CONT_TEXT_STYLE);
            unsigned fg_color_number = get_cell_property_hierarchically(props, row, col, FT_CPROP_CONT_FG_COLOR);
            unsigned bg_color_number = get_cell_property_hierarchically(props, row, col, FT_CPROP_CONT_BG_COLOR);

            reset_style_tag[0] = '\0';

            if (text_style < (1U << n_styles))
            {
                for (i = 0; i < n_styles; ++i)
                {
                    if (text_style & (1 << i))
                    {
                        if (i != 0) // FT_TSTYLE_DEFAULT
                            goto reset_style;
                    }
                }
            }
            else
            {
                goto error;
            }

            if (fg_color_number < n_fg_colors)
            {
                if (fg_color_number)
                    goto reset_style;
            }
            else
            {
                goto error;
            }

            if (bg_color_number < n_bg_colors)
            {
                if (bg_color_number)
                    goto reset_style;
            }
            else
            {
                goto error;
            }

            return;

        reset_style:
            strcat(reset_style_tag, UNIVERSAL_RESET_TAG);
            len = strlen(reset_style_tag);
            get_style_tag_for_cell(props, row, col, reset_style_tag + len, sz - len);
            return;

        error:
            /* shouldn't be here */
            assert(0);
            reset_style_tag[0] = '\0';
            return;
        }

        static struct f_cell_props g_default_cell_properties = {
            FT_ANY_ROW,    /* cell_row */
            FT_ANY_COLUMN, /* cell_col */

            /* properties_flags */
            FT_CPROP_MIN_WIDTH | FT_CPROP_TEXT_ALIGN | FT_CPROP_TOP_PADDING | FT_CPROP_BOTTOM_PADDING | FT_CPROP_LEFT_PADDING | FT_CPROP_RIGHT_PADDING | FT_CPROP_EMPTY_STR_HEIGHT | FT_CPROP_CONT_FG_COLOR | FT_CPROP_CELL_BG_COLOR | FT_CPROP_CONT_BG_COLOR |
                FT_CPROP_CELL_TEXT_STYLE | FT_CPROP_CONT_TEXT_STYLE,

            0,               /* col_min_width */
            FT_ALIGNED_LEFT, /* align */
            0,               /* cell_padding_top         */
            0,               /* cell_padding_bottom      */
            1,               /* cell_padding_left        */
            1,               /* cell_padding_right       */
            1,               /* cell_empty_string_height */

            FT_ROW_COMMON,     /* row_type */
            FT_COLOR_DEFAULT,  /* content_fg_color_number */
            FT_COLOR_DEFAULT,  /* content_bg_color_number */
            FT_COLOR_DEFAULT,  /* cell_bg_color_number */
            FT_TSTYLE_DEFAULT, /* cell_text_style */
            FT_TSTYLE_DEFAULT, /* content_text_style */
        };

        static int get_prop_value_if_exists_otherwise_default(const struct f_cell_props* cell_opts, uint32_t property)
        {
            if (cell_opts == NULL || !PROP_IS_SET(cell_opts->properties_flags, property))
            {
                cell_opts = &g_default_cell_properties;
            }

            switch (property)
            {
                case FT_CPROP_MIN_WIDTH: return cell_opts->col_min_width;
                case FT_CPROP_TEXT_ALIGN: return cell_opts->align;
                case FT_CPROP_TOP_PADDING: return cell_opts->cell_padding_top;
                case FT_CPROP_BOTTOM_PADDING: return cell_opts->cell_padding_bottom;
                case FT_CPROP_LEFT_PADDING: return cell_opts->cell_padding_left;
                case FT_CPROP_RIGHT_PADDING: return cell_opts->cell_padding_right;
                case FT_CPROP_EMPTY_STR_HEIGHT: return cell_opts->cell_empty_string_height;
                case FT_CPROP_ROW_TYPE: return cell_opts->row_type;
                case FT_CPROP_CONT_FG_COLOR: return cell_opts->content_fg_color_number;
                case FT_CPROP_CONT_BG_COLOR: return cell_opts->content_bg_color_number;
                case FT_CPROP_CELL_BG_COLOR: return cell_opts->cell_bg_color_number;
                case FT_CPROP_CELL_TEXT_STYLE: return cell_opts->cell_text_style;
                case FT_CPROP_CONT_TEXT_STYLE: return cell_opts->content_text_style;
                default:
                    /* todo: implement later */
                    exit(333);
            }
        }

        FT_INTERNAL
        f_cell_prop_container_t* create_cell_prop_container(void)
        {
            f_cell_prop_container_t* ret = create_vector(sizeof(f_cell_props_t), DEFAULT_VECTOR_CAPACITY);
            return ret;
        }

        FT_INTERNAL
        void destroy_cell_prop_container(f_cell_prop_container_t* cont)
        {
            if (cont)
                destroy_vector(cont);
        }

        FT_INTERNAL
        const f_cell_props_t* cget_cell_prop(const f_cell_prop_container_t* cont, s32 row, s32 col)
        {
            assert(cont);
            s32 sz = vector_size(cont);
            s32 i  = 0;
            for (i = 0; i < sz; ++i)
            {
                const f_cell_props_t* opt = &VECTOR_AT_C(cont, i, const f_cell_props_t);
                if (opt->cell_row == row && opt->cell_col == col)
                    return opt;
            }
            return NULL;
        }

        FT_INTERNAL
        f_cell_props_t* get_cell_prop_and_create_if_not_exists(f_cell_prop_container_t* cont, s32 row, s32 col)
        {
            assert(cont);
            s32 sz = vector_size(cont);
            s32 i  = 0;
            for (i = 0; i < sz; ++i)
            {
                f_cell_props_t* opt = &VECTOR_AT(cont, i, f_cell_props_t);
                if (opt->cell_row == row && opt->cell_col == col)
                    return opt;
            }

            f_cell_props_t opt;
            if (row == FT_ANY_ROW && col == FT_ANY_COLUMN)
                memcpy(&opt, &g_default_cell_properties, sizeof(f_cell_props_t));
            else
                memset(&opt, 0, sizeof(f_cell_props_t));

            opt.cell_row = row;
            opt.cell_col = col;
            if (FT_IS_SUCCESS(vector_push(cont, &opt)))
            {
                return &VECTOR_AT(cont, sz, f_cell_props_t);
            }

            return NULL;
        }

        FT_INTERNAL
        int get_cell_property_hierarchically(const f_table_properties_t* propertiess, s32 row, s32 column, uint32_t property)
        {
            assert(propertiess);
            s32 row_origin = row;

            const f_cell_props_t* opt = NULL;
            if (propertiess->cell_properties != NULL)
            {
                while (1)
                {
                    opt = cget_cell_prop(propertiess->cell_properties, row, column);
                    if (opt != NULL && PROP_IS_SET(opt->properties_flags, property))
                        break;

                    if (row != FT_ANY_ROW && column != FT_ANY_COLUMN)
                    {
                        row = FT_ANY_ROW;
                        continue;
                    }
                    else if (row == FT_ANY_ROW && column != FT_ANY_COLUMN)
                    {
                        row    = row_origin;
                        column = FT_ANY_COLUMN;
                        continue;
                    }
                    else if (row != FT_ANY_ROW && column == FT_ANY_COLUMN)
                    {
                        row    = FT_ANY_ROW;
                        column = FT_ANY_COLUMN;
                        continue;
                    }

                    opt = NULL;
                    break;
                }
            }

            return get_prop_value_if_exists_otherwise_default(opt, property);
        }

        static f_status set_cell_property_impl(f_cell_props_t* opt, uint32_t property, int value)
        {
            assert(opt);

            PROP_SET(opt->properties_flags, property);
            if (PROP_IS_SET(property, FT_CPROP_MIN_WIDTH))
            {
                CHECK_NOT_NEGATIVE(value);
                opt->col_min_width = value;
            }
            else if (PROP_IS_SET(property, FT_CPROP_TEXT_ALIGN))
            {
                opt->align = (enum ft_text_alignment)value;
            }
            else if (PROP_IS_SET(property, FT_CPROP_TOP_PADDING))
            {
                CHECK_NOT_NEGATIVE(value);
                opt->cell_padding_top = value;
            }
            else if (PROP_IS_SET(property, FT_CPROP_BOTTOM_PADDING))
            {
                CHECK_NOT_NEGATIVE(value);
                opt->cell_padding_bottom = value;
            }
            else if (PROP_IS_SET(property, FT_CPROP_LEFT_PADDING))
            {
                CHECK_NOT_NEGATIVE(value);
                opt->cell_padding_left = value;
            }
            else if (PROP_IS_SET(property, FT_CPROP_RIGHT_PADDING))
            {
                CHECK_NOT_NEGATIVE(value);
                opt->cell_padding_right = value;
            }
            else if (PROP_IS_SET(property, FT_CPROP_EMPTY_STR_HEIGHT))
            {
                CHECK_NOT_NEGATIVE(value);
                opt->cell_empty_string_height = value;
            }
            else if (PROP_IS_SET(property, FT_CPROP_ROW_TYPE))
            {
                opt->row_type = (enum ft_row_type)value;
            }
            else if (PROP_IS_SET(property, FT_CPROP_CONT_FG_COLOR))
            {
                opt->content_fg_color_number = value;
            }
            else if (PROP_IS_SET(property, FT_CPROP_CONT_BG_COLOR))
            {
                opt->content_bg_color_number = value;
            }
            else if (PROP_IS_SET(property, FT_CPROP_CELL_BG_COLOR))
            {
                opt->cell_bg_color_number = value;
            }
            else if (PROP_IS_SET(property, FT_CPROP_CELL_TEXT_STYLE))
            {
                enum ft_text_style v = (enum ft_text_style)value;
                if (v == FT_TSTYLE_DEFAULT)
                {
                    opt->cell_text_style = FT_TSTYLE_DEFAULT;
                }
                else
                {
                    opt->cell_text_style = (enum ft_text_style)(opt->cell_text_style | v);
                }
            }
            else if (PROP_IS_SET(property, FT_CPROP_CONT_TEXT_STYLE))
            {
                enum ft_text_style v = (enum ft_text_style)value;
                if (v == FT_TSTYLE_DEFAULT)
                {
                    opt->content_text_style = v;
                }
                else
                {
                    opt->content_text_style = (enum ft_text_style)(opt->content_text_style | v);
                }
            }

            return FT_SUCCESS;

        fort_fail:
            return FT_EINVAL;
        }

        FT_INTERNAL
        f_status set_cell_property(f_cell_prop_container_t* cont, s32 row, s32 col, uint32_t property, int value)
        {
            f_cell_props_t* opt = get_cell_prop_and_create_if_not_exists(cont, row, col);
            if (opt == NULL)
                return FT_GEN_ERROR;

            return set_cell_property_impl(opt, property, value);
            /*
            PROP_SET(opt->propertiess, property);
            if (PROP_IS_SET(property, FT_CPROP_MIN_WIDTH)) {
                opt->col_min_width = value;
            } else if (PROP_IS_SET(property, FT_CPROP_TEXT_ALIGN)) {
                opt->align = value;
            }

            return FT_SUCCESS;
            */
        }

        FT_INTERNAL
        f_status set_default_cell_property(uint32_t property, int value) { return set_cell_property_impl(&g_default_cell_properties, property, value); }

#define BASIC_STYLE                                                                                            \
    {                                                                                                          \
        /* border_chars */                                                                                     \
        {                                                                                                      \
            "+", "-", "+", "+", "|", "|", "|", "\0", "\0", "\0", "\0", "+", "-", "+", "+", "+", "+", "+", "+", \
        }, /* header_border_chars */                                                                           \
            {                                                                                                  \
                "+", "-", "+", "+", "|", "|", "|", "+", "-", "+", "+", "+", "-", "+", "+", "+", "+", "+", "+", \
            }, /* separator_chars */                                                                           \
            {                                                                                                  \
                "+", "-", "+", "+", "+", "+",                                                                  \
            },                                                                                                 \
    }

#define BASIC2_STYLE                                                                                           \
    {                                                                                                          \
        /* border_chars */                                                                                     \
        {                                                                                                      \
            "+", "-", "+", "+", "|", "|", "|", "+", "-", "+", "+", "+", "-", "+", "+", "+", "+", "+", "+",     \
        }, /* header_border_chars */                                                                           \
            {                                                                                                  \
                "+", "-", "+", "+", "|", "|", "|", "+", "-", "+", "+", "+", "-", "+", "+", "+", "+", "+", "+", \
            }, /* separator_chars */                                                                           \
            {                                                                                                  \
                "+", "-", "+", "+", "+", "+",                                                                  \
            },                                                                                                 \
    }

#define SIMPLE_STYLE                                                                                                         \
    {                                                                                                                        \
        /* border_chars */                                                                                                   \
        {                                                                                                                    \
            "\0", "\0", "\0", "\0", "\0", " ", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", \
        }, /* header_border_chars */                                                                                         \
            {                                                                                                                \
                "\0", "\0", "\0", "\0", "\0", " ", "\0", "\0", "-", " ", "\0", "\0", " ", " ", "\0", " ", "-", " ", "-",     \
            }, /* separator_chars */                                                                                         \
            {                                                                                                                \
                "\0", "-", " ", "\0", " ", " ",                                                                              \
            },                                                                                                               \
    }

#define PLAIN_STYLE                                                                                                          \
    {                                                                                                                        \
        /* border_chars */                                                                                                   \
        {                                                                                                                    \
            "\0", "\0", "\0", "\0", "\0", " ", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", \
        }, /* header_border_chars */                                                                                         \
            {                                                                                                                \
                "\0", "-", "-", "\0", "\0", " ", "\0", "\0", "-", "-", "\0", "\0", "-", "-", "\0", " ", "-", " ", "-",       \
            }, /* separator_chars */                                                                                         \
            {                                                                                                                \
                "\0", "-", "-", "\0", "-", "-",                                                                              \
            },                                                                                                               \
    }

#define DOT_STYLE                                                                                              \
    {                                                                                                          \
        /* border_chars */                                                                                     \
        {                                                                                                      \
            ".", ".", ".", ".", ":", ":", ":", "\0", "\0", "\0", "\0", ":", ".", ":", ":", "+", ":", "+", ":", \
        }, /* header_border_chars */                                                                           \
            {                                                                                                  \
                ".", ".", ".", ".", ":", ":", ":", ":", ".", ":", ":", ":", ".", ":", ":", "+", ".", "+", ".", \
            }, /* separator_chars */                                                                           \
            {                                                                                                  \
                ":", ".", ":", ":", ":", ":",                                                                  \
            },                                                                                                 \
    }

#define EMPTY_STYLE                                                                                                               \
    {                                                                                                                             \
        /* border_chars */                                                                                                        \
        {                                                                                                                         \
            "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",     \
        }, /* header_border_chars */                                                                                              \
            {                                                                                                                     \
                "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", \
            }, /* separator_chars */                                                                                              \
            {                                                                                                                     \
                "\0", " ", "\0 ", "\0", "\0", "\0",                                                                               \
            },                                                                                                                    \
    }

#define EMPTY2_STYLE                                                                                               \
    {                                                                                                              \
        /* border_chars */                                                                                         \
        {                                                                                                          \
            " ", " ", " ", " ", " ", " ", " ", "\0", "\0", "\0", "\0", " ", " ", " ", " ", " ", " ", " ", " ",     \
        }, /* header_border_chars */                                                                               \
            {                                                                                                      \
                " ", " ", " ", " ", " ", " ", " ", "\0", "\0", "\0", "\0", " ", " ", " ", " ", " ", " ", " ", " ", \
            }, /* separator_chars */                                                                               \
            {                                                                                                      \
                " ", " ", " ", " ", " ", " ",                                                                      \
            },                                                                                                     \
    }

#define SOLID_STYLE                                                                                            \
    {                                                                                                          \
        /* border_chars */                                                                                     \
        {                                                                                                      \
            "┌", "─", "┬", "┐", "│", "│", "│", "", "", "", "", "└", "─", "┴", "┘", "│", "─", "│", "─",         \
        }, /* header_border_chars */                                                                           \
            {                                                                                                  \
                "┌", "─", "┬", "┐", "│", "│", "│", "├", "─", "┼", "┤", "└", "─", "┴", "┘", "┼", "┬", "┼", "┴", \
            }, /* separator_chars */                                                                           \
            {                                                                                                  \
                "├", "─", "┼", "┤", "┬", "┴",                                                                  \
            },                                                                                                 \
    }

#define SOLID_ROUND_STYLE                                                                                      \
    {                                                                                                          \
        /* border_chars */                                                                                     \
        {                                                                                                      \
            "╭", "─", "┬", "╮", "│", "│", "│", "", "", "", "", "╰", "─", "┴", "╯", "│", "─", "│", "─",         \
        }, /* header_border_chars */                                                                           \
            {                                                                                                  \
                "╭", "─", "┬", "╮", "│", "│", "│", "├", "─", "┼", "┤", "╰", "─", "┴", "╯", "┼", "┬", "┼", "┴", \
            }, /* separator_chars */                                                                           \
            {                                                                                                  \
                "├", "─", "┼", "┤", "┬", "┴",                                                                  \
            },                                                                                                 \
    }

#define NICE_STYLE                                                                                             \
    {                                                                                                          \
        /* border_chars */                                                                                     \
        {                                                                                                      \
            "╔", "═", "╦", "╗", "║", "║", "║", "", "", "", "", "╚", "═", "╩", "╝", "┣", "┻", "┣", "┳",         \
        }, /* header_border_chars */                                                                           \
            {                                                                                                  \
                "╔", "═", "╦", "╗", "║", "║", "║", "╠", "═", "╬", "╣", "╚", "═", "╩", "╝", "┣", "╦", "┣", "╩", \
            }, /* separator_chars */                                                                           \
            {                                                                                                  \
                "╟", "─", "╫", "╢", "╥", "╨",                                                                  \
            },                                                                                                 \
    }

#define DOUBLE_STYLE                                                                                           \
    {                                                                                                          \
        /* border_chars */                                                                                     \
        {                                                                                                      \
            "╔", "═", "╦", "╗", "║", "║", "║", "", "", "", "", "╚", "═", "╩", "╝", "┣", "┻", "┣", "┳",         \
        }, /* header_border_chars */                                                                           \
            {                                                                                                  \
                "╔", "═", "╦", "╗", "║", "║", "║", "╠", "═", "╬", "╣", "╚", "═", "╩", "╝", "┣", "╦", "┣", "╩", \
            }, /* separator_chars */                                                                           \
            {                                                                                                  \
                "╠", "═", "╬", "╣", "╦", "╩",                                                                  \
            },                                                                                                 \
    }

#define DOUBLE2_STYLE                                                                                          \
    {                                                                                                          \
        /* border_chars */                                                                                     \
        {                                                                                                      \
            "╔", "═", "╤", "╗", "║", "│", "║", "╟", "─", "┼", "╢", "╚", "═", "╧", "╝", "├", "┬", "┤", "┴",     \
        }, /* header_border_chars */                                                                           \
            {                                                                                                  \
                "╔", "═", "╤", "╗", "║", "│", "║", "╠", "═", "╪", "╣", "╚", "═", "╧", "╝", "├", "╤", "┤", "╧", \
            }, /* separator_chars */                                                                           \
            {                                                                                                  \
                "╠", "═", "╪", "╣", "╤", "╧",                                                                  \
            },                                                                                                 \
    }

#define BOLD_STYLE                                                                                             \
    {                                                                                                          \
        /* border_chars */                                                                                     \
        {                                                                                                      \
            "┏", "━", "┳", "┓", "┃", "┃", "┃", "", "", "", "", "┗", "━", "┻", "┛", "┣", "┻", "┣", "┳",         \
        }, /* header_border_chars */                                                                           \
            {                                                                                                  \
                "┏", "━", "┳", "┓", "┃", "┃", "┃", "┣", "━", "╋", "┫", "┗", "━", "┻", "┛", "┣", "┳", "┣", "┻", \
            }, /* separator_chars */                                                                           \
            {                                                                                                  \
                "┣", "━", "╋", "┫", "┳", "┻",                                                                  \
            },                                                                                                 \
    }

#define BOLD2_STYLE                                                                                            \
    {                                                                                                          \
        /* border_chars */                                                                                     \
        {                                                                                                      \
            "┏", "━", "┯", "┓", "┃", "│", "┃", "┠", "─", "┼", "┨", "┗", "━", "┷", "┛", "┣", "┬", "┣", "┴",     \
        }, /* header_border_chars */                                                                           \
            {                                                                                                  \
                "┏", "━", "┯", "┓", "┃", "│", "┃", "┣", "━", "┿", "┫", "┗", "━", "┷", "┛", "┣", "┯", "┣", "┷", \
            }, /* separator_chars */                                                                           \
            {                                                                                                  \
                "┣", "━", "┿", "┫", "┯", "┷",                                                                  \
            },                                                                                                 \
    }

#define FRAME_STYLE                                                                                                            \
    {                                                                                                                          \
        /* border_chars */                                                                                                     \
        {"▛", "▀", "▀", "▜", "▌", "┃", "▐", "", "", "", "", "▙", "▄", "▄", "▟", "┣", "━", "┣", "━"}, /* header_border_chars */ \
            {                                                                                                                  \
                "▛", "▀", "▀", "▜", "▌", "┃", "▐", "▌", "━", "╋", "▐", "▙", "▄", "▄", "▟", "┣", "━", "┣", "━",                 \
            }, /* separator_chars */                                                                                           \
            {                                                                                                                  \
                "▌", "━", "╋", "▐", "╋", "╋",                                                                                  \
            },                                                                                                                 \
    }

        struct fort_border_style FORT_BASIC_STYLE       = BASIC_STYLE;
        struct fort_border_style FORT_BASIC2_STYLE      = BASIC2_STYLE;
        struct fort_border_style FORT_SIMPLE_STYLE      = SIMPLE_STYLE;
        struct fort_border_style FORT_PLAIN_STYLE       = PLAIN_STYLE;
        struct fort_border_style FORT_DOT_STYLE         = DOT_STYLE;
        struct fort_border_style FORT_EMPTY_STYLE       = EMPTY_STYLE;
        struct fort_border_style FORT_EMPTY2_STYLE      = EMPTY2_STYLE;
        struct fort_border_style FORT_SOLID_STYLE       = SOLID_STYLE;
        struct fort_border_style FORT_SOLID_ROUND_STYLE = SOLID_ROUND_STYLE;
        struct fort_border_style FORT_NICE_STYLE        = NICE_STYLE;
        struct fort_border_style FORT_DOUBLE_STYLE      = DOUBLE_STYLE;
        struct fort_border_style FORT_DOUBLE2_STYLE     = DOUBLE2_STYLE;
        struct fort_border_style FORT_BOLD_STYLE        = BOLD_STYLE;
        struct fort_border_style FORT_BOLD2_STYLE       = BOLD2_STYLE;
        struct fort_border_style FORT_FRAME_STYLE       = FRAME_STYLE;

        fort_entire_table_properties_t g_entire_table_properties = {
            0,                   /* left_margin */
            0,                   /* top_margin */
            0,                   /* right_margin */
            0,                   /* bottom_margin */
            FT_STRATEGY_REPLACE, /* add_strategy */
        };

        static f_status set_entire_table_property_internal(fort_entire_table_properties_t* properties, uint32_t property, int value)
        {
            assert(properties);
            CHECK_NOT_NEGATIVE(value);
            if (PROP_IS_SET(property, FT_TPROP_LEFT_MARGIN))
            {
                properties->left_margin = value;
            }
            else if (PROP_IS_SET(property, FT_TPROP_TOP_MARGIN))
            {
                properties->top_margin = value;
            }
            else if (PROP_IS_SET(property, FT_TPROP_RIGHT_MARGIN))
            {
                properties->right_margin = value;
            }
            else if (PROP_IS_SET(property, FT_TPROP_BOTTOM_MARGIN))
            {
                properties->bottom_margin = value;
            }
            else if (PROP_IS_SET(property, FT_TPROP_ADDING_STRATEGY))
            {
                properties->add_strategy = (enum ft_adding_strategy)value;
            }
            else
            {
                return FT_EINVAL;
            }
            return FT_SUCCESS;

        fort_fail:
            return FT_EINVAL;
        }

        FT_INTERNAL
        f_status set_entire_table_property(f_table_properties_t* table_properties, uint32_t property, int value)
        {
            assert(table_properties);
            return set_entire_table_property_internal(&table_properties->entire_table_properties, property, value);
        }

        FT_INTERNAL
        f_status set_default_entire_table_property(uint32_t property, int value) { return set_entire_table_property_internal(&g_entire_table_properties, property, value); }

        FT_INTERNAL
        s32 max_border_elem_strlen(struct f_table_properties* properties)
        {
            assert(properties);
            s32 result = 1;
            int i      = 0;
            for (i = 0; i < BORDER_ITEM_POS_SIZE; ++i)
            {
                result = MAX(result, strlen(properties->border_style.border_chars[i]));
            }

            for (i = 0; i < BORDER_ITEM_POS_SIZE; ++i)
            {
                result = MAX(result, strlen(properties->border_style.header_border_chars[i]));
            }

            for (i = 0; i < SEPARATOR_ITEM_POS_SIZE; ++i)
            {
                result = MAX(result, strlen(properties->border_style.separator_chars[i]));
            }
            return result;
        }

        f_table_properties_t g_table_properties = {
            /* border_style */
            BASIC_STYLE,
            NULL, /* cell_properties */
            /* entire_table_properties */
            {
                0,                   /* left_margin */
                0,                   /* top_margin */
                0,                   /* right_margin */
                0,                   /* bottom_margin */
                FT_STRATEGY_REPLACE, /* add_strategy */
            }};

        FT_INTERNAL
        f_table_properties_t* create_table_properties(void)
        {
            f_table_properties_t* properties = (f_table_properties_t*)F_CALLOC(sizeof(f_table_properties_t), 1);
            if (properties == NULL)
            {
                return NULL;
            }
            memcpy(properties, &g_table_properties, sizeof(f_table_properties_t));
            properties->cell_properties = create_cell_prop_container();
            if (properties->cell_properties == NULL)
            {
                destroy_table_properties(properties);
                return NULL;
            }
            memcpy(&properties->entire_table_properties, &g_entire_table_properties, sizeof(fort_entire_table_properties_t));
            return properties;
        }

        FT_INTERNAL
        void destroy_table_properties(f_table_properties_t* properties)
        {
            if (properties == NULL)
                return;

            if (properties->cell_properties != NULL)
            {
                destroy_cell_prop_container(properties->cell_properties);
            }
            F_FREE(properties);
        }

        static f_cell_prop_container_t* copy_cell_properties(f_cell_prop_container_t* cont)
        {
            f_cell_prop_container_t* result = create_cell_prop_container();
            if (result == NULL)
                return NULL;

            s32 i  = 0;
            s32 sz = vector_size(cont);
            for (i = 0; i < sz; ++i)
            {
                f_cell_props_t* opt = (f_cell_props_t*)vector_at(cont, i);
                if (FT_IS_ERROR(vector_push(result, opt)))
                {
                    destroy_cell_prop_container(result);
                    return NULL;
                }
            }
            return result;
        }

        FT_INTERNAL
        f_table_properties_t* copy_table_properties(const f_table_properties_t* properties)
        {
            f_table_properties_t* new_opt = create_table_properties();
            if (new_opt == NULL)
                return NULL;

            destroy_vector(new_opt->cell_properties);
            new_opt->cell_properties = copy_cell_properties(properties->cell_properties);
            if (new_opt->cell_properties == NULL)
            {
                destroy_table_properties(new_opt);
                return NULL;
            }

            memcpy(&new_opt->border_style, &properties->border_style, sizeof(struct fort_border_style));
            memcpy(&new_opt->entire_table_properties, &properties->entire_table_properties, sizeof(fort_entire_table_properties_t));

            return new_opt;
        }

        /********************************************************
           End of file "properties.c"
         ********************************************************/

        /********************************************************
           Begin of file "row.c"
         ********************************************************/

        struct f_row
        {
            f_vector_t* cells;
        };

        static f_row_t* create_row_impl(f_vector_t* cells)
        {
            f_row_t* row = (f_row_t*)F_CALLOC(1, sizeof(f_row_t));
            if (row == NULL)
                return NULL;
            if (cells)
            {
                row->cells = cells;
            }
            else
            {
                row->cells = create_vector(sizeof(f_cell_t*), DEFAULT_VECTOR_CAPACITY);
                if (row->cells == NULL)
                {
                    F_FREE(row);
                    return NULL;
                }
            }
            return row;
        }

        FT_INTERNAL
        f_row_t* create_row(void) { return create_row_impl(NULL); }

        static void destroy_each_cell(f_vector_t* cells)
        {
            s32 i       = 0;
            s32 cells_n = vector_size(cells);
            for (i = 0; i < cells_n; ++i)
            {
                f_cell_t* cell = VECTOR_AT(cells, i, f_cell_t*);
                destroy_cell(cell);
            }
        }

        FT_INTERNAL
        void destroy_row(f_row_t* row)
        {
            if (row == NULL)
                return;

            if (row->cells)
            {
                destroy_each_cell(row->cells);
                destroy_vector(row->cells);
            }

            F_FREE(row);
        }

        FT_INTERNAL
        f_row_t* copy_row(f_row_t* row)
        {
            assert(row);
            f_row_t* result = create_row();
            if (result == NULL)
                return NULL;

            s32 i      = 0;
            s32 cols_n = vector_size(row->cells);
            for (i = 0; i < cols_n; ++i)
            {
                f_cell_t* cell     = VECTOR_AT(row->cells, i, f_cell_t*);
                f_cell_t* new_cell = copy_cell(cell);
                if (new_cell == NULL)
                {
                    destroy_row(result);
                    return NULL;
                }
                vector_push(result->cells, &new_cell);
            }

            return result;
        }

        FT_INTERNAL
        f_row_t* split_row(f_row_t* row, s32 pos)
        {
            assert(row);

            f_vector_t* cells = vector_split(row->cells, pos);
            if (!cells)
                return NULL;
            f_row_t* tail = create_row_impl(cells);
            if (!tail)
            {
                destroy_each_cell(cells);
                destroy_vector(cells);
            }
            return tail;
        }

        FT_INTERNAL
        int ft_row_erase_range(f_row_t* row, s32 left, s32 right)
        {
            assert(row);
            s32 cols_n = vector_size(row->cells);
            if (cols_n == 0 || (right < left))
                return FT_SUCCESS;

            f_cell_t* cell = NULL;
            s32       i    = left;
            while (i < cols_n && i <= right)
            {
                cell = VECTOR_AT(row->cells, i, f_cell_t*);
                destroy_cell(cell);
                ++i;
            }
            s32 n_destroy = MIN(cols_n - 1, right) - left + 1;
            while (n_destroy--)
            {
                vector_erase(row->cells, left);
            }
            return FT_SUCCESS;
        }

        FT_INTERNAL
        s32 columns_in_row(const f_row_t* row)
        {
            if (row == NULL || row->cells == NULL)
                return 0;

            return vector_size(row->cells);
        }

        static f_cell_t* get_cell_impl(f_row_t* row, s32 col, enum f_get_policy policy)
        {
            if (row == NULL || row->cells == NULL)
            {
                return NULL;
            }

            switch (policy)
            {
                case DONT_CREATE_ON_NULL:
                    if (col < columns_in_row(row))
                    {
                        return VECTOR_AT(row->cells, col, f_cell_t*);
                    }
                    return NULL;
                case CREATE_ON_NULL:
                    while (col >= columns_in_row(row))
                    {
                        f_cell_t* new_cell = create_cell();
                        if (new_cell == NULL)
                            return NULL;
                        if (FT_IS_ERROR(vector_push(row->cells, &new_cell)))
                        {
                            destroy_cell(new_cell);
                            return NULL;
                        }
                    }
                    return VECTOR_AT(row->cells, col, f_cell_t*);
            }

            assert(0 && "Shouldn't be here!");
            return NULL;
        }

        FT_INTERNAL
        f_cell_t* get_cell(f_row_t* row, s32 col) { return get_cell_impl(row, col, DONT_CREATE_ON_NULL); }

        FT_INTERNAL
        const f_cell_t* get_cell_c(const f_row_t* row, s32 col) { return get_cell((f_row_t*)row, col); }

        FT_INTERNAL
        f_cell_t* get_cell_and_create_if_not_exists(f_row_t* row, s32 col) { return get_cell_impl(row, col, CREATE_ON_NULL); }

        FT_INTERNAL
        f_cell_t* create_cell_in_position(f_row_t* row, s32 col)
        {
            if (row == NULL || row->cells == NULL)
            {
                return NULL;
            }

            f_cell_t* new_cell = create_cell();
            if (new_cell == NULL)
                return NULL;
            if (FT_IS_ERROR(vector_insert(row->cells, &new_cell, col)))
            {
                destroy_cell(new_cell);
                return NULL;
            }
            return VECTOR_AT(row->cells, col, f_cell_t*);
        }

        FT_INTERNAL
        f_status swap_row(f_row_t* cur_row, f_row_t* ins_row, s32 pos)
        {
            assert(cur_row);
            assert(ins_row);
            s32 cur_sz = vector_size(cur_row->cells);
            if (cur_sz == 0 && pos == 0)
            {
                f_row_t tmp;
                memcpy(&tmp, cur_row, sizeof(f_row_t));
                memcpy(cur_row, ins_row, sizeof(f_row_t));
                memcpy(ins_row, &tmp, sizeof(f_row_t));
                return FT_SUCCESS;
            }

            // Append empty cells to `cur_row` if needed.
            while (vector_size(cur_row->cells) < pos)
            {
                create_cell_in_position(cur_row, vector_size(cur_row->cells));
            }

            return vector_swap(cur_row->cells, ins_row->cells, pos);
        }

        /* Ownership of cells of `ins_row` is passed to `cur_row`. */
        FT_INTERNAL
        f_status insert_row(f_row_t* cur_row, f_row_t* ins_row, s32 pos)
        {
            assert(cur_row);
            assert(ins_row);

            while (vector_size(cur_row->cells) < pos)
            {
                f_cell_t* new_cell = create_cell();
                if (!new_cell)
                    return FT_GEN_ERROR;
                vector_push(cur_row->cells, &new_cell);
            }

            s32 sz = vector_size(ins_row->cells);
            s32 i  = 0;
            for (i = 0; i < sz; ++i)
            {
                f_cell_t* cell = VECTOR_AT(ins_row->cells, i, f_cell_t*);
                if (FT_IS_ERROR(vector_insert(cur_row->cells, &cell, pos + i)))
                {
                    /* clean up what we have inserted */
                    while (i--)
                    {
                        vector_erase(cur_row->cells, pos);
                    }
                    return FT_GEN_ERROR;
                }
            }
            /* Clear cells so that it will be safe to destroy this row */
            vector_clear(ins_row->cells);
            return FT_SUCCESS;
        }

        FT_INTERNAL
        s32 group_cell_number(const f_row_t* row, s32 master_cell_col)
        {
            assert(row);
            const f_cell_t* master_cell = get_cell_c(row, master_cell_col);
            if (master_cell == NULL)
                return 0;

            if (get_cell_type(master_cell) != GROUP_MASTER_CELL)
                return 1;

            s32 total_cols = vector_size(row->cells);
            s32 slave_col  = master_cell_col + 1;
            while (slave_col < total_cols)
            {
                const f_cell_t* cell = get_cell_c(row, slave_col);
                if (cell && get_cell_type(cell) == GROUP_SLAVE_CELL)
                {
                    ++slave_col;
                }
                else
                {
                    break;
                }
            }
            return slave_col - master_cell_col;
        }

        FT_INTERNAL
        int get_row_cell_types(const f_row_t* row, enum f_cell_type* types, s32 types_sz)
        {
            assert(row);
            assert(types);
            s32 i = 0;
            for (i = 0; i < types_sz; ++i)
            {
                const f_cell_t* cell = get_cell_c(row, i);
                if (cell)
                {
                    types[i] = get_cell_type(cell);
                }
                else
                {
                    types[i] = COMMON_CELL;
                }
            }
            return FT_SUCCESS;
        }

        FT_INTERNAL
        f_status row_set_cell_span(f_row_t* row, s32 cell_column, s32 hor_span)
        {
            assert(row);

            if (hor_span < 2)
                return FT_EINVAL;

            f_cell_t* main_cell = get_cell_and_create_if_not_exists(row, cell_column);
            if (main_cell == NULL)
            {
                return FT_GEN_ERROR;
            }
            set_cell_type(main_cell, GROUP_MASTER_CELL);
            --hor_span;
            ++cell_column;

            while (hor_span)
            {
                f_cell_t* slave_cell = get_cell_and_create_if_not_exists(row, cell_column);
                if (slave_cell == NULL)
                {
                    return FT_GEN_ERROR;
                }
                set_cell_type(slave_cell, GROUP_SLAVE_CELL);
                --hor_span;
                ++cell_column;
            }

            return FT_SUCCESS;
        }

        static int print_row_separator_impl(f_conv_context_t* cntx, const s32* col_width_arr, s32 cols, const f_row_t* upper_row, const f_row_t* lower_row, enum f_hor_separator_pos separatorPos, const f_separator_t* sep)
        {
            assert(cntx);

            int status = FT_GEN_ERROR;

            const f_context_t* context = cntx->cntx;

            /* Get cell types
             *
             * Regions above top row and below bottom row areconsidered full of virtual
             * GROUP_SLAVE_CELL cells
             */
            enum f_cell_type* top_row_types = (enum f_cell_type*)F_MALLOC(sizeof(enum f_cell_type) * cols * 2);
            if (top_row_types == NULL)
            {
                return FT_MEMORY_ERROR;
            }
            enum f_cell_type* bottom_row_types = top_row_types + cols;
            if (upper_row)
            {
                get_row_cell_types(upper_row, top_row_types, cols);
            }
            else
            {
                s32 i = 0;
                for (i = 0; i < cols; ++i)
                    top_row_types[i] = GROUP_SLAVE_CELL;
            }
            if (lower_row)
            {
                get_row_cell_types(lower_row, bottom_row_types, cols);
            }
            else
            {
                s32 i = 0;
                for (i = 0; i < cols; ++i)
                    bottom_row_types[i] = GROUP_SLAVE_CELL;
            }

            f_table_properties_t*           properties    = context->table_properties;
            fort_entire_table_properties_t* entire_tprops = &properties->entire_table_properties;

            s32 written = 0;
            int tmp     = 0;

            enum ft_row_type lower_row_type = FT_ROW_COMMON;
            if (lower_row != NULL)
            {
                lower_row_type = (enum ft_row_type)get_cell_property_hierarchically(properties, context->row, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE);
            }
            enum ft_row_type upper_row_type = FT_ROW_COMMON;
            if (upper_row != NULL)
            {
                upper_row_type = (enum ft_row_type)get_cell_property_hierarchically(properties, context->row - 1, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE);
            }

            /* Row separator anatomy
             *
             *  |      C11    |   C12         C13      |      C14           C15         |
             *  L  I  I  I   IV  I   I   IT  I  I  I  IB    I    I     II    I    I     R
             *  |      C21    |   C22     |   C23             C24           C25         |
             */
            const char** L  = NULL;
            const char** I  = NULL;
            const char** IV = NULL;
            const char** R  = NULL;
            const char** IT = NULL;
            const char** IB = NULL;
            const char** II = NULL;

            struct fort_border_style* border_style = &properties->border_style;

            typedef const char*(*border_chars_point_t)[BORDER_ITEM_POS_SIZE];
            const char*(*border_chars)[BORDER_ITEM_POS_SIZE] = NULL;
            border_chars                                     = (border_chars_point_t)&border_style->border_chars;
            if (upper_row_type == FT_ROW_HEADER || lower_row_type == FT_ROW_HEADER)
            {
                border_chars = (border_chars_point_t)&border_style->header_border_chars;
            }

            if (sep && sep->enabled)
            {
                L  = &(border_style->separator_chars[LH_sip]);
                I  = &(border_style->separator_chars[IH_sip]);
                IV = &(border_style->separator_chars[II_sip]);
                R  = &(border_style->separator_chars[RH_sip]);

                IT = &(border_style->separator_chars[TI_sip]);
                IB = &(border_style->separator_chars[BI_sip]);
                II = &(border_style->separator_chars[IH_sip]);

                if (lower_row == NULL)
                {
                    L = &(*border_chars)[BL_bip];
                    R = &(*border_chars)[BR_bip];
                }
                else if (upper_row == NULL)
                {
                    L = &(*border_chars)[TL_bip];
                    R = &(*border_chars)[TR_bip];
                }
            }
            else
            {
                switch (separatorPos)
                {
                    case TOP_SEPARATOR:
                        L  = &(*border_chars)[TL_bip];
                        I  = &(*border_chars)[TT_bip];
                        IV = &(*border_chars)[TV_bip];
                        R  = &(*border_chars)[TR_bip];

                        IT = &(*border_chars)[TV_bip];
                        IB = &(*border_chars)[TV_bip];
                        II = &(*border_chars)[TT_bip];
                        break;
                    case INSIDE_SEPARATOR:
                        L  = &(*border_chars)[LH_bip];
                        I  = &(*border_chars)[IH_bip];
                        IV = &(*border_chars)[II_bip];
                        R  = &(*border_chars)[RH_bip];

                        IT = &(*border_chars)[TI_bip];
                        IB = &(*border_chars)[BI_bip];
                        II = &(*border_chars)[IH_bip];
                        break;
                    case BOTTOM_SEPARATOR:
                        L  = &(*border_chars)[BL_bip];
                        I  = &(*border_chars)[BB_bip];
                        IV = &(*border_chars)[BV_bip];
                        R  = &(*border_chars)[BR_bip];

                        IT = &(*border_chars)[BV_bip];
                        IB = &(*border_chars)[BV_bip];
                        II = &(*border_chars)[BB_bip];
                        break;
                    default: break;
                }
            }

            s32 i = 0;

            /* If all chars are not printable, skip line separator */
            /* NOTE: argument of `isprint` should be explicitly converted to
             * unsigned char according to
             * https://en.cppreference.com/w/c/string/byte/isprint
             */
            if ((strlen(*L) == 0 || (strlen(*L) == 1 && !isprint((unsigned char)**L))) && (strlen(*I) == 0 || (strlen(*I) == 1 && !isprint((unsigned char)**I))) && (strlen(*IV) == 0 || (strlen(*IV) == 1 && !isprint((unsigned char)**IV))) &&
                (strlen(*R) == 0 || (strlen(*R) == 1 && !isprint((unsigned char)**R))))
            {
                status = 0;
                goto clear;
            }

            /* Print left margin */
            CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, entire_tprops->left_margin, FT_SPACE));

            for (i = 0; i < cols; ++i)
            {
                if (i == 0)
                {
                    CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *L));
                }
                else
                {
                    if ((top_row_types[i] == COMMON_CELL || top_row_types[i] == GROUP_MASTER_CELL) && (bottom_row_types[i] == COMMON_CELL || bottom_row_types[i] == GROUP_MASTER_CELL))
                    {
                        CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *IV));
                    }
                    else if (top_row_types[i] == GROUP_SLAVE_CELL && bottom_row_types[i] == GROUP_SLAVE_CELL)
                    {
                        CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *II));
                    }
                    else if (top_row_types[i] == GROUP_SLAVE_CELL)
                    {
                        CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *IT));
                    }
                    else
                    {
                        CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *IB));
                    }
                }
                CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, col_width_arr[i], *I));
            }
            CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *R));

            /* Print right margin */
            CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, entire_tprops->right_margin, FT_SPACE));

            CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, FT_NEWLINE));

            status = (int)written;

        clear:
            F_FREE(top_row_types);
            return status;
        }

        FT_INTERNAL
        int print_row_separator(f_conv_context_t* cntx, const s32* col_width_arr, s32 cols, const f_row_t* upper_row, const f_row_t* lower_row, enum f_hor_separator_pos separatorPos, const f_separator_t* sep)
        {
            return print_row_separator_impl(cntx, col_width_arr, cols, upper_row, lower_row, separatorPos, sep);
        }

        FT_INTERNAL
        f_row_t* create_row_from_string(const char* str)
        {
            typedef char char_type;
            char_type* (*strdup_)(const char_type* str)                         = F_STRDUP;
            const char_type zero_char                                           = '\0';
            f_status (*fill_cell_from_string_)(f_cell_t* cell, const char* str) = fill_cell_from_string;
            const char_type* const zero_string                                  = "";
#define STRCHR strchr

            char_type* pos                  = NULL;
            char_type* base_pos             = NULL;
            s32        number_of_separators = 0;

            f_row_t* row = create_row();
            if (row == NULL)
                return NULL;

            if (str == NULL)
                return row;

            char_type* str_copy = strdup_(str);
            if (str_copy == NULL)
                goto clear;

            pos                  = str_copy;
            base_pos             = str_copy;
            number_of_separators = 0;
            while (*pos)
            {
                pos = STRCHR(pos, g_col_separator);
                if (pos != NULL)
                {
                    *(pos) = zero_char;
                    ++pos;
                    number_of_separators++;
                }

                f_cell_t* cell = create_cell();
                if (cell == NULL)
                    goto clear;

                int status = fill_cell_from_string_(cell, base_pos);
                if (FT_IS_ERROR(status))
                {
                    destroy_cell(cell);
                    goto clear;
                }

                status = vector_push(row->cells, &cell);
                if (FT_IS_ERROR(status))
                {
                    destroy_cell(cell);
                    goto clear;
                }

                if (pos == NULL)
                    break;
                base_pos = pos;
            }

            /* special case if in format string last cell is empty */
            while (vector_size(row->cells) < (number_of_separators + 1))
            {
                f_cell_t* cell = create_cell();
                if (cell == NULL)
                    goto clear;

                int status = fill_cell_from_string_(cell, zero_string);
                if (FT_IS_ERROR(status))
                {
                    destroy_cell(cell);
                    goto clear;
                }

                status = vector_push(row->cells, &cell);
                if (FT_IS_ERROR(status))
                {
                    destroy_cell(cell);
                    goto clear;
                }
            }

            F_FREE(str_copy);
            return row;

        clear:
            destroy_row(row);
            F_FREE(str_copy);
            return NULL;

#undef STRCHR
        }

#ifdef FT_HAVE_WCHAR
        FT_INTERNAL
        f_row_t* create_row_from_wstring(const wchar_t* str)
        {
            typedef wchar_t char_type;
            char_type* (*strdup_)(const char_type* str)                            = F_WCSDUP;
            const char_type zero_char                                              = L'\0';
            f_status (*fill_cell_from_string_)(f_cell_t* cell, const wchar_t* str) = fill_cell_from_wstring;
            const char_type* const zero_string                                     = L"";
#define STRCHR wcschr

            char_type* pos                  = NULL;
            char_type* base_pos             = NULL;
            s32        number_of_separators = 0;

            f_row_t* row = create_row();
            if (row == NULL)
                return NULL;

            if (str == NULL)
                return row;

            char_type* str_copy = strdup_(str);
            if (str_copy == NULL)
                goto clear;

            pos                  = str_copy;
            base_pos             = str_copy;
            number_of_separators = 0;
            while (*pos)
            {
                pos = STRCHR(pos, g_col_separator);
                if (pos != NULL)
                {
                    *(pos) = zero_char;
                    ++pos;
                    number_of_separators++;
                }

                f_cell_t* cell = create_cell();
                if (cell == NULL)
                    goto clear;

                int status = fill_cell_from_string_(cell, base_pos);
                if (FT_IS_ERROR(status))
                {
                    destroy_cell(cell);
                    goto clear;
                }

                status = vector_push(row->cells, &cell);
                if (FT_IS_ERROR(status))
                {
                    destroy_cell(cell);
                    goto clear;
                }

                if (pos == NULL)
                    break;
                base_pos = pos;
            }

            /* special case if in format string last cell is empty */
            while (vector_size(row->cells) < (number_of_separators + 1))
            {
                f_cell_t* cell = create_cell();
                if (cell == NULL)
                    goto clear;

                int status = fill_cell_from_string_(cell, zero_string);
                if (FT_IS_ERROR(status))
                {
                    destroy_cell(cell);
                    goto clear;
                }

                status = vector_push(row->cells, &cell);
                if (FT_IS_ERROR(status))
                {
                    destroy_cell(cell);
                    goto clear;
                }
            }

            F_FREE(str_copy);
            return row;

        clear:
            destroy_row(row);
            F_FREE(str_copy);
            return NULL;
#undef STRCHR
        }
#endif

        FT_INTERNAL
        f_row_t* create_row_from_buffer(const f_string_buffer_t* buffer)
        {
            switch (buffer->type)
            {
                case CHAR_BUF: return create_row_from_string(buffer->str.cstr);
#ifdef FT_HAVE_WCHAR
                case W_CHAR_BUF: return create_row_from_wstring(buffer->str.wstr);
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
                case UTF8_BUF: return create_row_from_string((const char*)buffer->str.u8str);
#endif /* FT_HAVE_UTF8 */
                default: assert(0); return NULL;
            }
        }

        static int vsnprintf_buffer(f_string_buffer_t* buffer, const struct f_string_view* fmt, va_list* va)
        {
            /* Disable compiler diagnostic (format string is not a string literal) */
#if defined(FT_CLANG_COMPILER)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif
#if defined(FT_GCC_COMPILER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif
            s32 width_capacity = string_buffer_width_capacity(buffer);
            switch (buffer->type)
            {
                case CHAR_BUF: return vsnprintf(buffer->str.cstr, width_capacity, fmt->u.cstr, *va);
#ifdef FT_HAVE_WCHAR
                case W_CHAR_BUF: return vswprintf(buffer->str.wstr, width_capacity, fmt->u.wstr, *va);
#endif
#ifdef FT_HAVE_UTF8
                case UTF8_BUF: return vsnprintf(buffer->str.cstr, width_capacity, fmt->u.cstr, *va);
#endif
                default: assert(0); return 0;
            }
#if defined(FT_CLANG_COMPILER)
#pragma clang diagnostic pop
#endif
#if defined(FT_GCC_COMPILER)
#pragma GCC diagnostic pop
#endif
        }

        FT_INTERNAL
        f_row_t* create_row_from_fmt_string(const struct f_string_view* fmt, va_list* va_args)
        {
            f_string_buffer_t* buffer = create_string_buffer(DEFAULT_STR_BUF_SIZE, fmt->type);
            if (buffer == NULL)
                return NULL;

            s32 cols_origin = number_of_columns_in_format_string(fmt);
            s32 cols        = 0;

            while (1)
            {
                va_list va;
                va_copy(va, *va_args);
                int virtual_sz = vsnprintf_buffer(buffer, fmt, &va);
                va_end(va);
                /* If error encountered */
                if (virtual_sz < 0)
                    goto clear;

                /* Successful write */
                if ((s32)virtual_sz < string_buffer_width_capacity(buffer))
                    break;

                /* Otherwise buffer was too small, so incr. buffer size ant try again. */
                if (!FT_IS_SUCCESS(realloc_string_buffer_without_copy(buffer)))
                    goto clear;
            }

            cols = number_of_columns_in_format_buffer(buffer);
            if (cols == cols_origin)
            {
                f_row_t* row = create_row_from_buffer(buffer);
                if (row == NULL)
                {
                    goto clear;
                }

                destroy_string_buffer(buffer);
                return row;
            }

            if (cols_origin == 1)
            {
                f_row_t* row = create_row();
                if (row == NULL)
                {
                    goto clear;
                }

                f_cell_t* cell = get_cell_and_create_if_not_exists(row, 0);
                if (cell == NULL)
                {
                    destroy_row(row);
                    goto clear;
                }

                f_status result = fill_cell_from_buffer(cell, buffer);
                if (FT_IS_ERROR(result))
                {
                    destroy_row(row);
                    goto clear;
                }

                destroy_string_buffer(buffer);
                return row;
            }

            /*
             * todo: add processing of cols != cols_origin in a general way
             * (when cols_origin != 1).
             */

        clear:
            destroy_string_buffer(buffer);
            return NULL;
        }

        FT_INTERNAL
        int snprintf_row(const f_row_t* row, f_conv_context_t* cntx, s32* col_width_arr, s32 col_width_arr_sz, s32 row_height)
        {
            const f_context_t* context = cntx->cntx;
            assert(context);

            if (row == NULL)
                return -1;

            s32 cols_in_row = columns_in_row(row);
            if (cols_in_row > col_width_arr_sz)
                return -1;

            /*  Row separator anatomy
             *
             *  L    data    IV    data   IV   data    R
             */
            f_table_properties_t* properties = context->table_properties;

            typedef const char*(*border_chars_point_t)[BORDER_ITEM_POS_SIZE];
            enum ft_row_type row_type                      = (enum ft_row_type)get_cell_property_hierarchically(properties, context->row, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE);
            const char*(*bord_chars)[BORDER_ITEM_POS_SIZE] = (row_type == FT_ROW_HEADER) ? (border_chars_point_t)(&properties->border_style.header_border_chars) : (border_chars_point_t)(&properties->border_style.border_chars);
            const char** L                                 = &(*bord_chars)[LL_bip];
            const char** IV                                = &(*bord_chars)[IV_bip];
            const char** R                                 = &(*bord_chars)[RR_bip];

            s32                             written       = 0;
            int                             tmp           = 0;
            s32                             i             = 0;
            fort_entire_table_properties_t* entire_tprops = &context->table_properties->entire_table_properties;
            for (i = 0; i < row_height; ++i)
            {
                /* Print left margin */
                CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, entire_tprops->left_margin, FT_SPACE));

                /* Print left table boundary */
                CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *L));
                s32 j = 0;
                while (j < col_width_arr_sz)
                {
                    if (j < cols_in_row)
                    {
                        ((f_context_t*)context)->column = j;
                        f_cell_t* cell                  = VECTOR_AT(row->cells, j, f_cell_t*);
                        s32       cell_vis_width        = 0;

                        s32 group_slave_sz = group_cell_number(row, j);
                        cell_vis_width     = col_width_arr[j];
                        s32 slave_j        = 0;
                        s32 master_j       = j;
                        for (slave_j = master_j + 1; slave_j < (master_j + group_slave_sz); ++slave_j)
                        {
                            cell_vis_width += col_width_arr[slave_j] + FORT_COL_SEPARATOR_LENGTH;
                            ++j;
                        }

                        CHCK_RSLT_ADD_TO_WRITTEN(cell_printf(cell, i, cntx, cell_vis_width));
                    }
                    else
                    {
                        /* Print empty cell */
                        CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, col_width_arr[j], FT_SPACE));
                    }

                    /* Print boundary between cells */
                    if (j < col_width_arr_sz - 1)
                        CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *IV));

                    ++j;
                }

                /* Print right table boundary */
                CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *R));

                /* Print right margin */
                CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, entire_tprops->right_margin, FT_SPACE));

                /* Print new line character */
                CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, FT_NEWLINE));
            }
            return (int)written;

        clear:
            return -1;
        }

        /********************************************************
           End of file "row.c"
         ********************************************************/

        /********************************************************
           Begin of file "string_buffer.c"
         ********************************************************/

#include <assert.h>
#include <stddef.h>
#ifdef FT_HAVE_WCHAR
#include <wchar.h>
#endif
#if defined(FT_HAVE_UTF8)
/* #include "utf8.h" */ /* Commented by amalgamation script */
#endif

        static ptrdiff_t str_iter_width(const char* beg, const char* end)
        {
            assert(end >= beg);
            return (end - beg);
        }

#ifdef FT_HAVE_WCHAR
        static ptrdiff_t wcs_iter_width(const wchar_t* beg, const wchar_t* end)
        {
            assert(end >= beg);
            return mk_wcswidth(beg, (s32)(end - beg));
        }
#endif /* FT_HAVE_WCHAR */

        static s32 buf_str_len(const f_string_buffer_t* buf)
        {
            assert(buf);

            switch (buf->type)
            {
                case CHAR_BUF: return strlen(buf->str.cstr);
#ifdef FT_HAVE_WCHAR
                case W_CHAR_BUF: return wcslen(buf->str.wstr);
#endif
#ifdef FT_HAVE_UTF8
                case UTF8_BUF: return utf8len(buf->str.u8str);
#endif
            }

            assert(0);
            return 0;
        }

        FT_INTERNAL
        s32 strchr_count(const char* str, char ch)
        {
            if (str == NULL)
                return 0;

            s32 count = 0;
            str       = strchr(str, ch);
            while (str)
            {
                count++;
                str++;
                str = strchr(str, ch);
            }
            return count;
        }

#ifdef FT_HAVE_WCHAR
        FT_INTERNAL
        s32 wstrchr_count(const wchar_t* str, wchar_t ch)
        {
            if (str == NULL)
                return 0;

            s32 count = 0;
            str       = wcschr(str, ch);
            while (str)
            {
                count++;
                str++;
                str = wcschr(str, ch);
            }
            return count;
        }
#endif

#if defined(FT_HAVE_UTF8)
        /* todo: do something with code below!!! */
        FT_INTERNAL
        void* ut8next(const void* str)
        {
            utf8_int32_t out_codepoint;
            return utf8codepoint(str, &out_codepoint);
        }

        FT_INTERNAL
        s32 utf8chr_count(const void* str, utf8_int32_t ch)
        {
            if (str == NULL)
                return 0;

            s32 count = 0;
            str       = utf8chr(str, ch);
            while (str)
            {
                count++;
                str = ut8next(str);
                str = utf8chr(str, ch);
            }
            return count;
        }
#endif /* FT_HAVE_UTF8 */

        FT_INTERNAL
        const char* str_n_substring_beg(const char* str, char ch_separator, s32 n)
        {
            if (str == NULL)
                return NULL;

            if (n == 0)
                return str;

            str = strchr(str, ch_separator);
            --n;
            while (n > 0)
            {
                if (str == NULL)
                    return NULL;
                --n;
                str++;
                str = strchr(str, ch_separator);
            }
            return str ? (str + 1) : NULL;
        }

#ifdef FT_HAVE_WCHAR
        FT_INTERNAL
        const wchar_t* wstr_n_substring_beg(const wchar_t* str, wchar_t ch_separator, s32 n)
        {
            if (str == NULL)
                return NULL;

            if (n == 0)
                return str;

            str = wcschr(str, ch_separator);
            --n;
            while (n > 0)
            {
                if (str == NULL)
                    return NULL;
                --n;
                str++;
                str = wcschr(str, ch_separator);
            }
            return str ? (str + 1) : NULL;
        }
#endif /* FT_HAVE_WCHAR */

#if defined(FT_HAVE_UTF8)
        FT_INTERNAL
        const void* utf8_n_substring_beg(const void* str, utf8_int32_t ch_separator, s32 n)
        {
            if (str == NULL)
                return NULL;

            if (n == 0)
                return str;

            str = utf8chr(str, ch_separator);
            --n;
            while (n > 0)
            {
                if (str == NULL)
                    return NULL;
                --n;
                str = ut8next(str);
                str = utf8chr(str, ch_separator);
            }
            return str ? (ut8next(str)) : NULL;
        }
#endif

        FT_INTERNAL
        void str_n_substring(const char* str, char ch_separator, s32 n, const char** begin, const char** end)
        {
            const char* beg = str_n_substring_beg(str, ch_separator, n);
            if (beg == NULL)
            {
                *begin = NULL;
                *end   = NULL;
                return;
            }

            const char* en = strchr(beg, ch_separator);
            if (en == NULL)
            {
                en = str + strlen(str);
            }

            *begin = beg;
            *end   = en;
            return;
        }

#ifdef FT_HAVE_WCHAR
        FT_INTERNAL
        void wstr_n_substring(const wchar_t* str, wchar_t ch_separator, s32 n, const wchar_t** begin, const wchar_t** end)
        {
            const wchar_t* beg = wstr_n_substring_beg(str, ch_separator, n);
            if (beg == NULL)
            {
                *begin = NULL;
                *end   = NULL;
                return;
            }

            const wchar_t* en = wcschr(beg, ch_separator);
            if (en == NULL)
            {
                en = str + wcslen(str);
            }

            *begin = beg;
            *end   = en;
            return;
        }
#endif /* FT_HAVE_WCHAR */

#if defined(FT_HAVE_UTF8)
        FT_INTERNAL
        void utf8_n_substring(const void* str, utf8_int32_t ch_separator, s32 n, const void** begin, const void** end)
        {
            const char* beg = (const char*)utf8_n_substring_beg(str, ch_separator, n);
            if (beg == NULL)
            {
                *begin = NULL;
                *end   = NULL;
                return;
            }

            const char* en = (const char*)utf8chr(beg, ch_separator);
            if (en == NULL)
            {
                en = (const char*)str + strlen((const char*)str);
            }

            *begin = beg;
            *end   = en;
            return;
        }
#endif /* FT_HAVE_UTF8 */

        FT_INTERNAL
        f_string_buffer_t* create_string_buffer(s32 n_chars, enum f_string_type type)
        {
            s32 char_sz = 0;
            switch (type)
            {
                case CHAR_BUF: char_sz = 1; break;
#ifdef FT_HAVE_WCHAR
                case W_CHAR_BUF: char_sz = sizeof(wchar_t); break;
#endif
#ifdef FT_HAVE_UTF8
                case UTF8_BUF: char_sz = 4; break;
#endif
            }

            s32                sz     = n_chars * char_sz;
            f_string_buffer_t* result = (f_string_buffer_t*)F_MALLOC(sizeof(f_string_buffer_t));
            if (result == NULL)
                return NULL;
            result->str.data = F_MALLOC(sz);
            if (result->str.data == NULL)
            {
                F_FREE(result);
                return NULL;
            }
            result->data_sz = sz;
            result->type    = type;

            if (sz)
            {
                switch (type)
                {
                    case CHAR_BUF: result->str.cstr[0] = '\0'; break;
#ifdef FT_HAVE_WCHAR
                    case W_CHAR_BUF: result->str.wstr[0] = L'\0'; break;
#endif
#ifdef FT_HAVE_UTF8
                    case UTF8_BUF: result->str.cstr[0] = '\0'; break;
#endif
                }
            }

            return result;
        }

        FT_INTERNAL
        void destroy_string_buffer(f_string_buffer_t* buffer)
        {
            if (buffer == NULL)
                return;
            F_FREE(buffer->str.data);
            buffer->str.data = NULL;
            F_FREE(buffer);
        }

        FT_INTERNAL
        f_string_buffer_t* copy_string_buffer(const f_string_buffer_t* buffer)
        {
            assert(buffer);
            f_string_buffer_t* result = create_string_buffer(buffer->data_sz, buffer->type);
            if (result == NULL)
                return NULL;
            switch (buffer->type)
            {
                case CHAR_BUF:
                    if (FT_IS_ERROR(fill_buffer_from_string(result, buffer->str.cstr)))
                    {
                        destroy_string_buffer(result);
                        return NULL;
                    }
                    break;
#ifdef FT_HAVE_WCHAR
                case W_CHAR_BUF:
                    if (FT_IS_ERROR(fill_buffer_from_wstring(result, buffer->str.wstr)))
                    {
                        destroy_string_buffer(result);
                        return NULL;
                    }
                    break;
#endif /* FT_HAVE_WCHAR */
                default: destroy_string_buffer(result); return NULL;
            }
            return result;
        }

        FT_INTERNAL
        f_status realloc_string_buffer_without_copy(f_string_buffer_t* buffer)
        {
            assert(buffer);
            char* new_str = (char*)F_MALLOC(buffer->data_sz * 2);
            if (new_str == NULL)
            {
                return FT_MEMORY_ERROR;
            }
            F_FREE(buffer->str.data);
            buffer->str.data = new_str;
            buffer->data_sz *= 2;
            return FT_SUCCESS;
        }

        FT_INTERNAL
        f_status fill_buffer_from_string(f_string_buffer_t* buffer, const char* str)
        {
            assert(buffer);
            assert(str);

            char* copy = F_STRDUP(str);
            if (copy == NULL)
                return FT_MEMORY_ERROR;

            F_FREE(buffer->str.data);
            buffer->str.cstr = copy;
            buffer->type     = CHAR_BUF;

            return FT_SUCCESS;
        }

#ifdef FT_HAVE_WCHAR
        FT_INTERNAL
        f_status fill_buffer_from_wstring(f_string_buffer_t* buffer, const wchar_t* str)
        {
            assert(buffer);
            assert(str);

            wchar_t* copy = F_WCSDUP(str);
            if (copy == NULL)
                return FT_MEMORY_ERROR;

            F_FREE(buffer->str.data);
            buffer->str.wstr = copy;
            buffer->type     = W_CHAR_BUF;

            return FT_SUCCESS;
        }
#endif /* FT_HAVE_WCHAR */

#ifdef FT_HAVE_UTF8
        FT_INTERNAL
        f_status fill_buffer_from_u8string(f_string_buffer_t* buffer, const void* str)
        {
            assert(buffer);
            assert(str);

            void* copy = F_UTF8DUP(str);
            if (copy == NULL)
                return FT_MEMORY_ERROR;

            F_FREE(buffer->str.u8str);
            buffer->str.u8str = copy;
            buffer->type      = UTF8_BUF;

            return FT_SUCCESS;
        }
#endif /* FT_HAVE_UTF8 */

        FT_INTERNAL
        s32 buffer_text_visible_height(const f_string_buffer_t* buffer)
        {
            if (buffer == NULL || buffer->str.data == NULL || buf_str_len(buffer) == 0)
            {
                return 0;
            }
            if (buffer->type == CHAR_BUF)
                return 1 + strchr_count(buffer->str.cstr, '\n');
#ifdef FT_HAVE_WCHAR
            else if (buffer->type == W_CHAR_BUF)
                return 1 + wstrchr_count(buffer->str.wstr, L'\n');
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
            else if (buffer->type == UTF8_BUF)
                return 1 + utf8chr_count(buffer->str.u8str, '\n');
#endif /* FT_HAVE_WCHAR */

            assert(0);
            return 0;
        }

        FT_INTERNAL
        s32 string_buffer_cod_width_capacity(const f_string_buffer_t* buffer) { return string_buffer_width_capacity(buffer); }

        FT_INTERNAL
        s32 string_buffer_raw_capacity(const f_string_buffer_t* buffer) { return buffer->data_sz; }

#ifdef FT_HAVE_UTF8
        /* User provided function to compute utf8 string visible width */
        static int (*_custom_u8strwid)(const void* beg, const void* end, s32* width) = NULL;

        FT_INTERNAL
        void buffer_set_u8strwid_func(int (*u8strwid)(const void* beg, const void* end, s32* width)) { _custom_u8strwid = u8strwid; }

        static s32 utf8_width(const void* beg, const void* end)
        {
            if (_custom_u8strwid)
            {
                s32 width = 0;
                if (!_custom_u8strwid(beg, end, &width))
                    return width;
            }

            s32   sz  = (s32)((const char*)end - (const char*)beg);
            char* tmp = (char*)F_MALLOC(sizeof(char) * (sz + 1));
            // @todo: add check to tmp
            assert(tmp);

            memcpy(tmp, beg, sz);
            tmp[sz]    = '\0';
            s32 result = utf8width(tmp);
            F_FREE(tmp);
            return result;
        }
#endif /* FT_HAVE_WCHAR */

        FT_INTERNAL
        s32 buffer_text_visible_width(const f_string_buffer_t* buffer)
        {
            s32 max_length = 0;
            if (buffer->type == CHAR_BUF)
            {
                s32 n = 0;
                while (1)
                {
                    const char* beg = NULL;
                    const char* end = NULL;
                    str_n_substring(buffer->str.cstr, '\n', n, &beg, &end);
                    if (beg == NULL || end == NULL)
                        return max_length;

                    max_length = MAX(max_length, (s32)(end - beg));
                    ++n;
                }
#ifdef FT_HAVE_WCHAR
            }
            else if (buffer->type == W_CHAR_BUF)
            {
                s32 n = 0;
                while (1)
                {
                    const wchar_t* beg = NULL;
                    const wchar_t* end = NULL;
                    wstr_n_substring(buffer->str.wstr, L'\n', n, &beg, &end);
                    if (beg == NULL || end == NULL)
                        return max_length;

                    int line_width = mk_wcswidth(beg, (s32)(end - beg));
                    if (line_width < 0) /* For safety */
                        line_width = 0;
                    max_length = MAX(max_length, (s32)line_width);

                    ++n;
                }
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
            }
            else if (buffer->type == UTF8_BUF)
            {
                s32 n = 0;
                while (1)
                {
                    const void* beg = NULL;
                    const void* end = NULL;
                    utf8_n_substring(buffer->str.u8str, '\n', n, &beg, &end);
                    if (beg == NULL || end == NULL)
                        return max_length;

                    max_length = MAX(max_length, (s32)utf8_width(beg, end));
                    ++n;
                }
#endif /* FT_HAVE_WCHAR */
            }

            return max_length; /* shouldn't be here */
        }

        static void buffer_substring(const f_string_buffer_t* buffer, s32 buffer_row, const void** begin, const void** end, ptrdiff_t* str_it_width)
        {
            switch (buffer->type)
            {
                case CHAR_BUF:
                    str_n_substring(buffer->str.cstr, '\n', buffer_row, (const char**)begin, (const char**)end);
                    if ((*(const char**)begin) && (*(const char**)end))
                        *str_it_width = str_iter_width(*(const char**)begin, *(const char**)end);
                    break;
#ifdef FT_HAVE_WCHAR
                case W_CHAR_BUF:
                    wstr_n_substring(buffer->str.wstr, L'\n', buffer_row, (const wchar_t**)begin, (const wchar_t**)end);
                    if ((*(const wchar_t**)begin) && (*(const wchar_t**)end))
                        *str_it_width = wcs_iter_width(*(const wchar_t**)begin, *(const wchar_t**)end);
                    break;
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
                case UTF8_BUF:
                    utf8_n_substring(buffer->str.u8str, '\n', buffer_row, begin, end);
                    if ((*(const char**)begin) && (*(const char**)end))
                        *str_it_width = utf8_width(*begin, *end);
                    break;
#endif /* FT_HAVE_UTF8 */
                default: assert(0);
            }
        }

        static int buffer_print_range(f_conv_context_t* cntx, const void* beg, const void* end)
        {
            s32 len;
            switch (cntx->b_type)
            {
                case CHAR_BUF: len = (s32)((const char*)end - (const char*)beg); return ft_nprint(cntx, (const char*)beg, len);
#ifdef FT_HAVE_WCHAR
                case W_CHAR_BUF: len = (s32)((const wchar_t*)end - (const wchar_t*)beg); return ft_nwprint(cntx, (const wchar_t*)beg, len);
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
                case UTF8_BUF: return ft_nu8print(cntx, beg, end);
#endif /* FT_HAVE_UTF8 */
                default: assert(0); return -1;
            }
        }

        FT_INTERNAL
        int buffer_printf(f_string_buffer_t* buffer, s32 buffer_row, f_conv_context_t* cntx, s32 vis_width, const char* content_style_tag, const char* reset_content_style_tag)
        {
            const f_context_t*    context = cntx->cntx;
            f_table_properties_t* props   = context->table_properties;
            s32                   row     = context->row;
            s32                   column  = context->column;

            if (buffer == NULL || buffer->str.data == NULL || buffer_row >= buffer_text_visible_height(buffer))
            {
                return -1;
            }

            s32 content_width = buffer_text_visible_width(buffer);
            if (vis_width < content_width)
                return -1;

            s32 left  = 0;
            s32 right = 0;
            switch (get_cell_property_hierarchically(props, row, column, FT_CPROP_TEXT_ALIGN))
            {
                case FT_ALIGNED_LEFT:
                    left  = 0;
                    right = (vis_width)-content_width;
                    break;
                case FT_ALIGNED_CENTER:
                    left  = ((vis_width)-content_width) / 2;
                    right = ((vis_width)-content_width) - left;
                    break;
                case FT_ALIGNED_RIGHT:
                    left  = (vis_width)-content_width;
                    right = 0;
                    break;
                default: assert(0); break;
            }

            s32         written      = 0;
            int         tmp          = 0;
            ptrdiff_t   str_it_width = 0;
            const void* beg          = NULL;
            const void* end          = NULL;
            buffer_substring(buffer, buffer_row, &beg, &end, &str_it_width);
            if (beg == NULL || end == NULL)
                return -1;
            if (str_it_width < 0 || content_width < (s32)str_it_width)
                return -1;

            s32 padding = content_width - (s32)str_it_width;

            CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, left, FT_SPACE));
            CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, content_style_tag));
            CHCK_RSLT_ADD_TO_WRITTEN(buffer_print_range(cntx, beg, end));
            CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, reset_content_style_tag));
            CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, padding, FT_SPACE));
            CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, right, FT_SPACE));
            return (int)written;

        clear:
            return -1;
        }

        FT_INTERNAL
        s32 string_buffer_width_capacity(const f_string_buffer_t* buffer)
        {
            assert(buffer);
            switch (buffer->type)
            {
                case CHAR_BUF: return buffer->data_sz;
#ifdef FT_HAVE_WCHAR
                case W_CHAR_BUF: return buffer->data_sz / sizeof(wchar_t);
#endif
#ifdef FT_HAVE_UTF8
                case UTF8_BUF: return buffer->data_sz / 4;
#endif
                default: assert(0); return 0;
            }
        }

        FT_INTERNAL
        void* buffer_get_data(f_string_buffer_t* buffer)
        {
            assert(buffer);
            return buffer->str.data;
        }

        FT_INTERNAL
        int buffer_check_align(f_string_buffer_t* buffer)
        {
            assert(buffer);
            assert(buffer->str.data);

            switch (buffer->type)
            {
                case CHAR_BUF: return 1;
#ifdef FT_HAVE_WCHAR
                case W_CHAR_BUF: return (((uintptr_t)buffer->str.data) & (sizeof(wchar_t) - 1)) == 0;
#endif
#ifdef FT_HAVE_UTF8
                case UTF8_BUF: return 1;
#endif
                default: assert(0); return 0;
            }
        }

        /********************************************************
           End of file "string_buffer.c"
         ********************************************************/

        /********************************************************
           Begin of file "table.c"
         ********************************************************/

        FT_INTERNAL
        f_separator_t* create_separator(int enabled)
        {
            f_separator_t* res = (f_separator_t*)F_CALLOC(1, sizeof(f_separator_t));
            if (res == NULL)
                return NULL;
            res->enabled = enabled;
            return res;
        }

        FT_INTERNAL
        void destroy_separator(f_separator_t* sep) { F_FREE(sep); }

        FT_INTERNAL
        f_separator_t* copy_separator(f_separator_t* sep)
        {
            assert(sep);
            return create_separator(sep->enabled);
        }

        static f_row_t* get_row_impl(ft_table_t* table, s32 row, enum f_get_policy policy)
        {
            if (table == NULL || table->rows == NULL)
            {
                return NULL;
            }

            switch (policy)
            {
                case DONT_CREATE_ON_NULL:
                    if (row < vector_size(table->rows))
                    {
                        return VECTOR_AT(table->rows, row, f_row_t*);
                    }
                    return NULL;
                case CREATE_ON_NULL:
                    while (row >= vector_size(table->rows))
                    {
                        f_row_t* new_row = create_row();
                        if (new_row == NULL)
                            return NULL;
                        if (FT_IS_ERROR(vector_push(table->rows, &new_row)))
                        {
                            destroy_row(new_row);
                            return NULL;
                        }
                    }
                    return VECTOR_AT(table->rows, row, f_row_t*);
            }

            assert(0 && "Shouldn't be here!");
            return NULL;
        }

        FT_INTERNAL
        f_row_t* get_row(ft_table_t* table, s32 row) { return get_row_impl(table, row, DONT_CREATE_ON_NULL); }

        FT_INTERNAL
        const f_row_t* get_row_c(const ft_table_t* table, s32 row) { return get_row((ft_table_t*)table, row); }

        FT_INTERNAL
        f_row_t* get_row_and_create_if_not_exists(ft_table_t* table, s32 row) { return get_row_impl(table, row, CREATE_ON_NULL); }

        FT_INTERNAL
        f_string_buffer_t* get_cur_str_buffer_and_create_if_not_exists(ft_table_t* table)
        {
            assert(table);

            f_row_t* row = get_row_and_create_if_not_exists(table, table->cur_row);
            if (row == NULL)
                return NULL;

            f_cell_t*                       cell        = NULL;
            fort_entire_table_properties_t* table_props = &table->properties->entire_table_properties;
            switch (table_props->add_strategy)
            {
                case FT_STRATEGY_INSERT: cell = create_cell_in_position(row, table->cur_col); break;
                case FT_STRATEGY_REPLACE: cell = get_cell_and_create_if_not_exists(row, table->cur_col); break;
                default: assert(0 && "Unexpected situation inside libfort"); break;
            }

            if (cell == NULL)
                return NULL;

            return cell_get_string_buffer(cell);
        }

        /*
         * Returns number of cells (rows * cols)
         */
        FT_INTERNAL
        f_status get_table_sizes(const ft_table_t* table, s32* rows, s32* cols)
        {
            *rows = 0;
            *cols = 0;
            if (table && table->rows)
            {
                *rows         = vector_size(table->rows);
                s32 row_index = 0;
                for (row_index = 0; row_index < vector_size(table->rows); ++row_index)
                {
                    f_row_t* row         = VECTOR_AT(table->rows, row_index, f_row_t*);
                    s32      cols_in_row = columns_in_row(row);
                    if (cols_in_row > *cols)
                        *cols = cols_in_row;
                }
            }
            return FT_SUCCESS;
        }

        FT_INTERNAL
        f_status table_rows_and_cols_geometry(const ft_table_t* table, s32** col_width_arr_p, s32* col_width_arr_sz, s32** row_height_arr_p, s32* row_height_arr_sz, enum f_geometry_type geom)
        {
            if (table == NULL)
            {
                return FT_GEN_ERROR;
            }

            s32 max_invis_codepoints = 0;
            s32 cols                 = 0;
            s32 rows                 = 0;
            int status               = get_table_sizes(table, &rows, &cols);
            if (FT_IS_ERROR(status))
                return status;

            s32* col_width_arr  = (s32*)F_CALLOC(cols, sizeof(s32));
            s32* row_height_arr = (s32*)F_CALLOC(rows, sizeof(s32));
            if (col_width_arr == NULL || row_height_arr == NULL)
            {
                F_FREE(col_width_arr);
                F_FREE(row_height_arr);
                return FT_GEN_ERROR;
            }

            int         combined_cells_found = 0;
            f_context_t context;
            context.table_properties = (table->properties ? table->properties : &g_table_properties);
            s32 col                  = 0;
            for (col = 0; col < cols; ++col)
            {
                col_width_arr[col] = 0;
                s32 row            = 0;
                for (row = 0; row < rows; ++row)
                {
                    const f_row_t*  row_p = get_row_c(table, row);
                    const f_cell_t* cell  = get_cell_c(row_p, col);
                    context.column        = col;
                    context.row           = row;
                    if (cell)
                    {
                        switch (get_cell_type(cell))
                        {
                            case COMMON_CELL: col_width_arr[col] = MAX(col_width_arr[col], cell_vis_width(cell, &context)); break;
                            case GROUP_MASTER_CELL: combined_cells_found = 1; break;
                            case GROUP_SLAVE_CELL:; /* Do nothing */ break;
                        }
                        row_height_arr[row] = MAX(row_height_arr[row], hint_height_cell(cell, &context));
                    }
                    else
                    {
                        s32 cell_empty_string_height = get_cell_property_hierarchically(context.table_properties, context.row, context.column, FT_CPROP_EMPTY_STR_HEIGHT);
                        if (cell_empty_string_height)
                        {
                            s32 cell_top_padding    = get_cell_property_hierarchically(context.table_properties, context.row, context.column, FT_CPROP_TOP_PADDING);
                            s32 cell_bottom_padding = get_cell_property_hierarchically(context.table_properties, context.row, context.column, FT_CPROP_BOTTOM_PADDING);
                            row_height_arr[row]     = MAX(row_height_arr[row], cell_empty_string_height + cell_top_padding + cell_bottom_padding);
                        }
                    }
                }

                if (geom == INTERN_REPR_GEOMETRY)
                {
                    max_invis_codepoints = 0;
                    for (row = 0; row < rows; ++row)
                    {
                        const f_row_t*  row_p = get_row_c(table, row);
                        const f_cell_t* cell  = get_cell_c(row_p, col);
                        if (!cell)
                            continue;
                        context.column       = col;
                        context.row          = row;
                        s32 inv_codepoints   = cell_invis_codes_width(cell, &context);
                        max_invis_codepoints = MAX(max_invis_codepoints, inv_codepoints);
                    }
                    col_width_arr[col] += max_invis_codepoints;
                }
            }

            if (combined_cells_found)
            {
                for (col = 0; col < cols; ++col)
                {
                    s32 row = 0;
                    for (row = 0; row < rows; ++row)
                    {
                        const f_row_t*  row_p = get_row_c(table, row);
                        const f_cell_t* cell  = get_cell_c(row_p, col);
                        context.column        = col;
                        context.row           = row;
                        if (cell)
                        {
                            if (get_cell_type(cell) == GROUP_MASTER_CELL)
                            {
                                s32 hint_width = cell_vis_width(cell, &context);
                                if (geom == INTERN_REPR_GEOMETRY)
                                {
                                    hint_width += cell_invis_codes_width(cell, &context);
                                }
                                s32 slave_col   = col + group_cell_number(row_p, col);
                                s32 cur_adj_col = col;
                                s32 group_width = col_width_arr[col];
                                s32 i;
                                for (i = col + 1; i < slave_col; ++i)
                                    group_width += col_width_arr[i] + FORT_COL_SEPARATOR_LENGTH;
                                /* adjust col. widths */
                                while (1)
                                {
                                    if (group_width >= hint_width)
                                        break;
                                    col_width_arr[cur_adj_col] += 1;
                                    group_width++;
                                    cur_adj_col++;
                                    if (cur_adj_col == slave_col)
                                        cur_adj_col = col;
                                }
                            }
                        }
                    }
                }
            }

            /* todo: Maybe it is better to move min width checking to a particular cell
             * width checking. At the moment min width includes paddings. Maybe it is
             * better that min width weren't include paddings but be min width of the
             * cell content without padding
             */
            /*
            if (table->properties) {
                for (s32 i = 0; i < cols; ++i) {
                    col_width_arr[i] = MAX((int)col_width_arr[i], fort_props_column_width(table->properties, i));
                }
            }
            */

            *col_width_arr_p   = col_width_arr;
            *col_width_arr_sz  = cols;
            *row_height_arr_p  = row_height_arr;
            *row_height_arr_sz = rows;
            return FT_SUCCESS;
        }

        /*
         * Returns geometry in characters
         */
        FT_INTERNAL
        f_status table_geometry(const ft_table_t* table, s32* height, s32* width)
        {
            if (table == NULL)
                return FT_GEN_ERROR;

            *height             = 0;
            *width              = 0;
            s32  cols           = 0;
            s32  rows           = 0;
            s32* col_width_arr  = NULL;
            s32* row_height_arr = NULL;

            int status = table_rows_and_cols_geometry(table, &col_width_arr, &cols, &row_height_arr, &rows, INTERN_REPR_GEOMETRY);
            if (FT_IS_ERROR(status))
                return status;

            *width = 1 + (cols == 0 ? 1 : cols) + 1; /* for boundaries (that take 1 symbol) + newline   */
            s32 i  = 0;
            for (i = 0; i < cols; ++i)
            {
                *width += col_width_arr[i];
            }

            /* todo: add check for non printable horizontal row separators */
            *height = 1 + (rows == 0 ? 1 : rows); /* for boundaries (that take 1 symbol)  */
            for (i = 0; i < rows; ++i)
            {
                *height += row_height_arr[i];
            }
            F_FREE(col_width_arr);
            F_FREE(row_height_arr);

            f_table_properties_t* properties = table->properties;
            if (properties)
            {
                *height += properties->entire_table_properties.top_margin;
                *height += properties->entire_table_properties.bottom_margin;
                *width += properties->entire_table_properties.left_margin;
                *width += properties->entire_table_properties.right_margin;
            }

            /* Take into account that border elements can be more than one byte long */
            f_table_properties_t* table_properties    = properties ? properties : &g_table_properties;
            s32                   max_border_elem_len = max_border_elem_strlen(table_properties);
            *width *= max_border_elem_len;

            return FT_SUCCESS;
        }

        FT_INTERNAL
        f_status table_internal_codepoints_geometry(const ft_table_t* table, s32* height, s32* width) { return table_geometry(table, height, width); }

        /********************************************************
           End of file "table.c"
         ********************************************************/

        /********************************************************
           Begin of file "vector.c"
         ********************************************************/

        struct f_vector
        {
            s32   m_size;
            void* m_data;
            s32   m_capacity;
            s32   m_item_size;
        };

        static int vector_reallocate_(f_vector_t* vector, s32 new_capacity)
        {
            assert(vector);
            assert(new_capacity > vector->m_capacity);

            s32 new_size   = new_capacity * vector->m_item_size;
            vector->m_data = F_REALLOC(vector->m_data, new_size);
            if (vector->m_data == NULL)
                return -1;
            return 0;
        }

        FT_INTERNAL
        f_vector_t* create_vector(s32 item_size, s32 capacity)
        {
            f_vector_t* vector = (f_vector_t*)F_MALLOC(sizeof(f_vector_t));
            if (vector == NULL)
            {
                return NULL;
            }

            s32 init_size  = MAX(item_size * capacity, 1);
            vector->m_data = F_MALLOC(init_size);
            if (vector->m_data == NULL)
            {
                F_FREE(vector);
                return NULL;
            }

            vector->m_size      = 0;
            vector->m_capacity  = capacity;
            vector->m_item_size = item_size;

            return vector;
        }

        FT_INTERNAL
        void destroy_vector(f_vector_t* vector)
        {
            assert(vector);
            F_FREE(vector->m_data);
            F_FREE(vector);
        }

        FT_INTERNAL
        s32 vector_size(const f_vector_t* vector)
        {
            assert(vector);
            return vector->m_size;
        }

        FT_INTERNAL
        s32 vector_capacity(const f_vector_t* vector)
        {
            assert(vector);
            return vector->m_capacity;
        }

        FT_INTERNAL
        int vector_push(f_vector_t* vector, const void* item)
        {
            assert(vector);
            assert(item);

            if (vector->m_size == vector->m_capacity)
            {
                if (vector_reallocate_(vector, vector->m_capacity * 2) == -1)
                    return FT_GEN_ERROR;
                vector->m_capacity = vector->m_capacity * 2;
            }

            s32 offset = vector->m_size * vector->m_item_size;
            memcpy((char*)vector->m_data + offset, item, vector->m_item_size);

            ++(vector->m_size);

            return FT_SUCCESS;
        }

        FT_INTERNAL
        int vector_insert(f_vector_t* vector, const void* item, s32 pos)
        {
            assert(vector);
            assert(item);
            s32 needed_capacity = MAX(pos + 1, vector->m_size + 1);
            if (vector->m_capacity < needed_capacity)
            {
                if (vector_reallocate_(vector, needed_capacity) == -1)
                    return FT_GEN_ERROR;
                vector->m_capacity = needed_capacity;
            }
            s32 offset = pos * vector->m_item_size;
            if (pos >= vector->m_size)
            {
                /* Data in the middle are not initialized */
                memcpy((char*)vector->m_data + offset, item, vector->m_item_size);
                vector->m_size = pos + 1;
                return FT_SUCCESS;
            }
            else
            {
                /* Shift following data by one position */
                memmove((char*)vector->m_data + offset + vector->m_item_size, (char*)vector->m_data + offset, vector->m_item_size * (vector->m_size - pos));
                memcpy((char*)vector->m_data + offset, item, vector->m_item_size);
                ++(vector->m_size);
                return FT_SUCCESS;
            }
        }

        FT_INTERNAL
        f_vector_t* vector_split(f_vector_t* vector, s32 pos)
        {
            s32         trailing_sz = vector->m_size > pos ? vector->m_size - pos : 0;
            f_vector_t* new_vector  = create_vector(vector->m_item_size, trailing_sz);
            if (!new_vector)
                return new_vector;
            if (new_vector->m_capacity < trailing_sz)
            {
                destroy_vector(new_vector);
                return NULL;
            }

            if (trailing_sz == 0)
                return new_vector;

            s32 offset = vector->m_item_size * pos;
            memcpy(new_vector->m_data, (char*)vector->m_data + offset, trailing_sz * vector->m_item_size);
            new_vector->m_size = trailing_sz;
            vector->m_size     = pos;
            return new_vector;
        }

        FT_INTERNAL
        const void* vector_at_c(const f_vector_t* vector, s32 index)
        {
            if (index >= vector->m_size)
                return NULL;

            return (char*)vector->m_data + index * vector->m_item_size;
        }

        FT_INTERNAL
        void* vector_at(f_vector_t* vector, s32 index)
        {
            if (index >= vector->m_size)
                return NULL;

            return (char*)vector->m_data + index * vector->m_item_size;
        }

        FT_INTERNAL
        f_status vector_swap(f_vector_t* cur_vec, f_vector_t* mv_vec, s32 pos)
        {
            assert(cur_vec);
            assert(mv_vec);
            assert(cur_vec != mv_vec);
            assert(cur_vec->m_item_size == mv_vec->m_item_size);

            s32 cur_sz = vector_size(cur_vec);
            s32 mv_sz  = vector_size(mv_vec);
            if (mv_sz == 0)
            {
                return FT_SUCCESS;
            }

            s32 min_targ_size = pos + mv_sz;
            if (vector_capacity(cur_vec) < min_targ_size)
            {
                if (vector_reallocate_(cur_vec, min_targ_size) == -1)
                    return FT_GEN_ERROR;
                cur_vec->m_capacity = min_targ_size;
            }

            s32   offset    = pos * cur_vec->m_item_size;
            void* tmp       = NULL;
            s32   new_mv_sz = 0;
            if (cur_sz > pos)
            {
                new_mv_sz = MIN(cur_sz - pos, mv_sz);
                tmp       = F_MALLOC(cur_vec->m_item_size * new_mv_sz);
                if (tmp == NULL)
                {
                    return FT_MEMORY_ERROR;
                }
            }

            if (tmp)
            {
                memcpy(tmp, (char*)cur_vec->m_data + offset, cur_vec->m_item_size * new_mv_sz);
            }

            memcpy((char*)cur_vec->m_data + offset, mv_vec->m_data, cur_vec->m_item_size * mv_sz);

            if (tmp)
            {
                memcpy(mv_vec->m_data, tmp, cur_vec->m_item_size * new_mv_sz);
            }

            cur_vec->m_size = MAX(cur_vec->m_size, min_targ_size);
            mv_vec->m_size  = new_mv_sz;
            F_FREE(tmp);
            return FT_SUCCESS;
        }

        FT_INTERNAL
        void vector_clear(f_vector_t* vector) { vector->m_size = 0; }

        FT_INTERNAL
        int vector_erase(f_vector_t* vector, s32 index)
        {
            assert(vector);

            if (vector->m_size == 0 || index >= vector->m_size)
                return FT_GEN_ERROR;

            memmove((char*)vector->m_data + vector->m_item_size * index, (char*)vector->m_data + vector->m_item_size * (index + 1), (vector->m_size - 1 - index) * vector->m_item_size);
            vector->m_size--;
            return FT_SUCCESS;
        }

#ifdef FT_TEST_BUILD

        f_vector_t* copy_vector(f_vector_t* v)
        {
            if (v == NULL)
                return NULL;

            f_vector_t* new_vector = create_vector(v->m_item_size, v->m_capacity);
            if (new_vector == NULL)
                return NULL;

            memcpy(new_vector->m_data, v->m_data, v->m_item_size * v->m_size);
            new_vector->m_size      = v->m_size;
            new_vector->m_item_size = v->m_item_size;
            return new_vector;
        }

        s32 vector_index_of(const f_vector_t* vector, const void* item)
        {
            assert(vector);
            assert(item);

            s32 i = 0;
            for (i = 0; i < vector->m_size; ++i)
            {
                void* data_pos = (char*)vector->m_data + i * vector->m_item_size;
                if (memcmp(data_pos, item, vector->m_item_size) == 0)
                {
                    return i;
                }
            }
            return INVALID_VEC_INDEX;
        }

#endif

        /********************************************************
           End of file "vector.c"
         ********************************************************/
    }
}