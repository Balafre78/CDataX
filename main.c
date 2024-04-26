#include <stdio.h>
#include "column.h"

/**
 * @brief Debug the column by dumping all infos
 * @param column : Pointer to a column
 */
void printColumnInfo(Column *column);


int main() {
    Column *mycol = create_column(INT, "My column");
    int a = 42, c = 2;
    insert_value(mycol, &a);
    //insert_value(mycol, NULL);
    insert_value(mycol, &c);
    printColumnInfo(mycol);
    return 0;
}


void printColumnInfo(Column *column) {
    printf("Column %p:\n - title = \"%s\";\n - max_size = %d;\n - size = %d;\n - enum_type = %d\n - data = 0x%p;\n - First value = %d\n",
           column, column->title, column->max_size, column->size, column->column_type, column->data, column->data[0]->int_value);
}