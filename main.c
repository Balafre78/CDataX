#include <stdio.h>
#include "column.h"
#include "cdataframe.h"

#define LINE 2
#define COLUMN 4

void next_section(char *title) {
    printf("\n\n\n");
    printf("===========================================================\n");
    printf("%s\n\n\n", title);
}

/*
 * *********************************
 *           TEST SUITE
 * *********************************
 */

void test_column_basics() {
    Column *mycol = create_column(INT, "My column");
    int a = 42, c = 2;
    append_value(mycol, &a);
    append_value(mycol, NULL);
    append_value(mycol, &c);
    printf("Column = %p\n", mycol);
    printf("Title = \"%s\"\n", mycol->title);
    printf("Max_size = %lld\n", mycol->max_size);
    printf("Size = %lld\n", mycol->size);
    printf("Enum_type = %d\n", mycol->column_type);
    printf("Data = %p\n", mycol->data);
    printf("First value (int) = %d\n", mycol->data[0]->int_value);
    printf("NULL value = %p\n", mycol->data[1]->struct_value);
    int *ptr = (int *) get_value(mycol, 1);
    *ptr = 42;
    printf("NULL value has been modify to %d\n", mycol->data[1]->int_value);
    delete_column(&mycol);
    printf("After deletation : \n");
    printf("Column = %p\n", mycol);
}

void test_column_occurences_raw() {
    char buff[10];
    Column *mycol = create_column(INT, "My column");
    int a = 52, b = 44, c = 15, d = 18;
    append_value(mycol, &a);
    append_value(mycol, &b);
    append_value(mycol, NULL);
    append_value(mycol, &c);
    append_value(mycol, &d);
    convert_value(mycol, 3, buff, 10);
    printf("Buffer contains row 3: %s \n", buff);
    int rc = convert_value(mycol, 2, buff, 1);
    printf("Buffer contains nothing since it's small: %s\nExit with %d\n", buff, rc);
    print_col_raw(mycol);
    int seuil = 20;
    printf("Less than 20 : %d\n", get_occurrences_inferior_raw(mycol, &seuil));
    printf("Greater than 20 : %d\n", get_occurrences_superior_raw(mycol, &seuil));
    printf("Equals to 20 : %d\n", get_occurrences_equal_raw(mycol, &seuil));
    delete_column(&mycol);
}


void test_column_sorting() {
    Column *withindex = create_column(INT, "sorted column");
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

    update_index(withindex, ASC);

    printf("Column content before sorting : \n");
    print_col_raw(withindex);
    printf("\nColumn content after sorting : \n");
    print_col_by_index(withindex);
    printf("\nColumn content after reverse sorting : \n");
    update_index(withindex, DESC);
    print_col_by_index(withindex);

    delete_column(&withindex);
}

void test_column_sorting_operations() {
    Column *withindex = create_column(INT, "sorted column");
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

    sort(withindex, ASC);

    int value = 15;
    int answer = 44;
    printf("Is 44 in withindex : %d\n", search_value_in_column(withindex, &answer));
    printf("Amount of 15 : %d\n", get_occurrences_equal_by_index(withindex, &value));
    printf("Check index : %d\n", check_index(withindex));
    int vz = 0;
    append_value(withindex, &vz);
    printf("Checking invalid index : %d\n", check_index(withindex));
    erase_index(withindex);
    printf("Erase index : %d\n", check_index(withindex));
}

CDataframe *test_cdataframe_basics() {
    char buff[10];
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
    for (indexation i = 0; i < 3; i++) {
        convert_value(node->data, 0, buff, 10);
        printf("%s\t@ %s\n", buff, node->data->title);
        node = get_next_node(node);
    }

    for (int i = 0; i < LINE; i++)
        free(prefabValuePtr[i]);
    free(prefabValuePtr);

    printf("\nOn remarquera que les données sont persistantes : \n");

    node = cdf->data->head;
    for (indexation i = 0; i < 3; i++) {
        convert_value(node->data, 0, buff, 10);
        printf("%s\t@ %s\n", buff, node->data->title);
        node = get_next_node(node);
    }

    return cdf;
}

void test_cdataframe_printing(CDataframe *cdf) {
    Col_type **prefabValuePtr = malloc(LINE * sizeof(Col_type *));
    for (int i = 0; i < LINE; i++)
        prefabValuePtr[i] = malloc(COLUMN * sizeof(Col_type *));


    printf("printed with %d!\n", print_lines(cdf, NULL, 0, 1));

    prefabValuePtr[1][0].char_value = 'b';
    prefabValuePtr[1][1].int_value = 495;
    prefabValuePtr[1][2].int_value = -200;
    add_newline(cdf, prefabValuePtr[1], 3);
    print_lines(cdf, NULL, 0, 2);
    print_lines(cdf, NULL, 1, 2);
    print_columns(cdf, NULL, 1, 3);
    print_all(cdf, NULL);

    printf("Before rename : %s\n", cdf->data->head->data->title);
    rename_column(cdf, "Col 1", "newCol1");
    print_columns(cdf, NULL, 0, 1);
    printf("Cdf of %lld lines and %lld columns\n", get_lines_amount(cdf), get_columns_amount(cdf));

    for (int i = 0; i < LINE; i++)
        free(prefabValuePtr[i]);
    free(prefabValuePtr);
}

void test_cdataframe_indexation(CDataframe *cdf) {
    printf("failed with 3=%d\n", print_columns(cdf, "Col 3", 2, 3));
    printf("\nBefore and after sorting\n");
    print_columns(cdf, NULL, 2, 3);
    sorting_column(cdf, "Col 3", ASC);
    print_all(cdf, "Col 3");
    sort_all_columns(cdf, ASC);
    print_all(cdf, "newCol1");

    char mystery_char = 'b';
    int mystery_int = 495;
    printf("find a 'b' @%p\n", find_in(cdf, &mystery_char));
    printf("find a 495 @%p\n", find_in(cdf, &mystery_int));
    mystery_int = 15;
    printf("don't find a 15 @%p\n", find_in(cdf, &mystery_int));
}

void test_cdataframe_destruction(CDataframe *cdf) {
    // IN PREVIOUS LINES ...
    Col_type **prefabValuePtr = malloc(LINE * sizeof(Col_type *));
    for (int i = 0; i < LINE; i++)
        prefabValuePtr[i] = malloc(COLUMN * sizeof(Col_type *));
    prefabValuePtr[1][0].char_value = 'b';
    prefabValuePtr[1][1].int_value = 495;
    prefabValuePtr[1][2].int_value = -200;
    // END

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
    delete_cdataframe(&cdf);
    printf("Bye Bye cdf %p\n", cdf);


    for (int i = 0; i < LINE; i++)
        free(prefabValuePtr[i]);
    free(prefabValuePtr);
}

void test_cdataframe_userinput() {
    CDataframe *fromwrite;
    write(&fromwrite);
    print_all(fromwrite, NULL);
    save_into_csv(fromwrite, "save.csv");
    delete_cdataframe(&fromwrite);
}

void test_cdataframe_loadfromcsv() {
    Enum_type cdftype[3] = {INT, STRING, FLOAT};
    CDataframe *fromcsv = load_from_csv("example.csv", cdftype, 3);
    // exemple de fonction qui permet un affichage partiel du CDataframe
    if (fromcsv != NULL)
        print_lines(fromcsv,NULL, 2,9);
    sorting_column(fromcsv, "Age", DESC);
    print_all(fromcsv, "Age");
    sorting_column(fromcsv, "Age", ASC);
    print_all(fromcsv, "Age");

    delete_cdataframe(&fromcsv);
}

/*
 * *********************************
 *               MAIN
 * *********************************
 */

int main() {
    next_section("Basic tests with Columns: ");
    test_column_basics();

    next_section("Occurences tests : ");
    test_column_occurences_raw();

    next_section("Sorting tests : ");
    test_column_sorting();

    next_section("Operations wiht sorting : ");
    test_column_sorting_operations();

    // Pour gagner en simplicité, les fonctions de test de CDataframe se passent le pointeur
    // utilisé dans la fonction précédente.
    CDataframe *ptr;

    next_section("Basics test with CDataframe.");
    ptr = test_cdataframe_basics();

    next_section("Let's start printing !");
    test_cdataframe_printing(ptr);

    next_section("Indexation must go on ...");
    test_cdataframe_indexation(ptr);

    next_section("The rise and fall of deletions");
    test_cdataframe_destruction(ptr);

    // Input method are :
    // * User input (to avoid !)
    // * CSV load (the best way !)

    // Be aware, on clion IO with debug application is not by default in project folder,
    // it should be in /cmake-build-debug

    //      MAKE SURE THE example.csv IS IN THE RIGHT PLACE     //
    // IT SHOULD BE in /cmake-build-debug in windows with CLION //
    next_section("CSV -> CDataframe");
    test_cdataframe_loadfromcsv();

    next_section("User input -> CSV");
    test_cdataframe_userinput();

    next_section("Thanks for testing our code :)\nHave a nice day !");
    return 0;

}