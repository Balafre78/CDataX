#ifndef CDATAX_CDATAFRAME_H
#define CDATAX_CDATAFRAME_H

#include <stdio.h>
#include <stdlib.h>

#define REALOC_SIZE 256

struct column {
    char *title;
    int physical_size;
    int size;
    int *data;
};

typedef struct column Column;

struct cdataframe {
    Column **columns;
    int size;
};

typedef struct cdataframe CDataframe;

/**
  * @brief : Create a column
  * @param title : Column title
  * @return : Pointer to created column or NULL pointer on failure
  */
Column *create_column(char *title);

/**
  * @brief : Add a new value to a column
  * @param col : Pointer to a column
  * @param value: The value to be added
  * @return : 1 if the value is added 0 otherwise
  */
int insert_value(Column *col, int value);

/**
  * @brief : Free allocated memory
  * @param col : Pointer to a column
  */
void delete_column(Column **col);

/**
  * @brief : Print a column content
  * @param col : Pointer to a column
  */
void print_col(Column *col);

/**
 * @brief : Get the value of the column index given
 * @param col : The pointer to the column
 * @param idx : The index to look for
 * @return : that value
 */
int get_value(Column *col, int index);

/**
 * @brief : Get the number of occurrences of value under the one given
 * @param col : Pointer to a column
 * @param x : The value to compare
 * @return : Number of occurrences
 * */
int get_occurrences_inferior(Column *col, int x);

/**
 * @brief : Get the number of occurrences of value above the one given
 * @param col : Pointer to a column
 * @param x : The value to compare
 * @return : Number of occurrences
 * */
int get_occurrences_superior(Column *col, int x);

/**
 * @brief : Get the number of occurrences of value equals to the one given
 * @param col : Pointer to a column
 * @param x : The value to compare
 * @return : Number of occurrences
 * */
int get_occurrences_equal(Column *col, int x);

/**
 * @brief : Create an empty Cdataframe
 * @return : Pointer to the newly created Cdataframe or the NULL pointer if it cannot allocate memory
 */
CDataframe *create_cdataframe();

/*NB: L'implémenter aussi en dur*/
/**
 * @brief : Write all data to a new cdf (from user input)
 * @param cdf : Cdataframe pointer (should be empty)
 * @return : 1 if there's an error with memory allocation, 0 else
 */
int write(CDataframe *cdf);

/**
 * @brief : Print all data to an existing cdf
 * @param cdf : Cdataframe pointer
 */
void print_all(CDataframe *cdf);

/**
 * @brief : Print all lines from a start to an end from an existing cdf
 * @param cdf : Cdataframe pointer
 * @param from : Starting point
 * @param to : Ending point (excluded)
 * @return : 2 if there's an error with the section selected, 0 else
 */
int print_lines(CDataframe *cdf, int from, int to);

/**
 * @brief : Print all columns from a start to an end from an existing cdf
 * @param cdf : Cdataframe pointer
 * @param from : Starting point
 * @param to : Ending point (excluded)
 * @return : 2 if there's an error with the section selected, 0 else
 */
int print_columns(CDataframe *cdf, int from, int to);

/**
 * @brief : Add a new line to an existing cdf
 * @param cdf : Cdataframe pointer
 * @param values : Tab of integers to insert to the new line
 * @param size : Size of the previous tab, should be the exact same size as the cdf size
 * @return : 3 if the cdf is empty, 2 if it's the size, 1 if it's the memory allocation, 0 else
 */
int add_newline(CDataframe *cdf, int *values, int size);

/**
 * @brief : Add a new line to an existing cdf
 * @param cdf : Cdataframe pointer
 * @param values : Tab of integers to insert to the new line
 * @param size : Size of the previous tab, should be the exact same size as the cdf size
 * @return : 2 if there's an error with the size, 1 if there's an error with memory allocation, 0 else
 */
int add_newcolumn(CDataframe *cdf, int *values, int size, char *title);

void del_line(CDataframe *cdf, int line);

void del_column(CDataframe *cdf, int column);

/**
 * @brief : Rename one column name
 * @param cdf : Cdataframe pointer
 * @return 0 on sucess, 2 if there's an error the column parameter
 */
int rename_column(CDataframe *cdf, int column, char *newTitle);

void find_in(CDataframe *cdf, int var);

/**
 * @brief : Print all column names
 * @param cdf : Cdataframe pointer
 */
void print_columns_names(CDataframe *cdf);

void get_var(CDataframe *cdf, int line, int column);

/**
 * @brief : Get the amount of lines
 * @param cdf : Cdataframe pointer
 * @return : an int
 */
int get_lines_amount(CDataframe *cdf);

/**
 * @brief : Get the amount of lines
 * @param cdf : Cdataframe pointer
 * @return : an int
 */
int get_columns_amount(CDataframe *cdf);

/**
 * @brief : Get the occurences of a var in a Cdataframe
 * @param cdf : Cdataframe pointer
 * @retunr : the amount of occurences
 */
int get_occurrences(CDataframe *cdf, int var);

/**
 * @brief : Get the occurences of a var superior to a var in a Cdataframe
 * @param cdf : Cdataframe pointer
 * @retunr : the amount of occurences
 */
int get_superior_occurrences(CDataframe *cdf, int var);

/**
 * @brief : Get the occurences of a var inferior to a var in a Cdataframe
 * @param cdf : Cdataframe pointer
 * @retunr : the amount of occurences
 */
int get_inferior_occurrences(CDataframe *cdf, int var);

#endif //CDATAX_CDATAFRAME_H
