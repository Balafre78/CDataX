#ifndef CDATAX_COLUMN_H
#define CDATAX_COLUMN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define REALOC_SIZE 256
#define STD_BUFF_SIZE 32

#define ASC 0
#define DESC 1

#define INVALID 0
#define ALMOST_SORT (-1)
#define SORTED 1

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

typedef long long int indexation;

struct column {
    char *title; // title
    indexation size; // logical size
    indexation max_size; // physical size
    Enum_type column_type; // data type
    Col_type **data; // array of pointers to stored data
    indexation *index; // array of integers
    // index valid
    // 0 : no index
    // -1 : invalid index
    // 1 : valid index
    int valid_index;
    // direction de tri Ascendant ou Déscendant
    // 0 : ASC
    // 1 : DESC
    int sort_dir;
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
int append_value(Column *col, void *value);

/**
* @brief: Free the space allocated by a column
* @param1: Pointer to the column
*/
void delete_column(Column **col);

/*Index functions*/

/**
* @brief: Remove the index of a column
* @param1: Pointer to the columnCOl
*/
void erase_index(Column *col);

/**
* @brief: Check if an index is correct
* @param1: Pointer to the column
* @return:  0: index not existing,
* @return: -1: the index exists but invalid,
* @return: +1: the index is correct
*/
int check_index(Column *col);

/**
* @brief: Update the index
* @param1: Pointer to the column
*/
void update_index(Column *col);

/*Printing functions*/

/**
* @brief: Convert a value into a string
* @param1: Pointer to the column
* @param2: Position of the value in the data array
* @param3: The string in which the value will be written
* @param4: Maximum size of the string
* @return: 0 if everuthing is fine, 1 if the buffer is too small and negative numbers if there's an encoding issue
*/
int convert_value(Column *col, indexation i, char *str, indexation size);

/**
* @brief Display the content of a column (no matter the index)
* @param col Pointer to the column to display
* @return 1 on internal allocation error, 0 else
*/
int print_col_raw(Column *col);

/**
* @brief: Display the content of a sorted column
* @param col Pointer to a column
* @return: 1 on internal allocation error, 0 else
*/
int print_col_by_index(Column *col);

/*Extra functions*/

/**
 * @brief Get the value of the column index given
 * @param col The pointer to the column
 * @param idx The index to look for
 * @return a pointer to that value may return NULL pointer if it doesn't exists
 */
Col_type *get_value(Column *col, indexation index);

/**
 * @brief Compare two vals of the same type of Col_type
 * @param A first param
 * @param B second param
 * @param type their type in Enum_type
 * @return  1 if A > B
 * @return -1 if A < B
 * @return  0 if A == B
 * */
int compare_Col_type(Col_type *A, Col_type *B, Enum_type type);

/**
* @brief: Sort a column according to a given order
* @param col Pointer to the column to sort
* @param sort_dir Sort type (ASC or DESC)
*/
void sort(Column *col, int sort_dir);

/**
 * @brief Get the number of occurrences of value under the one given
 * @param col Pointer to a column
 * @param x The value to compare (same type as col)
 * @return Number of occurrences
 * */
int get_occurrences_inferior_raw(Column *col, Col_type *x);

/**
 * @brief Get the number of occurrences of value under the one given
 * @param col Pointer to a column
 * @param x The value to compare
 * @return Number of occurrences
 * */
int get_occurrences_superior_raw(Column *col, Col_type *x);

/**
 * @brief Get the number of occurrences of value under the one given
 * @param col Pointer to a column
 * @param x The value to compare
 * @return Number of occurrences
 * */
int get_occurrences_equal_raw(Column *col, Col_type *x);

/**
 * @brief Get the number of occurrences of value under the one given, uses indexs
 * @param col Pointer to a column
 * @param x The value to compare
 * @warning works only if the list is sorted !
 * @return Number of occurrences
 * */
int get_occurrences_equal_by_index(Column *col, Col_type *x);

#endif //CDATAX_COLUMN_H
