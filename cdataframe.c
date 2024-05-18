#include "cdataframe.h"

/* Internal functions */

/**
 * @brief Empty the stdin buffer the right way
 */
void fflush_stdin();

void fflush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) putchar(c);
}

/* API functions */

CDataframe *create_empty_cdataframe() {
    CDataframe *ptr = (CDataframe *) malloc(sizeof(CDataframe));
    if (ptr != NULL) {
        ptr->size = 0;
        ptr->colsize = 0;
        ptr->data = lst_create_list();
    }
    return ptr;
}

CDataframe *create_cdataframe(Enum_type *cdfTypes, char **colNames, indexation size) {
    CDataframe *ptr = create_empty_cdataframe();
    if (ptr != NULL) {
        for (indexation i = 0; i < size; i++) {
            if (add_newcolumn(ptr, cdfTypes[i], NULL, 0, colNames[i]))
                return NULL;
        }
    }
    return ptr;
}

int add_newcolumn(CDataframe *cdf, Enum_type type, Col_type *values, indexation size, char *title) {
    if (cdf->colsize != size)
        return 2;

    Column *col = create_column(type, title);
    if (col == NULL)
        return 1;

    lnode *node = lst_create_lnode(col);
    if (node == NULL)
        return 1;

    lst_insert_tail(cdf->data, node);

    for (indexation i = 0; i < size; i++) {
        append_value(cdf->data->tail->data, &values[i]);
    }

    cdf->size++;
    return 0;
}

int add_newline(CDataframe *cdf, Col_type *values, indexation size) {
    if (cdf->size == 0)
        return 3;
    if (cdf->size != size)
        return 2;

    lnode *node = cdf->data->head;
    indexation i = 0;
    while (node != NULL) {
        if (!append_value(node->data, &values[i]))
            return 1;
        node = get_next_node(node);
        i++;
    }
    cdf->colsize++;
    return 0;
}

int print_columns_names_partial(CDataframe *cdf, indexation from, indexation to) {
    if (from < 0 || from >= to || to > cdf->size)
        return 2;

    lnode *node = cdf->data->head;

    indexation i;
    for (i = 0; i < from; i++) {
        node = get_next_node(node);
    }

    while (i < to) {
        printf("\t%s", node->data->title);
        node = get_next_node(node);
        i++;
    }
    printf("\n");

    return 0;
}

void print_columns_names(CDataframe *cdf) {
    print_columns_names_partial(cdf, 0, cdf->size);
}

int print_lines_by_objects(CDataframe *cdf, indexation from, indexation to) {
    //printf("from = %d to = %d colsize = %d \n", from, to, cdf->colsize);
    if (from < 0 || from >= to || to > cdf->colsize)
        return 2;

    int buffer_size = STD_BUFF_SIZE;
    char *buffer = malloc(buffer_size * sizeof(char)), *newPtr;
    if (buffer == NULL) {
        return 1;
    }

    print_columns_names(cdf);
    lnode *node = NULL;
    for (indexation line = from; line < to; line++) {
        node = cdf->data->head;
        printf("[%d]", line);
        while (node != NULL) {
            while (convert_value(node->data, line, buffer, buffer_size) == 1) {
                newPtr = realloc(buffer, buffer_size + STD_BUFF_SIZE);
                if (newPtr == NULL) {
                    free(buffer);
                    free(newPtr);
                    return 1;
                }
                buffer_size += STD_BUFF_SIZE;
            }
            printf("\t%s", buffer);
            node = get_next_node(node);
        }
        printf("\n");
    }
    free(buffer);

    return 0;
}

int print_columns_by_objects(CDataframe *cdf, indexation from, indexation to) {
    if (from < 0 || from >= to || to > cdf->size)
        return 2;

    int buffer_size = STD_BUFF_SIZE;
    char *buffer = malloc(buffer_size * sizeof(char)), *newPtr;
    if (buffer == NULL) {
        return 1;
    }

    print_columns_names_partial(cdf, from, to);

    lnode *node = cdf->data->head;

    indexation i;
    for (i = 0; i < from; i++) {
        node = get_next_node(node);
    }

    lnode *fromnode = node;
    while (i < to) {
        node = get_next_node(node);
        i++;
    }
    lnode *tonode = node;

    node = fromnode;
    for (indexation line = 0; line < node->data->size; line++) {
        printf("[%d]", line);

        while (node != tonode) {
            while (convert_value(node->data, line, buffer, buffer_size) == 1) {
                newPtr = realloc(buffer, buffer_size + STD_BUFF_SIZE);
                if (newPtr == NULL) {
                    free(buffer);
                    free(newPtr);
                    return 1;
                }
                buffer_size += STD_BUFF_SIZE;
            }
            printf("\t%s", buffer);
            node = get_next_node(node);
        }

        node = fromnode;
        printf("\n");
    }
    free(buffer);
    return 0;
}

int print_lines(CDataframe *cdf, char *ref_col, indexation from, indexation to) {
    if (ref_col == NULL)
        return print_lines_by_objects(cdf, from, to);

    if (from < 0 || from >= to || to > cdf->colsize)
        return 2;

    Column *col_sort = query_column_by_name(cdf, ref_col);
    if (col_sort == NULL)
        return 2;
    if (check_index(col_sort) != SORTED)
        return 3;

    int buffer_size = STD_BUFF_SIZE;
    char *buffer = malloc(buffer_size * sizeof(char)), *newPtr;
    if (buffer == NULL) {
        return 1;
    }

    print_columns_names(cdf);
    lnode *node = NULL;
    for (indexation line = from; line < to; line++) {
        node = cdf->data->head;
        printf("[%d]", line);
        while (node != NULL) {
            while (convert_value(node->data, col_sort->index[line], buffer, buffer_size) == 1) {
                newPtr = realloc(buffer, buffer_size + STD_BUFF_SIZE);
                if (newPtr == NULL) {
                    free(buffer);
                    free(newPtr);
                    return 1;
                }
                buffer_size += STD_BUFF_SIZE;
            }
            printf("\t%s", buffer);
            node = get_next_node(node);
        }
        printf("\n");
    }
    free(buffer);

    return 0;
}

int print_columns(CDataframe *cdf, char *ref_col, indexation from, indexation to) {
    if (ref_col == NULL)
        return print_columns_by_objects(cdf, from, to);

    if (from < 0 || from >= to || to > cdf->size)
        return 2;

    Column *col_sort = query_column_by_name(cdf, ref_col);
    if (col_sort == NULL)
        return 2;
    if (check_index(col_sort) != SORTED)
        return 3;

    int buffer_size = STD_BUFF_SIZE;
    char *buffer = malloc(buffer_size * sizeof(char)), *newPtr;
    if (buffer == NULL) {
        return 1;
    }

    print_columns_names_partial(cdf, from, to);

    lnode *node = cdf->data->head;

    indexation i;
    for (i = 0; i < from; i++) {
        node = get_next_node(node);
    }

    lnode *fromnode = node;
    while (i < to) {
        node = get_next_node(node);
        i++;
    }
    lnode *tonode = node;

    node = fromnode;
    for (indexation line = 0; line < node->data->size; line++) {
        printf("[%d]", line);

        while (node != tonode) {
            while (convert_value(node->data, col_sort->index[line], buffer, buffer_size) == 1) {
                newPtr = realloc(buffer, buffer_size + STD_BUFF_SIZE);
                if (newPtr == NULL) {
                    free(buffer);
                    free(newPtr);
                    return 1;
                }
                buffer_size += STD_BUFF_SIZE;
            }
            printf("\t%s", buffer);
            node = get_next_node(node);
        }

        node = fromnode;
        printf("\n");
    }
    free(buffer);
    return 0;
}

int print_all(CDataframe *cdf, char *ref_col) {
    return print_lines(cdf, ref_col, 0, cdf->colsize);
}

Column *query_column_by_name(CDataframe *cdf, char *title) {
    lnode *node = cdf->data->head;
    while (node != NULL) {
        if (strcmp(node->data->title, title) == 0)
            return node->data;
        node = get_next_node(node);
    }
    return NULL;
}

indexation get_lines_amount(CDataframe *cdf) {
    return cdf->colsize;
}

indexation get_columns_amount(CDataframe *cdf) {
    return cdf->size;
}

int rename_column(CDataframe *cdf, char *col_title, char *newTitle) {
    Column *ptr = query_column_by_name(cdf, col_title);
    if (ptr == NULL)
        return 2;
    char *newPtr = realloc(ptr->title, (strlen(newTitle) + 1) * sizeof(char *));
    if (newPtr == NULL)
        return 1;
    ptr->title = newPtr;
    strcpy(ptr->title, newTitle);
    return 0;
}

void sorting_column(CDataframe *cdf, char *col_title, int sort_dir) {
    if (sort_dir != ASC && sort_dir != DESC)
        return;

    Column *ptr = query_column_by_name(cdf, col_title);
    if (ptr == NULL)
        return;

    sort(ptr, sort_dir);
}

void sort_all_columns(CDataframe *cdf, int sort_dir) {
    if (sort_dir != ASC && sort_dir != DESC)
        return;

    lnode *node = get_first_node(cdf->data);
    while (node != NULL) {
        sort(node->data, sort_dir);
        node = get_next_node(node);
    }
}

Col_type *find_in(CDataframe *cdf, void *var) {
    Col_type *ptr = NULL;
    lnode *node = get_first_node(cdf->data);
    while (node != NULL) {
        if (check_index(node->data) == SORTED) {
            ptr = get_value_in_column_indexed(node->data, var);
            if (ptr != NULL)
                return ptr;
        } else {
            ptr = get_value_in_column_unindexed(node->data, var);
            if (ptr != NULL)
                return ptr;
        }
        node = get_next_node(node);
    }
    return NULL;
}

Col_type *get_var(CDataframe *cdf, char *col_title, indexation line) {
    Column *col = query_column_by_name(cdf, col_title);
    if (col == NULL)
        return NULL;

    if (0 <= line && line < col->size)
        return col->data[line];

    return NULL;
}

int del_column(CDataframe *cdf, char *col_title) {
    lnode *node = cdf->data->head;
    while (node != NULL) {
        if (strcmp(node->data->title, col_title) == 0) {
            if (node->prev != NULL)
                (node->prev)->next = node->next;
            if (node->next != NULL)
                (node->next)->prev = node->prev;
            if (cdf->data->head == node)
                cdf->data->head = node->next;
            if (cdf->data->tail == node)
                cdf->data->tail = node->prev;
            break;
        }
        node = get_next_node(node);
    }
    if (node == NULL)
        return 2;
    delete_column(&node->data);
    free(node);
    cdf->size--;
    return 0;
}

int del_line(CDataframe *cdf, indexation line) {
    if (line >= cdf->colsize || line < 0)
        return 2;
    lnode *node = cdf->data->head;
    while (node != NULL) {
        if (node->data->column_type == STRING)
            free(node->data->data[line]->string_value);
        free(node->data->data[line]);
        node->data->size--;
        for (indexation i = line; i < node->data->size; i++) {
            node->data->data[i] = node->data->data[i + 1];
        }
        node = get_next_node(node);
    }
    cdf->colsize--;
    return 0;
}

void delete_cdataframe(CDataframe **cdf) {
    lnode *node = (*cdf)->data->head;
    while (node != NULL) {
        del_column(*cdf, node->data->title);
        node = (*cdf)->data->head;
    }
    free(*cdf);
    *cdf = NULL;
}

int get_occurrences(CDataframe *cdf, void *var) {
    lnode *node = cdf->data->head;
    int res = 0;
    while (node != NULL) {
        if (check_index(node->data) == SORTED)
            res += get_occurrences_equal_by_index(node->data, var);
        else
            res += get_occurrences_equal_raw(node->data, var);
        node = get_next_node(node);
    }
    return res;
}

int get_superior_occurrences(CDataframe *cdf, void *var) {
    lnode *node = cdf->data->head;
    int res = 0;
    while (node != NULL) {
        res += get_occurrences_superior_raw(node->data, var);
        node = get_next_node(node);
    }
    return res;
}

int get_inferior_occurrences(CDataframe *cdf, void *var) {
    lnode *node = cdf->data->head;
    int res = 0;
    while (node != NULL) {
        res += get_occurrences_inferior_raw(node->data, var);
        node = get_next_node(node);
    }
    return res;
}

void write(CDataframe **cdf) {
    char userinput[USER_INPUT_SIZE];

    indexation lcolsize, lcdfsize;
    printf("Saisir le nombre de colonnes :");
    fgets(userinput, USER_INPUT_SIZE, stdin);
    sscanf(userinput, INDEXATION_FORMAT, &lcdfsize);
    printf("Saisir le nombre de lignes :");
    fgets(userinput, USER_INPUT_SIZE, stdin);
    sscanf(userinput, INDEXATION_FORMAT, &lcolsize);

    Enum_type *coltypes = malloc(lcdfsize * sizeof(Enum_type));
    printf("Types disponibles :\n"
           "1 - NULL\n"
           "2 - unsigned int\n"
           "3 - signed int\n"
           "4 - char\n"
           "5 - float\n"
           "6 - double\n"
           "7 - string\n"
           "8 - pointer\n");
    for (indexation i = 0; i < lcdfsize; i++) {
        printf("Quelle type doit être la colonne %lld :", i);
        fgets(userinput, USER_INPUT_SIZE, stdin);
        sscanf(userinput, "%d", &coltypes[i]);
    }

    char **colnames = malloc(lcdfsize * sizeof(char *));
    for (indexation i = 0; i < lcdfsize; i++) {
        colnames[i] = malloc(USER_INPUT_SIZE * sizeof(char));
        printf("Quel nom doit porter la colonne %lld :", i);
        fgets(colnames[i], USER_INPUT_SIZE, stdin);
        sscanf(colnames[i], "%s", colnames[i]);
        //printf("Nom : '%s'\n", colnames[i]);
    }
    *cdf = create_cdataframe(coltypes, colnames, lcdfsize);
    for (indexation i = 0; i < lcdfsize; i++)
        free(colnames[i]);
    free(colnames);
    free(coltypes);

    Col_type *values = malloc(lcdfsize * sizeof(Col_type));
    lnode *node;
    indexation j;

    node = (*cdf)->data->head;
    j = 0;
    while (node != NULL) {
        if (node->data->column_type == STRING)
            values[j].string_value = malloc(USER_INPUT_SIZE * sizeof(char));
        node = node->next;
        j++;
    }

    for (indexation i = 0; i < lcolsize; i++) {
        node = (*cdf)->data->head;
        j = 0;
        while (node != NULL) {
            printf("Saisir la case x,y = %d,%d : ", i, j);
            fgets(userinput, USER_INPUT_SIZE, stdin);
            //printf("\n");

            // Delete fgets last return carriage
            //int delzero = 0;
            //while (userinput[delzero] != '\n' && userinput[delzero] != EOF && delzero < USER_INPUT_SIZE) delzero++;
            //userinput[delzero] = '\0';

            //printf("echo '%s'\n", userinput);
            format_value(&values[j], userinput, node->data->column_type);
            //printf("Formated!\n");

            node = get_next_node(node);
            j++;
        }
        printf("Adding line %d !\n", i);
        add_newline(*cdf, values, lcdfsize);
        print_lines(*cdf, NULL, i, i + 1);
    }

    node = (*cdf)->data->head;
    j = 0;
    while (node != NULL) {
        if (node->data->column_type == STRING)
            free(values[j].string_value);
        node = node->next;
        j++;
    }

    free(values);

    printf("CDataframe completed !\n");
}

void save_into_csv(CDataframe *cdf, char *file_name) {
    FILE *fptr;
    fptr = fopen(file_name, "w+");

    lnode *node = NULL;

    node = cdf->data->head;
    while (node != NULL) {
        fprintf(fptr, "%s", node->data->title);

        node = get_next_node(node);
        if (node != NULL)
            fprintf(fptr, "%c", ',');
    }
    fprintf(fptr, "%c", '\n');


    int buffer_size = STD_BUFF_SIZE;
    char *buffer = malloc(buffer_size * sizeof(char)), *newPtr;
    if (buffer == NULL)
        return;


    for (indexation line = 0; line < cdf->colsize; line++) {
        node = cdf->data->head;
        while (node != NULL) {
            while (convert_value(node->data, line, buffer, buffer_size) == 1) {
                newPtr = realloc(buffer, buffer_size + STD_BUFF_SIZE);
                if (newPtr == NULL) {
                    free(buffer);
                    free(newPtr);
                    return;
                }
                buffer_size += STD_BUFF_SIZE;
            }
            fprintf(fptr, "%s", buffer);

            node = get_next_node(node);
            if (node != NULL)
                fprintf(fptr, "%c", ',');
        }
        fprintf(fptr, "%c", '\n');
    }
    free(buffer);
    fclose(fptr);
    fptr = NULL;
}

CDataframe *load_from_csv(char *file_name, Enum_type *dftype, int size) {
    char csvinput[USER_INPUT_SIZE];
    int inputsize;
    int cc;

    FILE *fptr;
    fptr = fopen(file_name, "r");
    if (fptr == NULL) {
        fprintf(stderr, "%s", "Wasn't able to open file !");
        return NULL;
    }

    /*while ((cc = fgetc(fptr)) != EOF) {
        //print the character to a string
        printf("%c", cc);
    }*/


    indexation lcdfsize = 1;
    // SAISIE DE indexation lcolsize, lcdfsize
    char **colnames = malloc(MAX_COL * sizeof(char *));

    inputsize = 0;
    for (cc = fgetc(fptr);
         cc != '\n' && cc != EOF && inputsize < USER_INPUT_SIZE && lcdfsize < MAX_COL;
         cc = fgetc(fptr)) {
        if (cc == ',') {
            csvinput[inputsize + 1] = '\0';

            colnames[lcdfsize - 1] = malloc((inputsize + 1) * sizeof(char));
            inputsize = 0;
            strcpy(colnames[lcdfsize - 1], csvinput);
            lcdfsize++;
        } else {
            csvinput[inputsize] = (char) cc;
            inputsize++;
        }
    }
    if (cc == '\n') {
        csvinput[inputsize + 1] = '\0';

        colnames[lcdfsize - 1] = malloc((inputsize + 1) * sizeof(char));
        inputsize = 0;
        strcpy(colnames[lcdfsize - 1], csvinput);
    }


    if (inputsize == USER_INPUT_SIZE) {
        fprintf(stderr, "%s", "Max input size reach !\n");
        return NULL;
    }
    if (lcdfsize == MAX_COL) {
        fprintf(stderr, "%s", "Max input size reach !\n");
        return NULL;
    }
    if (cc == EOF) {
        fprintf(stderr, "%s", "CSV format incorrect !\n");
        return NULL;
    }
    if (size != lcdfsize) {
        fprintf(stderr, "CSV file %lld and given size %d are different !\n", lcdfsize, size);
        return NULL;
    }

    CDataframe *cdf = create_cdataframe(dftype, colnames, lcdfsize);
    for (indexation i = 0; i < lcdfsize; i++)
        free(colnames[i]);
    free(colnames);

    Col_type *values = malloc(lcdfsize * sizeof(Col_type));
    lnode *node;
    indexation j;

    node = cdf->data->head;
    j = 0;
    while (node != NULL) {
        if (node->data->column_type == STRING)
            values[j].string_value = malloc(USER_INPUT_SIZE * sizeof(char));
        node = node->next;
        j++;
    }

    indexation i = 0;
    cc = fgetc(fptr);
    while (cc != EOF) {
        node = cdf->data->head;
        j = 0;

        inputsize = 0;
        while (cc != '\n' && cc != EOF && inputsize < USER_INPUT_SIZE && node != NULL) {

            if (cc == ',') {
                csvinput[inputsize + 1] = '\0';

                format_value(&values[j], csvinput, node->data->column_type);

                inputsize = 0;
                node = get_next_node(node);
                j++;
            } else {
                csvinput[inputsize] = (char) cc;
                inputsize++;
            }
            cc = fgetc(fptr);
        }
        if (cc == EOF) {
            fprintf(stderr, "Inconsistent CSV file EOF before end\n");
            return NULL;
        }
        if (inputsize == USER_INPUT_SIZE) {
            fprintf(stderr, "Too large input on line %lld\n", i + 1);
            fprintf(stderr, "%s\n", csvinput);
            return NULL;
        }
        if (node == NULL || node->next != NULL) {
            fprintf(stderr, "%s %lld %s", "Inconsistent CSV file not enough argument for object", i + 1, "!\n");
            return NULL;
        }
        if (cc == '\n') {
            csvinput[inputsize + 1] = '\0';

            format_value(&values[j], csvinput, node->data->column_type);
        }

        printf("Adding line %lld !\n", i);
        add_newline(cdf, values, lcdfsize);
        print_lines(cdf, NULL, 0, i + 1);

        cc = fgetc(fptr);
        i++;
    }

    node = cdf->data->head;
    j = 0;
    while (node != NULL) {
        if (node->data->column_type == STRING)
            free(values[j].string_value);
        node = get_next_node(node);
        j++;
    }

    free(values);
    fclose(fptr);
    fptr = NULL;

    printf("CDataframe completed !\n");
    return cdf;
}