#include <stdio.h>
#include "CDataFrame.h"

/**
 * @brief Debug the column by dumping all infos
 * @param column : Pointer to a column
 */
void printColumnInfo(Column *column);

/**
 * @brief Series of test
*/
void test();

int main() {
    test();
    return 0;
}


void printColumnInfo(Column *column) {
    printf("Column 0x%p:\n - title = \"%s\";\n - physical_size = %d;\n - size = %d;\n - columns address = 0x%p;\n",
           column, column->title, column->physical_size, column->size, column->data);
}

void test() {
    Column *mycol = create_column("My column");
    printColumnInfo(mycol);
    int val = 5;
    if (insert_value(mycol, val))
        printf("Value added successfully to my column\n");
    else
        printf("Error adding value to my column\n");
    printColumnInfo(mycol);
    delete_column(&mycol);
    printf("0x%p\n", mycol);

    Column *secondcol = create_column("My column");
    insert_value(secondcol, 52);
    insert_value(secondcol, 44);
    insert_value(secondcol, 15);
    print_col(secondcol);

    printf("Value@1  :%d\n", get_value(secondcol, 1));
    // printf("Value@48 :%d\n", get_value(secondcol, 48));

    insert_value(secondcol, 72);
    insert_value(secondcol, 64);
    insert_value(secondcol, 3);

    Column *emptycol = create_column("Empty column");

    //printf("Inferior to 5 (mycol) : %d\n", get_occurrences_inferior(emptycol, 5));
    printf("Inferior to 50 (mycol) : %d\n", get_occurrences_inferior(secondcol, 50));
    printf("Superior to 50 (mycol) : %d\n", get_occurrences_superior(secondcol, 50));
    printf("Equal to 3 (mycol) : %d\n", get_occurrences_equal(secondcol, 3));

    CDataframe *cdf = create_cdataframe();
    print_all(cdf);

}