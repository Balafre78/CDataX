#ifndef CDATAX_COLUMN_H
#define CDATAX_COLUMN_H

#include <stdlib.h>
#include <stdio.h>

#define REALOC_SIZE 256

enum enum_type
{
    NULLVAL = 1 , UINT, INT, CHAR, FLOAT, DOUBLE, STRING, STRUCTURE
};
typedef enum enum_type Enum_type;

union column_type {
    unsigned int uint_value;
    signed int int_value;
    char char_value;
    float float_value;
    double double_value;
    char* string_value;
    void* struct_value;
};
typedef union column_type Col_type;

struct column {
    char *title; // title
    unsigned int size; // logical size
    unsigned int max_size; // physical size
    Enum_type column_type; // data type
    Col_type **data; // array of pointers to stored data
    unsigned long long int *index; // array of integers
};

typedef struct column Column;

/**
* Create a new column
* @param1 : Column type
* @param2 : Column title
* @return : Pointer to the created column
*/
Column *create_column(Enum_type type, char *title);

/**
  * @brief Insert a new value into a column
  * @param1 Pointer to the column
  * @param2 Pointer to the value to insert
  * @return 1 if the value is correctly inserted 0 otherwise
*/
int insert_value(Column *col, void *value);

/**
* @brief: Free the space allocated by a column
* @param1: Pointer to the column
*/
void delete_column(Column **col);

#endif //CDATAX_COLUMN_H
