#include <stdio.h>
#include "column.h"
#include "cdataframe.h"

#define LINE 2
#define COLUMN 4

/**
 * @brief Debug the column by dumping all infos
 * @param column : Pointer to a column
 */
void printColumnInfo(Column *column);


int main() {
    /*Column *mycol = create_column(INT, "My column");
    int a = 42, c = 2;
    append_value(mycol, &a);
    //insert_value(mycol, NULL);
    append_value(mycol, &c);
    printColumnInfo(mycol);*/

    char buff[10];
    /*Column *mycol = create_column(INT, "My column");
    int a = 52, b = 44, c = 15, d = 18;
    append_value(mycol, &a);
    append_value(mycol, &b);
    append_value(mycol, NULL);
    append_value(mycol, &c);
    append_value(mycol, &d);
    //convert_value(mycol, 3, buff, 1);
    //printf("%s \n", buff);
    print_col_raw(mycol);
    Col_type seuil;
    seuil.int_value = 20;
    printf("Less than 20 : %d\n", get_occurrences_superior_raw(mycol, &seuil));
    delete_column(&mycol);*/

    /*Column *withindex = create_column(INT, "sorted column");
    int va = 52;
    int vb = 18;
    int vc = 5;
    int vd = 44;
    int ve = 2;
    int vf = 15;
    int vg = 15;
    int vh = 15;
    int vi = 15;
    append_value(withindex, &va);
    append_value(withindex, &vb);
    append_value(withindex, &vc);
    append_value(withindex, &vd);
    append_value(withindex, &ve);
    append_value(withindex, &vf);
    append_value(withindex, &vg);
    append_value(withindex, &vh);
    append_value(withindex, &vi);

    sort(withindex,ASC);

    printf("Column content before sorting : \n");
    print_col_raw(withindex);
    printf("Column content after sorting : \n");
    print_col_by_index(withindex);
    sort(withindex,DESC);
    print_col_by_index(withindex);
    Col_type Var_col;
    Var_col.int_value = 15;
    int answer = 44;
    printf("Is 44 in ? : %d\n", search_value_in_column(withindex, &answer));
    printf("Amount of 15 : %d\n", get_occurrences_equal_by_index(withindex, &Var_col));
    printf("Check index : %d\n", check_index(withindex));
    int vz = 0;
    append_value(withindex, &vz);
    printf("Checking unvalid index : %d\n", check_index(withindex));
    erase_index(withindex);
    printf("Erease index : %d\n", check_index(withindex));*/


    Enum_type prefabTypes[3] = {CHAR, INT, INT};
    Col_type **prefabValuePtr = malloc(LINE * sizeof(Col_type *));
    for (int i = 0; i < LINE; i++)
        prefabValuePtr[i] = malloc(COLUMN * sizeof(Col_type *));
    prefabValuePtr[0][0].char_value = 'a';
    prefabValuePtr[0][1].int_value = 1024;
    prefabValuePtr[0][2].int_value = 2048;
    char *prefabNames[3] = {"Col 1", "Col 2", "Col 3"};
    CDataframe *cdf = create_cdataframe(prefabTypes, prefabNames, 3);
    printf("add with %d\n", add_newline(cdf, prefabValuePtr[0], 3));

    lnode *node = cdf->data->head;
    for (int i = 0; i < 3; i++) {
        convert_value(node->data, 0, buff, 10);
        printf("%s\t@ %s\n", buff, node->data->title);
        node = get_next_node(cdf->data, node);
    }

    printf("printed with %d!\n", print_lines(cdf, NULL, 0, 1));

    prefabValuePtr[1][0].char_value = 'b';
    prefabValuePtr[1][1].int_value = 495;
    prefabValuePtr[1][2].int_value = -200;
    add_newline(cdf, prefabValuePtr[1], 3);
    print_lines(cdf, NULL, 0, 2);
    print_lines(cdf, NULL, 1, 2);
    print_columns(cdf, NULL, 1, 3);
    print_all(cdf, NULL);

    //printf("%s\n", cdf->data->head->data->title);
    rename_column(cdf, "Col 1", "newCol1");
    print_columns(cdf, NULL, 0, 1);
    printf("Cdf of %lld lines and %lld columns\n", get_lines_amount(cdf), get_columns_amount(cdf));

    printf("\n\nINDEXING TEST SUITE\n");

    printf("failed with 3=%d\n", print_columns(cdf, "Col 3", 2, 3));
    printf("\nBefore and efter sorting\n");
    print_columns(cdf, NULL, 2, 3);
    sorting_column(cdf, "Col 3", ASC);
    print_all(cdf, "Col 3");
    sort_all_columns(cdf, ASC);

    char mystery_char = 'b';
    int mystery_int = 495;
    printf("find a 'b' @%p\n", find_in(cdf, &mystery_char));
    printf("find a 495 @%p\n", find_in(cdf, &mystery_int));
    mystery_int = 15;
    printf("don't find a 15 @%p\n", find_in(cdf, &mystery_int));

    //  Test deletation
    prefabValuePtr[0][3].string_value = "str1";
    prefabValuePtr[1][3].string_value = "str2";
    Col_type tabDELCOL[2] = {prefabValuePtr[0][3], prefabValuePtr[1][3]};
    add_newcolumn(cdf, STRING, tabDELCOL, 2, "DEL COL");
    print_all(cdf, NULL);
    del_column(cdf, "DEL COL");
    print_all(cdf, NULL);
    del_line(cdf, 1);
    print_all(cdf, NULL);
    add_newline(cdf, prefabValuePtr[1], 3);
    print_all(cdf, NULL);

    CDataframe *fromwrite;
    write(&fromwrite);
    print_all(fromwrite, NULL);






    for (int i = 0; i < LINE; i++)
        free(prefabValuePtr[i]);
    free(prefabValuePtr);
    return 0;

}


void printColumnInfo(Column *column) {
    printf("Column %p:\n - title = \"%s\";\n - max_size = %d;\n - size = %d;\n - enum_type = %d\n - data = 0x%p;\n - First value = %d\n",
           column, column->title, column->max_size, column->size, column->column_type, column->data,
           column->data[0]->int_value);
}