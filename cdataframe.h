/* CDataX
 * Pierre CAMELERI
 * Raphaël GUIGNOLLE
 * cdataframe.c -
 * Function to manage a CDataframe
 *   - Creation
 *   - Destruction
 *   - Printing
 *   - Modification (addition)
 *   - Getters
 *   - Setters
 *   - Sorting
 *   - Interaction with IO
 * Also include some internal function (not declared in header).
 */
#ifndef CDATAX_CDATAFRAME_H
#define CDATAX_CDATAFRAME_H

#include "llc.h"
#include "column.h"

// Should be inferrior as an int
#define USER_INPUT_SIZE 255

#define MAX_COL 255

typedef struct cadataframe {
    indexation size;
    indexation colsize;
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
CDataframe *create_cdataframe(Enum_type *cdfTypes, char **colNames, indexation size);

/**
* @brief: Column deletion
* @param cdf Pointer to the CDataframe to delete
*/
void delete_cdataframe(CDataframe **cdf);

//TODO: L'implémenter aussi en dur
/**
 * @brief Write all data to a new Cdataframe (from user input)
 * @param cdf Cdataframe pointer (should be empty)
 * @warning since, write interracts with IO, it doesn't check memeory allocation.
 */
void write(CDataframe **cdf);

/**
 * @brief Print all data of an existing Cdataframe
 * @param cdf Cdataframe pointer
 * @param ref_col Reference column used for sort, NULL if object is prefered
 * @return 3, if a ref_col is mentioned but the col isn't sorted
 *         2 if ref_col is unknown
 *         1 with internal memory allocation,
 *         0 else
 */
int print_all(CDataframe *cdf, char *ref_col);

/**
 * @brief Print all column names
 * @param cdf Cdataframe pointer
 * @param from Starting line
 * @param to Ending line (excluded)
 * @return 2 if there's an error with the section selected, 0 else
 */
int print_columns_names_partial(CDataframe *cdf, indexation from, indexation to);

/**
 * @brief Print all column names
 * @param cdf Cdataframe pointer
 */
void print_columns_names(CDataframe *cdf);

/**
 * @brief Print all lines from a start to an end from an existing Cdataframe
 * @param cdf Cdataframe pointer
 * @param ref_col Reference column used for sort, NULL if object is prefered
 * @param from Starting line
 * @param to Ending line (excluded)
 * @return 3, if a ref_col is mentioned but the col isn't sorted
 *         2 if there's an error with the section selected or name of ref_col,
 *         1 with internal memory allocation,
 *         0 else
 */
int print_lines(CDataframe *cdf, char *ref_col, indexation from, indexation to);

/**
 * @brief Print all columns from a start to an end from an existing Cdataframe
 * @param cdf Cdataframe pointer
 * @param ref_col Reference column used for sort, NULL if object is prefered
 * @param from Starting column
 * @param to Ending column (excluded)
 * @return 3, if a ref_col is mentioned but the col isn't sorted
 *         2 if there's an error with the section selected or name of ref_col,
 *         1 with internal memory allocation,
 *         0 else
 */
int print_columns(CDataframe *cdf, char *ref_col, indexation from, indexation to);

/**
 * @brief Add a new line to an existing Cdataframe
 * @param cdf Cdataframe pointer
 * @param values Tab of Col_type to insert to the new line
 * @param size Size of the previous tab, should be the exact same size as the Cdataframe size
 * @return 3 if the cdf is empty (no info on columns types),
 *         2 if it's the size,
 *         1 if it's the memory allocation,
 *         0 else
 */
int add_newline(CDataframe *cdf, Col_type *values, indexation size);

/**
 * @brief Add a new line to an existing Cdataframe
 * @param cdf Cdataframe pointer
 * @param type Type of value to insert
 * @param values Tab of Col_type to insert to the new line
 * @param size Size of the previous tab, should be the exact same size as the Cdataframe colsize
 * @return 2 if it's the size,
 *         1 if there's an error with memory allocation,
 *         0 else
 */
int add_newcolumn(CDataframe *cdf, Enum_type type, Col_type *values, indexation size, char *title);

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
 * @warning line represent object, no sorting
 * @return 0 on success, 2 if there's an error the line parameter
 */
int del_line(CDataframe *cdf, indexation line);

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
 * @return 0 on success,
 *         1 if there's an arror with the memory allocation
 *         2 if there's an error the col_title parameter
 */
int rename_column(CDataframe *cdf, char *col_title, char *newTitle);

/**
 * @brief Give a pointer to the first cell with the matching value
 * @param cdf Cdataframe pointer
 * @param var value to find
 * @return A pointer to the value (first found by column) if it's found, the NULL pointer else
 */
Col_type *find_in(CDataframe *cdf, void *var);

/**
 * @brief Give a pointer to the value at the line and column wanted
 * @param cdf Cdataframe pointer
 * @param line the line index
 * @param col_title column name to find in
 * @warning line is ordered according to objects (no sorting)
 * @return A pointer to the value if it's found, the NULL pointer else
 */
Col_type *get_var(CDataframe *cdf, char *col_title, indexation line);

/**
 * @brief Get the amount of lines
 * @param cdf Cdataframe pointer
 * @return number of lines
 */
indexation get_lines_amount(CDataframe *cdf);

/**
 * @brief Get the amount of lines
 * @param cdf Cdataframe pointer
 * @return number of columns
 */
indexation get_columns_amount(CDataframe *cdf);

/**
 * @brief Get the occurrences of a var in a Cdataframe
 * @param cdf Cdataframe pointer
 * @param var variable to compare
 * @return number of occurrences
 */
int get_occurrences(CDataframe *cdf, void *var);

/**
 * @brief Get the occurrences of a var superior to a var in a Cdataframe
 * @param cdf Cdataframe pointer
 * @param var variable to compare
 * @return number of occurrences
 */
int get_superior_occurrences(CDataframe *cdf, void *var);

/**
 * @brief Get the occurrences of a var inferior to a var in a Cdataframe
 * @param cdf Cdataframe pointer
 * @param var variable to compare
 * @return number of occurrences
 */
int get_inferior_occurrences(CDataframe *cdf, void *var);

/**
 * @brief Sort 1 column
 * @param cdf Cdataframe pointer
 * @param col_title name of the column to align sort on
 * @param sort_dir direction to sort either ASC or DESC is accepted
 * @warning fail if col_title is not found or sort_dir is incorrect !
 */
void sorting_column(CDataframe *cdf, char *col_title, int sort_dir);

/**
 * @brief Sort all columns
 * @param cdf Cdataframe pointer
 * @param sort_dir direction to sort either ASC or DESC is accepted
 * @warning fail if sort_dir is incorrect !
 */
void sort_all_columns(CDataframe *cdf, int sort_dir);


/**
* @brief: Export into a csvfile
* @param1: Pointer to the CDataframe
* @param2: csv filename where export file, if the file exists,
* it will be overwritten
*/
void save_into_csv(CDataframe *cdf, char *file_name);

/**
* @brief: Create a CDataframe from csvfile
* @param1: CSV filename
* @param2: Array of types
* @param3: Size of array in param2
*/
CDataframe *load_from_csv(char *file_name, Enum_type *dftype, int size);

#endif //CDATAX_CDATAFRAME_H
