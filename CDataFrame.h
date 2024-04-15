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
  * @return : Pointer to created column
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


CDataframe *create_cdataframe();

/*NB: L'implémenter aussi en dur ...*/
void write(CDataframe *cdf);

void print_all(CDataframe *cdf);

void print_lines(CDataframe *cdf, int from, int to);

void print_columns(CDataframe *cdf, int from, int to);

void add_newline(CDataframe *cdf);

void add_newcolumn(CDataframe *cdf);

void del_line(CDataframe *cdf, int line);

void del_column(CDataframe *cdf, int column);

void rename_column(CDataframe *cdf, int column, char *newTitle);

void find_in(CDataframe *cdf, int var);

void print_columns_names(CDataframe *cdf);

void get_var(CDataframe *cdf, int line, int column);

int get_lines_amount(CDataframe *cdf);

int get_columns_amount(CDataframe *cdf);

int get_occurrences(CDataframe *cdf, int var);

int get_superior_occurrences(CDataframe *cdf, int var);

int get_inferior_occurrences(CDataframe *cdf, int var);

#endif //CDATAX_CDATAFRAME_H
