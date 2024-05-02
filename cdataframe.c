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

CDataframe *create_cdataframe(Enum_type *cdfTypes, char **colNames, int size) {
    CDataframe *ptr = create_empty_cdataframe();
    if (ptr != NULL) {
        for (int i = 0; i < size; i++) {
            if (add_newcolumn(ptr, cdfTypes[i], NULL, 0, colNames[i]))
                return NULL;
        }
    }
    return ptr;
}

int add_newcolumn(CDataframe *cdf, Enum_type type, Col_type *values, int size, char *title) {
    if (cdf->colsize != size)
        return 2;

    Column *col = create_column(type, title);
    if (col == NULL)
        return 1;

    lnode *node = lst_create_lnode(col);
    if (node == NULL)
        return 1;

    lst_insert_tail(cdf->data, node);

    for (int i = 0; i < size; i++) {
        insert_value(cdf->data->tail->data, &values[i]);
    }

    cdf->size++;
    return 0;
}

int add_newline(CDataframe *cdf, Col_type *values, int size) {
    if (cdf->size == 0)
        return 3;
    if (cdf->size != size)
        return 2;

    lnode *node = cdf->data->head;
    for (int i = 0; i < size; i++) {
        if (!insert_value(node->data, &values[i]))
            return 1;
        node = get_next_node(cdf->data, node);
    }
    cdf->colsize++;
    return 0;
}