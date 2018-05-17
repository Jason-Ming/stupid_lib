#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "s_defines.h"
#include "s_log.h"
#include "s_draw.h"


#define PRINT_STRING_SIZE CHART_DATA_INFO_STR_SIZE//每个单元格的大小
#define CHART_ROWS 20 //直方图的单位是20行

typedef enum TAG_ENUM_PRINT_TYPE
{
    PRINT_TYPE_NO = 0,
    PRINT_TYPE_YES,
    PRINT_TYPE_VERTICAL_LINE,
    PRINT_TYPE_ROW_LABEL,
    PRINT_TYPE_HORIZONTAL_LINE,
    PRINT_TYPE_COL_LABEL,
}ENUM_PRINT_TYPE;

typedef struct TAG_STRU_PRINT_UNIT
{
    //ENUM_PRINT_TYPE print_type;
    char print_string[PRINT_STRING_SIZE];
}STRU_PRINT_UNIT;

PRIVATE int table_row = 0, table_col = 0;
PRIVATE STRU_PRINT_UNIT *pTable = NULL;

PRIVATE void init_row_and_col(void)
{
    table_row = 0;
    table_col = 0;
}

PRIVATE void save_row_and_col(int row, int col)
{
    table_row = row;
    table_col = col;
}

ENUM_BOOLEAN check_row_and_col(int row, int col)
{
    return ((row >= 0 && row < table_row) && (col >= 0 && col < table_col))?BOOLEAN_TURE:BOOLEAN_FALSE;
}

PRIVATE STRU_PRINT_UNIT * get_table_unit_ptr(int row, int col)
{
    R_ASSERT(check_row_and_col(row, col) == BOOLEAN_TURE, NULL);
    R_ASSERT(pTable != NULL, NULL);

    return &(pTable[row * table_col + col]);
}

PRIVATE ENUM_RETURN set_print_table_value(int row, int col, ENUM_PRINT_TYPE value_type, int val_num, const char* val_info)
{
    R_ASSERT(check_row_and_col(row, col) == BOOLEAN_TURE, RETURN_FAILURE);
    R_ASSERT(value_type >= PRINT_TYPE_NO && value_type <= PRINT_TYPE_COL_LABEL, RETURN_FAILURE);
    
    STRU_PRINT_UNIT *p = get_table_unit_ptr(row, col);
    R_ASSERT(p != NULL, RETURN_FAILURE);

    char str_buf[PRINT_STRING_SIZE];
    
    switch(value_type)
    {
        case PRINT_TYPE_NO:
            sprintf(str_buf, "     ");
            break;
        case PRINT_TYPE_YES:
            sprintf(str_buf, " *** ");
            break;
        case PRINT_TYPE_VERTICAL_LINE:
            sprintf(str_buf, "  |  ");
            break;
        case PRINT_TYPE_ROW_LABEL:
            sprintf(str_buf, "%5d", val_num);
            break;
        case PRINT_TYPE_HORIZONTAL_LINE:
            sprintf(str_buf, "-----");
            break;
        case PRINT_TYPE_COL_LABEL:
            R_ASSERT(val_info != NULL, RETURN_FAILURE);
            sprintf(str_buf, "%5s", val_info);
            break;
    }

    memcpy(p->print_string, str_buf, PRINT_STRING_SIZE);

    return RETURN_SUCCESS;
}

PRIVATE int get_max_value_of_array(STRU_CHART_DATA array[], int array_size)
{
    int max_value = 0;
    for(int i = 0; i < array_size; i++)
    {
        if(max_value < array[i].val)
        {
            max_value = array[i].val;
        }
    }

    return max_value;
}

PRIVATE ENUM_RETURN alloc_histogram_table(int row, int col)
{
    R_ASSERT(row > 0 && col > 0, RETURN_FAILURE);

    pTable = (STRU_PRINT_UNIT*)malloc(row * col * sizeof(STRU_PRINT_UNIT));
    R_ASSERT(pTable != NULL, RETURN_FAILURE);
    
    save_row_and_col(row, col);
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN init_histogram_table(STRU_CHART_DATA array[], int array_size)
{   
    ENUM_RETURN ret_val;

    int max_value_of_array = get_max_value_of_array(array, array_size);

    //以最大的数量为准，每增加一行的数量
    int step = max_value_of_array/CHART_ROWS + (max_value_of_array % CHART_ROWS == 0 ? 0 : 1);

    printf("max_value_of_array: %d, step: %d\n", max_value_of_array, step);
    
    //初始化整个表
    for(int i = 0; i < table_row; i++)
    {
        for(int j = 0; j < table_col; j++)
        {
            ret_val = set_print_table_value(i, j, PRINT_TYPE_NO, INVALID_INT, NULL);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        }
    }
    
    //初始化列表头
    for(int i = 0; i < table_row - 2; i++)
    {
        ret_val = set_print_table_value(i, 0, PRINT_TYPE_ROW_LABEL, step * (table_row -2 - i), NULL);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        ret_val = set_print_table_value(i, 1, PRINT_TYPE_VERTICAL_LINE, INVALID_INT, NULL);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    
    //初始化表格显示单元
    for(int i = 0; i < table_row -2; i++)
    {
        for(int j = 2; j < table_col; j++)
        {
            if( array[j - 2].val > step * (CHART_ROWS - i - 1))
            {
                ret_val = set_print_table_value(i, j, PRINT_TYPE_YES, INVALID_INT, NULL);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            }
        }
    }

    //初始化行表头
    for(int i = 2; i < table_col; i++)
    {
        ret_val = set_print_table_value(table_row - 2, i, PRINT_TYPE_HORIZONTAL_LINE, INVALID_INT, NULL);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        
        ret_val = set_print_table_value(table_row - 1, i, PRINT_TYPE_COL_LABEL, INVALID_INT, array[i-2].info);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }

    ret_val = set_print_table_value(table_row - 2, 1, PRINT_TYPE_COL_LABEL, INVALID_INT, "  +--");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN make_histogram_table(STRU_CHART_DATA array[], int array_size)
{
    R_ASSERT(array != NULL, RETURN_FAILURE);
    R_ASSERT(array_size > 0, RETURN_FAILURE);
    
    //纵坐标为单词数量，横坐标为单词长度，各增加2行2列打印标尺
    ENUM_RETURN ret_val;

    ret_val = alloc_histogram_table((CHART_ROWS + 2), (array_size + 2));
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = init_histogram_table(array, array_size);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
#if 0
    //打印表格
    for(int i = 0; i < table_row; i++)
    {
        for(int j = 0; j < table_col; j++)
        {
            printf("%5s", (get_table_unit_ptr(i, j))->print_string);
        }

        printf("\n");
    }
#endif

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN draw_hitogram_table(FILE *fpw)
{
    R_ASSERT(fpw != NULL, RETURN_FAILURE);
    R_ASSERT(pTable != NULL, RETURN_FAILURE);
    
    for(int i = 0; i < table_row; i++)
    {
        for(int j = 0; j < table_col; j++)
        {
            fputs(get_table_unit_ptr(i, j)->print_string, fpw);
        }
        fputs("\n", fpw);
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN clear_hitogram_table(void)
{
    R_ASSERT(pTable != NULL, RETURN_FAILURE);
    free(pTable);

    init_row_and_col();

    return RETURN_SUCCESS;
}

ENUM_RETURN draw_histogram(FILE *fpw, STRU_CHART_DATA array[], int array_size)
{
    R_ASSERT(fpw != NULL, RETURN_FAILURE);
    R_ASSERT(array != NULL, RETURN_FAILURE);
    R_ASSERT(array_size > 0, RETURN_FAILURE);

    ENUM_RETURN ret_value;
    ret_value = make_histogram_table(array, array_size);
    R_ASSERT(ret_value == RETURN_SUCCESS, RETURN_FAILURE);

    ret_value = draw_hitogram_table(fpw);
    R_ASSERT(ret_value == RETURN_SUCCESS, RETURN_FAILURE);

    ret_value = clear_hitogram_table();
    R_ASSERT(ret_value == RETURN_SUCCESS, RETURN_FAILURE);
    
    return RETURN_SUCCESS;
}

