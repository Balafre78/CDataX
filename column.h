#ifndef CDATAX_COLUMN_H
#define CDATAX_COLUMN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define REALOC_SIZE 256
#define STD_BUFF_SIZE 32

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

/**
* @brief: Convert a value into a string
* @param1: Pointer to the column
* @param2: Position of the value in the data array
* @param3: The string in which the value will be written
* @param4: Maximum size of the string
* @return: 0 if everuthing is fine, 1 if the buffer is too small and negative numbers if there's an encoding issue
*/
int convert_value(Column *col, unsigned int i, char *str, int size);

/**
* @brief: Display the content of a column
* @param: Pointer to the column to display
* @return: 1 on internal allocation error, 0 else
*/
int print_col(Column *col);

/*Extra functions*/

/**
 * @brief Get the value of the column index given
 * @param col The pointer to the column
 * @param idx The index to look for
 * @return a pointer to that value
 * @warning exit the program if invalid index provided
 */
Col_type *get_value(Column *col, unsigned int index);

/**
 * @brief Compare two vals of the same type of Col_type
 * @param A first param
 * @param B second param
 * @param type their type in Enum_type
 * @return 1 if A > B else (-1 if B > A else 0)
 * */
int compare_Col_type(Col_type *A, Col_type *B, Enum_type type);

/**
 * @brief Get the number of occurrences of value under the one given
 * @param col Pointer to a column
 * @param x The value to compare (same type as col)
 * @return Number of occurrences
 * */
int get_occurrences_inferior(Column *col, Col_type *x);

/**
 * @brief Get the number of occurrences of value under the one given
 * @param col Pointer to a column
 * @param x The value to compare
 * @return Number of occurrences
 * */
int get_occurrences_superior(Column *col, Col_type *x);

/**
 * @brief Get the number of occurrences of value under the one given
 * @param col Pointer to a column
 * @param x The value to compare
 * @return Number of occurrences
 * */
int get_occurrences_equal(Column *col, Col_type *x);

#endif //CDATAX_COLUMN_H
