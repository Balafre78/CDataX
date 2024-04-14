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

typedef struct column *CDataframe;

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
void write_cdataframe(CDataframe *cDataframe);

void read_cdataframe(CDataframe *cDataframe);

void read_cdataframe_lines(CDataframe *cDataframe, int from, int to);

void read_cdataframe_columns(CDataframe *cDataframe, int from, int to);

void add_cdataframe_newline(CDataframe *cDataframe);

void add_cdataframe_newcolumn(CDataframe *cDataframe);

void del_cdataframe_line(CDataframe *cDataframe, int line);

void del_cdataframe_column(CDataframe *cDataframe, int column);

void rename_cdataframe_column(CDataframe *cDataframe, int column);

void findin_cdataframe(CDataframe *cDataframe, int var);

void readname_cdataframe_allcolumns(CDataframe *cDataframe);

void get_cdataframe_var(CDataframe *cDataframe, int line, int column);

int get_cdataframe_lines_amount(CDataframe *cDataframe);

int get_cdataframe_columns_amount(CDataframe *cDataframe);

int get_cdataframe_occurrences_equal(CDataframe *cDataframe, int var);

int get_cdataframe_occurrences_superior(CDataframe *cDataframe, int var);

int get_cdataframe_occurrences_inferior(CDataframe *cDataframe, int var);

#endif //CDATAX_CDATAFRAME_H
