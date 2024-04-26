#include <stdio.h>
#include "column.h"

/**
 * @brief Debug the column by dumping all infos
 * @param column : Pointer to a column
 */
void printColumnInfo(Column *column);


int main() {
    /*Column *mycol = create_column(INT, "My column");
    int a = 42, c = 2;
    insert_value(mycol, &a);
    //insert_value(mycol, NULL);
    insert_value(mycol, &c);
    printColumnInfo(mycol);
    return 0;*/

    char buff[10];
    Column *mycol = create_column(INT, "My column");
    int a = 52, b = 44, c = 15, d = 18;
    insert_value(mycol, &a);
    insert_value(mycol, &b);
    insert_value(mycol, NULL);
    insert_value(mycol, &c);
    insert_value(mycol, &d);
    //convert_value(mycol, 3, buff, 1);
    //printf("%s \n", buff);
    print_col(mycol);
    Col_type seuil;
    seuil.int_value = 20;
    printf("Less than 20 : %d", get_occurrences_superior(mycol, &seuil));
    delete_column(&mycol);
}


void printColumnInfo(Column *column) {
    printf("Column %p:\n - title = \"%s\";\n - max_size = %d;\n - size = %d;\n - enum_type = %d\n - data = 0x%p;\n - First value = %d\n",
           column, column->title, column->max_size, column->size, column->column_type, column->data, column->data[0]->int_value);
}