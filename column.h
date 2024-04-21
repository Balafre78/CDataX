#ifndef CDATAX_COLUMN_H
#define CDATAX_COLUMN_H

#include <stdlib.h>
#include <stdio.h>

#define REALOC_SIZE 256

struct column {
    char *title;
    int physical_size;
    int size;
    int *data;
};

typedef struct column Column;

/**
  * @brief Create a column
  * @param title Column title
  * @return Pointer to created column or NULL pointer on failure
  */
Column *create_column(char *title);

/**
  * @brief Add a new value to a column
  * @param col Pointer to a column
  * @param value The value to be added
  * @return 1 if the value is added 0 otherwise
  */
int insert_value(Column *col, int value);

/**
  * @brief Free allocated memory
  * @param col Pointer to a column
  */
void delete_column(Column **col);

/**
 * @brief Delete last value of the column
 * @param col Pointer to a column
 * @param index Index to remove from the list
 * @return 1 if the index is invalid, 0 else
 */
int delete_value_at_index(Column *col, int index);

/**
  * @brief Print a column content
  * @param col Pointer to a column
  */
void print_col(Column *col);

/**
 * @brief Get the value of the column index given
 * @param col The pointer to the column
 * @param idx The index to look for
 * @return that value
 * @warning exit the program if invalid index provided
 */
int get_value(Column *col, int index);

/**
 * @brief Get the number of occurrences of value under the one given
 * @param col Pointer to a column
 * @param x The value to compare
 * @return Number of occurrences
 * */
int get_occurrences_inferior(Column *col, int x);

/**
 * @brief Get the number of occurrences of value above the one given
 * @param col Pointer to a column
 * @param x The value to compare
 * @return Number of occurrences
 * */
int get_occurrences_superior(Column *col, int x);

/**
 * @brief Get the number of occurrences of value equals to the one given
 * @param col Pointer to a column
 * @param x The value to compare
 * @return Number of occurrences
 * */
int get_occurrences_equal(Column *col, int x);

#endif //CDATAX_COLUMN_H
