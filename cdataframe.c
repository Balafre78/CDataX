#include "cdataframe.h"

/* Internal functions */

/**
 * @brief Empty the stdin buffer the right way
 */
void fflush_stdin();

void fflush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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
        for (int i = 0; i < size; i++) {
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
    int i = 0;
    while (node != NULL) {
        if (!append_value(node->data, &values[i]))
            return 1;
        node = get_next_node(cdf->data, node);
        i++;
    }
    cdf->colsize++;
    return 0;
}

int print_columns_names_partial(CDataframe *cdf, indexation from, indexation to) {
    if (from < 0 || from >= to || to >= cdf->size)
        return 2;

    lnode *node = cdf->data->head;

    int i;
    for (i = 0; i < from; i++) {
        node = get_next_node(cdf->data, node);
    }

    while (i < to) {
        printf("\t%s", node->data->title);
        node = get_next_node(cdf->data, node);
        i++;
    }
    printf("\n");

    return 0;
}

void print_columns_names(CDataframe *cdf) {
    print_columns_names_partial(cdf, 0, cdf->size);
}

int print_lines_by_objects(CDataframe *cdf, indexation from, indexation to) {
    if (from < 0 || from >= to || to >= cdf->colsize)
        return 2;

    int buffer_size = STD_BUFF_SIZE;
    char *buffer = malloc(buffer_size * sizeof(char)), *newPtr;
    if (buffer == NULL) {
        return 1;
    }

    print_columns_names(cdf);
    lnode *node = NULL;
    for (unsigned int line = from; line < to; line++) {
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
            node = get_next_node(cdf->data, node);
        }
        printf("\n");
    }
    free(buffer);

    return 0;
}

int print_columns(CDataframe *cdf, indexation from, indexation to) {
    if (from < 0 || from >= to || to >= cdf->size)
        return 2;

    int buffer_size = STD_BUFF_SIZE;
    char *buffer = malloc(buffer_size * sizeof(char)), *newPtr;
    if (buffer == NULL) {
        return 1;
    }

    print_columns_names_partial(cdf, from, to);

    lnode *node = cdf->data->head;

    int i;
    for (i = 0; i < from; i++) {
        node = get_next_node(cdf->data, node);
    }

    lnode *fromnode = node;
    while (i < to) {
        node = get_next_node(cdf->data, node);
        i++;
    }
    lnode *tonode = node;

    node = fromnode;
    for (unsigned int line = 0; line < node->data->size; line++) {
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
            node = get_next_node(cdf->data, node);
        }

        node = fromnode;
        printf("\n");
    }
    free(buffer);
    return 0;
}

int print_all(CDataframe *cdf) {
    return print_lines_by_objects(cdf, 0, cdf->colsize);
}

Column *query_column_by_name(CDataframe *cdf, char *title) {
    lnode *node = cdf->data->head;
    while (node != NULL) {
        if (strcmp(node->data->title, title) == 0)
            return node->data;
        node = node->next;
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
    ptr->title = newTitle;
    return 0;
}

void del_cell(CDataframe *cdf, char *col_title, indexation line) {
    //fprintf(stderr, "This function shouldn't be used; use at your own risks\n");
    //fflush(stderr);
    Column *column = query_column_by_name(cdf, col_title);
    free(column->data[line]);
}

int del_column(CDataframe *cdf, char *col_title) {
    Column *column = query_column_by_name(cdf, col_title);
    if (column == NULL)
        return 2;

}

int del_line(CDataframe *cdf, indexation line) {
    if (line < 0 || line >= cdf->size)
        return 2;
}