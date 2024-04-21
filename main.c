#include <stdio.h>
#include "cdataframe.h"

/**
 * @brief Debug the column by dumping all infos
 * @param column : Pointer to a column
 */
void printColumnInfo(Column *column);

/**
 * @brief Series of column tests
*/
void test_column();

/**
 * @brief Series of cdataframe tests
*/
void test_cdataframe();


int main() {
    printf("Testing Columns");
    test_column();
    printf("Testing CDataframe");
    test_cdataframe();
    return 0;
}


void printColumnInfo(Column *column) {
    printf("Column 0x%p:\n - title = \"%s\";\n - physical_size = %d;\n - size = %d;\n - columns address = 0x%p;\n",
           column, column->title, column->physical_size, column->size, column->data);
}

void test_column() {
    Column *mycol = create_column("My column");
    printColumnInfo(mycol);

    int val = 5;
    if (insert_value(mycol, val))
        printf("Value added successfully to my column\n");
    else
        printf("Error adding value to my column\n");

    insert_value(mycol, 4);
    insert_value(mycol, 1);
    print_col(mycol);

    int x = 4;
    printf("Occurrences equals to %d : %d", x, get_occurrences_equal(mycol, 4));
    printf("Occurrences inferior to %d : %d", x, get_occurrences_inferior(mycol, 4));
    printf("Occurrences superior to %d : %d", x, get_occurrences_superior(mycol, 4));

    delete_value_at_index(mycol, 2);

    delete_column(&mycol);
    printf("0x%p\n", mycol);
}

void test_cdataframe() {
    // Creation functions
    CDataframe *cdf = create_cdataframe();
    write(cdf);

    // Printing functions
    printf("Header :");
    print_columns_names(cdf);
    printf("All data :");
    print_all(cdf);
    printf("First column");
    print_columns(cdf, 0, 1);
    printf("First line");
    print_lines(cdf, 0, 1);

    // Analysis functions
    printf("Columns : %d", get_columns_amount(cdf));
    printf("Lines : %d", get_lines_amount(cdf));
    int x = 4;
    printf("Cells equals to %d : %d", x, get_occurrences(cdf, x));
    printf("Cells inferior to %d : %d", x, get_inferior_occurrences(cdf, x));
    printf("Cells superior to %d : %d", x, get_superior_occurrences(cdf, x));

    // Operation functions
    int vals[] = {1, 2, 3, 4, 5, 6};
    add_newcolumn(cdf, vals, 6, "Index");

    int li[] = {15, 23, 47};
    add_newline(cdf, li, 3);

    del_line(cdf, 0);
    del_column(cdf, 0);
    rename_column(cdf, 1, "Column 1");

    int *fifteen_cell = find_in(cdf, 15);
    int *selected_cell = get_var(cdf, 3, 6);

}