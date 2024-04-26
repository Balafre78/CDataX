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
    // Ensure Allocation size
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

    if (value == NULL) {
        col->data[col->size] = malloc(sizeof(void *));
        col->data[col->size]->struct_value = NULL;
        col->size++;
        return 1;
    }

    switch (col->column_type) {
        case INT:
            col->data[col->size] = malloc(sizeof(signed int));
            signed int *sint = (signed int *) value;
            col->data[col->size]->int_value = *sint;
            break;
        case UINT:
            col->data[col->size] = malloc(sizeof(int));
            unsigned int *uint = (unsigned int *) value;
            col->data[col->size]->uint_value = *uint;
            break;
        case FLOAT:
            col->data[col->size] = malloc(sizeof(float));
            float *f = (float *) value;
            col->data[col->size]->float_value = *f;
            break;
        case DOUBLE:
            col->data[col->size] = malloc(sizeof(double));
            double *d = (double *) value;
            col->data[col->size]->double_value = *d;
            break;
        case CHAR:
            col->data[col->size] = malloc(sizeof(char));
            char *c = (char *) value;
            col->data[col->size]->char_value = *c;
            break;
        case STRING:
            col->data[col->size] = malloc(sizeof(char *));
            char *str = (char *) value;
            col->data[col->size]->string_value = str;
            break;
        default:
            return 0;
    }
    col->size++;
    return 1;
}

void delete_column(Column **col) {
    for (unsigned int i = 0; i < (*col)->size; i++) {
        free((*col)->data[i]);
    }
    free((*col)->data);
    free((*col)->index);
    free(*col);
    *col = NULL;
}

int convert_value(Column *col, unsigned int i, char *str, int size) {
    int rc;
    if (col->data[i]->struct_value == NULL) {
        //printf("NULL ptr ");
        if (size < 5) {
            return 1;
        }
        rc = snprintf(str, size, "%s", "NULL");
    } else {
        switch (col->column_type) {
            case UINT:
                rc = snprintf(str, size, "%u", col->data[i]->uint_value);
                break;
            case INT:
                rc = snprintf(str, size, "%d", col->data[i]->int_value);
                break;
            case CHAR:
                rc = snprintf(str, size, "%c", col->data[i]->char_value);
                break;
            case FLOAT:
                rc = snprintf(str, size, "%f", col->data[i]->float_value);
                break;
            case DOUBLE:
                rc = snprintf(str, size, "%f", col->data[i]->double_value);
                break;
            case STRING:
                rc = snprintf(str, size, "%s", col->data[i]->string_value);
                break;
            case NULLVAL:
                rc = snprintf(str, size, "%p", col->data[i]->struct_value);
                break;
            case STRUCTURE:
                //TODO: better printing function specific structs types
                rc = snprintf(str, size, "%p", col->data[i]->struct_value);
                break;
        }
    }
    if (rc >= size) {
        return 1;
    } else if (rc < 0) {
        return rc;
    } else {
        return 0;
    }
}

int print_col(Column *col) {
    int buffer_size = STD_BUFF_SIZE, rc;
    char *buffer = malloc(buffer_size * sizeof(char)), *newPtr;
    if (buffer == NULL) {
        return 1;
    }
    printf("%s\n", col->title);
    for (unsigned int i = 0; i < col->size; i++) {
        while ((rc = convert_value(col, i, buffer, buffer_size)) == 1) {
            newPtr = realloc(buffer, buffer_size + STD_BUFF_SIZE);
            if (newPtr == NULL) {
                free(buffer);
                free(newPtr);
                return 1;
            }
            buffer_size += STD_BUFF_SIZE;
        }
        printf("[%d] %s\n", i, buffer);
    }

    free(buffer);
    return 0;
}

Col_type *get_value(Column *col, unsigned int index) {
    if (index >= col->size) {
        fprintf(stderr, "%s", "Index out of range");
        exit(1);
    }
    return col->data[index];
}

int compare_Col_type(Col_type *A, Col_type *B, Enum_type type) {
    switch (type) {
        case UINT:
            if (A->uint_value > B->uint_value)
                return 1;
            else if (A->uint_value < B->uint_value)
                return -1;
            else
                return 0;
        case INT:
            if (A->int_value > B->int_value)
                return 1;
            else if (A->int_value < B->int_value)
                return -1;
            else
                return 0;
        case CHAR:
            if (A->char_value > B->char_value)
                return 1;
            else if (A->char_value < B->char_value)
                return -1;
            else
                return 0;
        case FLOAT:
            if (A->float_value > B->float_value)
                return 1;
            else if (A->float_value < B->float_value)
                return -1;
            else
                return 0;
        case DOUBLE:
            if (A->double_value > B->double_value)
                return 1;
            else if (A->double_value < B->double_value)
                return -1;
            else
                return 0;
        case STRING:
            return strcmp(A->string_value, B->string_value);
        case NULLVAL:
            return 0;
        case STRUCTURE:
            //TODO: better printing function specific structs types
            return 0;
    }
}

int get_occurrences_inferior(Column *col, Col_type *x) {
    int occ = 0;
    for (unsigned int i = 0; i < col->size; i++) {
        if (col->data[i]->struct_value == NULL)
            continue;
        if (compare_Col_type(x, col->data[i], col->column_type) < 0)
            occ++;
    }
    return occ;
}

int get_occurrences_superior(Column *col, Col_type *x) {
    int occ = 0;
    for (unsigned int i = 0; i < col->size; i++) {
        if (col->data[i]->struct_value == NULL)
            continue;
        if (compare_Col_type(x, col->data[i], col->column_type) > 0)
            occ++;
    }
    return occ;
}

int get_occurrences_equal(Column *col, Col_type *x) {
    int occ = 0;
    for (unsigned int i = 0; i < col->size; i++) {
        if (col->data[i]->struct_value == NULL)
            continue;
        if (compare_Col_type(x, col->data[i], col->column_type) == 0)
            occ++;
    }
    return occ;
}