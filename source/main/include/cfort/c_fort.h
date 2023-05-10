#ifndef __CFORT_LIB_H__
#define __CFORT_LIB_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "ccore/c_allocator.h"
#include "ccore/c_debug.h"

namespace ncore
{
    namespace nfort
    {
        struct table_t;

        table_t* create_table(alloc_t* allocator);
        void     destroy_table(table_t* table);

        struct cursor_t
        {
            s32 row;
            s32 col;
        };

        s32  is_empty(const table_t* table);
        s32  row_count(const table_t* table);
        s32  col_count(const table_t* table);
        s32  next_column(table_t* table, cursor_t& cursor);
        s32  next_row(table_t* table, cursor_t& cursor);

        template <typename... Args>
        s32 table_column_format(table_t* table, cursor_t cursor, const char* format, Args&&... args);
        template <typename... Args>
        s32 table_row_format(table_t* table, cursor_t cursor, const char* format, Args&&... args);
        template <typename... Args>
        s32 table_grid_format(table_t* table, cursor_t cursor, s32 columns, const char* format, Args&&... args);

        s32 table_separator(table_t* table, cursor_t& cursor);

        s32 set_border_style(table_t* table, const border_style_t* style);

        enum EColor
        {
            COLOR_DEFAULT       = 0,  // Default color
            COLOR_BLACK         = 1,  // Black color
            COLOR_RED           = 2,  // Red color
            COLOR_GREEN         = 3,  // Green color
            COLOR_YELLOW        = 4,  // Yellow color
            COLOR_BLUE          = 5,  // Blue color
            COLOR_MAGENTA       = 6,  // Magenta color
            COLOR_CYAN          = 7,  // Cyan color
            COLOR_LIGHT_GRAY    = 8,  // Light gray color
            COLOR_DARK_GRAY     = 9,  // Dark gray color
            COLOR_LIGHT_RED     = 10, // Light red color
            COLOR_LIGHT_GREEN   = 11, // Light green color
            COLOR_LIGHT_YELLOW  = 12, // Light yellow color
            COLOR_LIGHT_BLUE    = 13, // Light blue color
            COLOR_LIGHT_MAGENTA = 14, // Light magenta color
            COLOR_LIGHT_CYAN    = 15, // Light cyan color
            COLOR_LIGHT_WHYTE   = 16  // Light whyte color
        };

        enum ETextAlignment
        {
            ALIGNED_LEFT = 0, // Align left
            ALIGNED_CENTER,   // Align center
            ALIGNED_RIGHT     // Align right
        };

        enum ERowType
        {
            ROW_COMMON = 0, // Common row
            ROW_HEADER      // Header row
        };


    } // namespace nfort
} // namespace ncore

#endif