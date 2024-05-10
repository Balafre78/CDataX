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
        col->valid_index = 0;
        col->sort_dir = ASC;
    }
    return col;
}

int insert_value_raw(Column *col, void *value) {
    // Ensure Allocation size
    if (col->size + 1 > col->max_size) {
        Col_type **newPtr;
        indexation *newIndex;
        if (col->size == 0) {
            newPtr = (Col_type **) malloc(REALOC_SIZE * sizeof(Col_type *));
            newIndex = (indexation *) calloc(REALOC_SIZE, sizeof(Col_type *));
        } else {
            newPtr = (Col_type **) realloc(col->data, col->max_size + REALOC_SIZE);
            newIndex = (indexation *) realloc(col->index, col->max_size + REALOC_SIZE);
        }
        if (newPtr == NULL || newIndex == NULL) {
            return 0;
        }
        col->data = newPtr;
        col->index = newIndex;
        col->max_size += REALOC_SIZE;
    }

    if (value == NULL) {
        col->data[col->size] = malloc(sizeof(void *));
        col->data[col->size]->struct_value = NULL;
        col->index[col->size] = col->size;
        col->size++;
        if (col->valid_index != UNSORTED)
            col->valid_index = ALMOST_SORT;
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

int print_col_raw(Column *col) {
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
        return NULL;
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

void insertion_sort(indexation *index, indexation size) {
    indexation i, j, k;
    for (i = 1; i < size; i++) {
        k = index[i];
        j = i - 1;
        while (j > 0 && index[j] > k) {
            index[j + 1] = index[j];
            j--;
        }
        index[j + 1] = k;
    }
}

void insertion_sort_reverse(indexation *index, indexation size) {
    indexation i, j, k;
    for (i = 1; i < size; i++) {
        k = index[i];
        j = i - 1;
        while (j > 0 && index[j] < k) {
            index[j + 1] = index[j];
            j--;
        }
        index[j + 1] = k;
    }
}

void swap(indexation *index, indexation i, indexation j) {
    indexation tmp = index[i];
    index[i] = index[j];
    index[j] = tmp;
}

indexation partition(indexation *index, indexation left, indexation right) {
    indexation pivot = index[right];
    indexation i = left - 1;
    for (indexation j = left; i <= right; j++) {
        if (index[j] < pivot) {
            i++;
            swap(index, i, j);
        }
    }
    swap(index, i + 1, right);
    return i + 1;
}

void quicksort(indexation *index, indexation left, indexation right) {
    if (left < right) {
        indexation pi = partition(index, left, right);
        quicksort(index, left, pi - 1);
        quicksort(index, pi + 1, right);
    }
}

indexation partition_reverse(indexation *index, indexation left, indexation right) {
    //TODO: fix the partition reverse
    indexation pivot = index[right];
    indexation i = left - 1;
    for (indexation j = left; i < right - 1; j++) {
        if (index[j] <= pivot) {
            i++;
            swap(index, i, j);
        }
    }
    swap(index, i + 1, right);
    return i + 1;
}

void quicksort_reverse(indexation *index, indexation left, indexation right) {
    if (left < right) {
        indexation pi = partition_reverse(index, left, right);
        quicksort(index, left, pi - 1);
        quicksort(index, pi + 1, right);
    }
}

void sort(Column *col, int sort_dir) {
    if (col->valid_index == UNSORTED && sort_dir == ASC) {
        quicksort(col->index, 0, col->size);
    } else if (col->valid_index == SORTED && sort_dir == ASC) {
        insertion_sort(col->index, col->size);
    } else if (col->valid_index == UNSORTED && sort_dir == DESC) {
        quicksort_reverse(col->index, 0, col->size);
    } else if (col->valid_index == SORTED && sort_dir == DESC) {
        insertion_sort_reverse(col->index, col->size);
    }
}

int print_col_by_index(Column *col) {
    int buffer_size = STD_BUFF_SIZE, rc;
    char *buffer = malloc(buffer_size * sizeof(char)), *newPtr;
    if (buffer == NULL) {
        return 1;
    }
    printf("%s\n", col->title);
    for (unsigned int i = 0; i < col->size; i++) {
        while ((rc = convert_value(col, col->index[i], buffer, buffer_size)) == 1) {
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

int get_occurrences_inferior_raw(Column *col, Col_type *x) {
    int occ = 0;
    for (unsigned int i = 0; i < col->size; i++) {
        if (col->data[i]->struct_value == NULL)
            continue;
        if (compare_Col_type(x, col->data[i], col->column_type) < 0)
            occ++;
    }
    return occ;
}

int get_occurrences_superior_raw(Column *col, Col_type *x) {
    int occ = 0;
    for (unsigned int i = 0; i < col->size; i++) {
        if (col->data[i]->struct_value == NULL)
            continue;
        if (compare_Col_type(x, col->data[i], col->column_type) > 0)
            occ++;
    }
    return occ;
}

int get_occurrences_equal_raw(Column *col, Col_type *x) {
    int occ = 0;
    for (unsigned int i = 0; i < col->size; i++) {
        if (col->data[i]->struct_value == NULL)
            continue;
        if (compare_Col_type(x, col->data[i], col->column_type) == 0)
            occ++;
    }
    return occ;
}