#include "column.h"

Column *create_column(Enum_type type, char *title) {
    Column *col = malloc(sizeof(Column));
    if (col != NULL) {
        col->title = title;
        col->max_size = 0;
        col->size = 0;
        col->column_type = type;
        col->data = NULL;
        col->index = NULL;
    }
    return col;
}

int insert_value(Column *col, void *value) {
    if (col->size + 1 > col->max_size) {
        Col_type **newPtr;
        if (col->size == 0) {
            newPtr = (Col_type **) malloc(REALOC_SIZE * sizeof(Col_type *));
        } else {
            newPtr = (Col_type **) realloc(col->data, col->max_size + REALOC_SIZE);
        }
        if (newPtr == NULL) {
            return 0;
        }
        col->data = newPtr;
        col->max_size += REALOC_SIZE;
    }
    switch (col->column_type) {
        case INT:
            col->data[col->size] = (signed int *) malloc(sizeof(signed int));
            signed int *sint = (signed int *) value;
            col->data[col->size]->int_value = *sint;
            break;
        case UINT:
            col->data[col->size] = (unsigned int *) malloc(sizeof(int));
            unsigned int *uint = (unsigned int *) value;
            col->data[col->size]->uint_value = *uint;
            break;
        case FLOAT:
            col->data[col->size] = (float *) malloc(sizeof(float));
            float *f = (float *) value;
            col->data[col->size]->float_value = *f;
            break;
        case DOUBLE:
            col->data[col->size] = (double *) malloc(sizeof(double));
            double *d = (double *) value;
            col->data[col->size]->double_value = *d;
            break;
        case CHAR:
            col->data[col->size] = (char *) malloc(sizeof(char));
            char *c = (char *) value;
            col->data[col->size]->char_value = *c;
            break;
        case STRING:
            col->data[col->size] = (char **) malloc(sizeof(char *));
            char *str = (char *) value;
            col->data[col->size]->string_value = str;
            break;
        default:
            fprintf(stderr, "%s", "Type not supported Error");
            exit(1);
            break;
    }
    col->size++;
    return 1;
}