#ifndef CDATAX_CDATAFRAME_H
#define CDATAX_CDATAFRAME_H

#include <stdio.h>
#include <stdlib.h>
#include "llc.h"
#include "column.h"

typedef struct cadataframe {
    int size;
    int colsize;
    list *data;

} CDataframe;


/**
 * @brief Create an empty Cdataframe (no data provided only the an empty list is created)
 * @return Pointer to the newly created Cdataframe or the NULL pointer if it cannot allocate memory
 */
CDataframe *create_empty_cdataframe();

/**
 * @brief Create an empty Cdataframe (no data provided by all columns defined)
 * @param cdftype list of types that will be used
 * @param colNames list og names that will be used
 * @param size size of the above list
 * @return Pointer to the newly created Cdataframe or the NULL pointer if it cannot allocate memory
 */
CDataframe *create_cdataframe(Enum_type *cdfTypes, char** colNames, int size);

/**
* @brief: Column deletion
* @param cdf Pointer to the CDataframe to delete
*/
void delete_cdataframe(CDataframe **cdf);

// /**
// * @brief: Number of columns
// * @param1: Point to the CDataframe
// * @return: Number of columns in the CDataframe
// */
// int get_cdataframe_cols_size(CDataframe *cdf);

//TODO: L'implémenter aussi en dur
/**
 * @brief Write all data to a new Cdataframe (from user input)
 * @param cdf Cdataframe pointer (should be empty)
 * @return 1 if there's an error with memory allocation, 0 else
 */
int write(CDataframe *cdf);

/**
 * @brief Print all data of an existing Cdataframe
 * @param cdf Cdataframe pointer
 * @return 1 with internal memory allocation, 0 else
 */
int print_all(CDataframe *cdf);

/**
 * @brief Print all column names
 * @param cdf Cdataframe pointer
 * @param from Starting line
 * @param to Ending line (excluded)
 * @return 2 if there's an error with the section selected, 0 else
 */
int print_columns_names_partial(CDataframe *cdf, int from, int to);

/**
 * @brief Print all column names
 * @param cdf Cdataframe pointer
 */
void print_columns_names(CDataframe *cdf);

/**
 * @brief Print all lines from a start to an end from an existing Cdataframe
 * @param cdf Cdataframe pointer
 * @param from Starting line
 * @param to Ending line (excluded)
 * @return 2 if there's an error with the section selected, 1 with internal memory allocation, 0 else
 */
int print_lines(CDataframe *cdf, int from, int to);

/**
 * @brief Print all columns from a start to an end from an existing Cdataframe
 * @param cdf Cdataframe pointer
 * @param from Starting column
 * @param to Ending column (excluded)
 * @return 2 if there's an error with the section selected, 1 with internal memory allocation, 0 else
 */
int print_columns(CDataframe *cdf, int from, int to);

/**
 * @brief Add a new line to an existing Cdataframe
 * @param cdf Cdataframe pointer
 * @param values Tab of Col_type to insert to the new line
 * @param size Size of the previous tab, should be the exact same size as the Cdataframe size
 * @return 3 if the cdf is empty (no info on columns types), 2 if it's the size, 1 if it's the memory allocation, 0 else
 */
int add_newline(CDataframe *cdf, Col_type *values, int size);

/**
 * @brief Add a new line to an existing Cdataframe
 * @param cdf Cdataframe pointer
 * @param type Type of value to insert
 * @param values Tab of Col_type to insert to the new line
 * @param size Size of the previous tab, should be the exact same size as the Cdataframe size
 * @return 2 if it's the size, 1 if there's an error with memory allocation, 0 else
 */
int add_newcolumn(CDataframe *cdf, Enum_type type, Col_type *values, int size, char *title);

/**
 * @brief Query column by his name
 * @param cdf Cdataframe pointer
 * @param title name of the column
 * @return the pointer to that column (may be NULL if nothing is found)
 */
Column *query_column_by_name(CDataframe *cdf, char *title);


/**
 * @brief Delete one line of the Cdataframe
 * @param cdf Cdataframe pointer
 * @param line line to remove
 * @return 0 on success, 2 if there's an error the line parameter
 */
int del_line(CDataframe *cdf, int line);

/**
 * @brief Delete one column of the Cdataframe, query by names
 * @param cdf Cdataframe pointer
 * @param col_title column name to remove
 * @return 0 on success, 2 if there's an error the col_name parameter
 */
int del_column(CDataframe *cdf, char *col_title);

/**
 * @brief Rename one column name
 * @param cdf Cdataframe pointer
 * @param col_title column name to rename
 * @param newTitle new title of the column
 * @return 0 on success, 2 if there's an error the col_title parameter
 */
int rename_column(CDataframe *cdf, char *col_title, char *newTitle);

/**
 * @brief Give a pointer to the first cell with the matching value
 * @param cdf Cdataframe pointer
 * @param var value to find
 * @return A pointer to the value (first found by column) if it's found, the NULL pointer else
 */
Col_type *find_in(CDataframe *cdf, Col_type var);

/**
 * @brief Give a pointer to the value at the line and column wanted
 * @param cdf Cdataframe pointer
 * @param line the line index
 * @param col_title column name to find in
 * @return A pointer to the value
 */
Col_type *get_var(CDataframe *cdf, char *col_title, int line);

/**
 * @brief Get the amount of lines
 * @param cdf Cdataframe pointer
 * @return number of lines
 */
int get_lines_amount(CDataframe *cdf);

/**
 * @brief Get the amount of lines
 * @param cdf Cdataframe pointer
 * @return number of columns
 */
int get_columns_amount(CDataframe *cdf);

/**
 * @brief Get the occurrences of a var in a Cdataframe
 * @param cdf Cdataframe pointer
 * @param var variable to compare
 * @retunr number of occurrences
 */
int get_occurrences(CDataframe *cdf, Col_type var);

/**
 * @brief Get the occurrences of a var superior to a var in a Cdataframe
 * @param cdf Cdataframe pointer
 * @param var variable to compare
 * @retunr number of occurrences
 */
int get_superior_occurrences(CDataframe *cdf, Col_type var);

/**
 * @brief Get the occurrences of a var inferior to a var in a Cdataframe
 * @param cdf Cdataframe pointer
 * @param var variable to compare
 * @retunr number of occurrences
 */
int get_inferior_occurrences(CDataframe *cdf, Col_type var);

#endif //CDATAX_CDATAFRAME_H
