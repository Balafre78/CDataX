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
    Column *data;
    int size;
};

typedef struct cdataframe Cdataframe;

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


Cdataframe *create_cdataframe();

/*NB: L'implémenter aussi en dur ...*/
void write_cdataframe(Cdataframe *cdf);

void read_cdataframe(Cdataframe *cdf);

void read_cdataframe_lines(Cdataframe *cdf, int from, int to);

void read_cdataframe_columns(Cdataframe *cdf, int from, int to);

void add_cdataframe_newline(Cdataframe *cdf);

void add_cdataframe_newcolumn(Cdataframe *cdf);

void del_cdataframe_line(Cdataframe *cdf, int line);

void del_cdataframe_column(Cdataframe *cdf, int column);

void rename_cdataframe_column(Cdataframe *cdf, int column);

void findin_cdataframe(Cdataframe *cdf, int var);

void readname_cdataframe_allcolumns(Cdataframe *cdf);

void get_cdataframe_var(Cdataframe *cdf, int line, int column);

int get_cdataframe_lines_amount(Cdataframe *cdf);

int get_cdataframe_columns_amount(Cdataframe *cdf);

int get_cdataframe_occurrences_equal(Cdataframe *cdf, int var);

int get_cdataframe_occurrences_superior(Cdataframe *cdf, int var);

int get_cdataframe_occurrences_inferior(Cdataframe *cdf, int var);

#endif //CDATAX_CDATAFRAME_H
