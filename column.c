#include "column.h"

Column *create_column(Enum_type type, char *title) {
    Column *col = malloc(sizeof(Column));
    if (col != NULL) {
        col->title = malloc((strlen(title) + 1) * sizeof(char));
        strcpy(col->title, title);
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

int append_value(Column *col, void *value) {
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

    col->index[col->size] = (indexation) col->size;
    if (col->valid_index != INVALID)
        col->valid_index = ALMOST_SORT;

    if (value == NULL) {
        col->data[col->size] = malloc(sizeof(void *));
        col->data[col->size]->struct_value = NULL;
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
            //char *c = (char *) value;
            col->data[col->size]->char_value = *((char *) value);
            break;
        case STRING:
            col->data[col->size] = malloc(sizeof(char *));
            // DONT CHANGE (char **) !!
            char **str = (char **) value;
            col->data[col->size]->string_value = malloc((strlen(*str) + 1) * sizeof(char));
            strcpy(col->data[col->size]->string_value, *str);
            break;
        default:
            return 0;
    }
    col->size++;
    return 1;
}

void erase_index(Column *col) {
    free(col->index);
    col->index = NULL;
    col->valid_index = 0;
}

int check_index(Column *col) {
    if (col->index == NULL)
        return INVALID;

    for (indexation i = 1; i < col->size; i++) {
        if (compare_Col_type(col->data[col->index[i]], col->data[col->index[i - 1]], col->column_type) < 0) {
            return ALMOST_SORT;
        }
    }

    return SORTED;
}

void delete_column(Column **col) {
    if ((*col)->column_type == STRING)
        for (indexation i = 0; i < (*col)->size; i++)
            free((*col)->data[i]->string_value);

    for (indexation i = 0; i < (*col)->size; i++)
            free((*col)->data[i]);


    free((*col)->data);
    free((*col)->index);
    free((*col)->title);
    free(*col);
    *col = NULL;
}

int convert_value(Column *col, indexation i, char *str, indexation size) {
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
    for (indexation i = 0; i < col->size; i++) {
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

Col_type *get_value(Column *col, indexation index) {
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

void format_value(Col_type *ptr, char *str, Enum_type type) {
    switch (type) {
        case UINT:
            //ptr->uint_value = (unsigned) strtoul(str, NULL, 10);
            sscanf(str, "%ud", &ptr->uint_value);
            break;
        case INT:
            //ptr->int_value = atoi(str);
            sscanf(str, "%d", &ptr->int_value);
            break;
        case CHAR:
            //ptr->char_value = *str;
            sscanf(str, "%c", &ptr->char_value);
            break;
        case FLOAT:
            //ptr->float_value = atof(str);
            sscanf(str, "%f", &ptr->float_value);
            break;
        case DOUBLE:
            //ptr->double_value = strtod(str, NULL);
            sscanf(str, "%lf", &ptr->double_value);
            break;
        case STRING:
            sscanf(str, "%s", ptr->string_value);
            //strcpy(ptr->string_value, str);
            break;
        case NULLVAL:
            ptr->struct_value = NULL;
            break;
        case STRUCTURE:
            //TODO: better printing function specific structs types
            ptr->struct_value = NULL;
            break;
    }
}

void insertion_sort(Column *column) {
    indexation i, j, k;
    for (i = 1; i < column->size; i++) {
        k = column->index[i];
        j = i - 1;
        while (j >= 0 && compare_Col_type(column->data[column->index[j]], column->data[k], column->column_type) > 0) {
            column->index[j + 1] = column->index[j];
            j--;
        }
        column->index[j + 1] = k;
    }
}

void insertion_sort_reverse(Column *column) {
    indexation i, j, k;
    //Col_type *knode;
    for (i = 1; i < column->size; i++) {
        k = column->index[i];
        j = i - 1;
        while (j >= 0 && compare_Col_type(column->data[column->index[j]], column->data[k], column->column_type) < 0) {
            column->index[j + 1] = column->index[j];
            j--;
        }
        column->index[j + 1] = k;
    }
}

void swap_idx(Column *column, indexation i, indexation j) {
    indexation tmp = column->index[i];
    column->index[i] = column->index[j];
    column->index[j] = tmp;
}

indexation partition(Column *column, indexation left, indexation right) {
    indexation pivot = right;
    indexation i = left - 1;
    for (indexation j = left; j < right; j++) {
        if (compare_Col_type(column->data[column->index[j]], column->data[column->index[pivot]],
                             column->column_type) <= 0) {
            i++;
            swap_idx(column, i, j);
        }
    }
    swap_idx(column, i + 1, right);
    return i + 1;
}

void quicksort(Column *column, indexation left, indexation right) {
    if (left < right) {
        indexation pi = partition(column, left, right);
        quicksort(column, left, pi - 1);
        quicksort(column, pi + 1, right);
    }
}

indexation partition_reverse(Column *column, indexation left, indexation right) {
    //TODO: Inverser la partition _reverse
    indexation pivot = right;
    indexation i = left - 1;
    for (indexation j = left; j < right; j++) {
        if (compare_Col_type(column->data[column->index[j]], column->data[column->index[pivot]],
                             column->column_type) >= 0) {
            i++;
            swap_idx(column, i, j);
        }
    }
    swap_idx(column, i + 1, right);
    return i + 1;
}

void quicksort_reverse(Column *column, indexation left, indexation right) {
    if (left < right) {
        indexation pi = partition(column, left, right);
        quicksort_reverse(column, left, pi - 1);
        quicksort_reverse(column, pi + 1, right);
    }
}


void sort(Column *col, int sort_dir) {
    //swap(col->index, 0, 2);
    //for (indexation i = 0; i < col->size; i++)
    //    printf("%lld ", col->index[i]);
    //printf("\n");
    if (col->valid_index == INVALID && sort_dir == ASC) {
        quicksort(col, 0, col->size - 1);
    } else if (col->valid_index == SORTED && sort_dir == ASC) {
        insertion_sort(col);
    } else if (col->valid_index == INVALID && sort_dir == DESC) {
        quicksort_reverse(col, 0, col->size - 1);
    } else if (col->valid_index == SORTED && sort_dir == DESC) {
        insertion_sort_reverse(col);
    }
}

int print_col_by_index(Column *col) {
    int buffer_size = STD_BUFF_SIZE, rc;
    char *buffer = malloc(buffer_size * sizeof(char)), *newPtr;
    if (buffer == NULL) {
        return 1;
    }
    //printf("Indexation ");
    //for (int i = 0; i < col->size; i++)
    //    printf("%lld ", col->index[i]);
    printf("\n");
    printf("%s\n", col->title);
    for (indexation i = 0; i < col->size; i++) {
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

void update_index(Column *col) {
    sort(col, ASC);
    col->valid_index = check_index(col);
}

int get_occurrences_inferior_raw(Column *col, void *x) {
    int occ = 0;
    Col_type *ptr = (Col_type *) x;
    for (indexation i = 0; i < col->size; i++) {
        if (col->data[i]->struct_value == NULL)
            continue;
        if (compare_Col_type(ptr, col->data[i], col->column_type) < 0)
            occ++;
    }
    return occ;
}

int get_occurrences_superior_raw(Column *col, void *x) {
    int occ = 0;
    Col_type *ptr = (Col_type *) x;
    for (indexation i = 0; i < col->size; i++) {
        if (col->data[i]->struct_value == NULL)
            continue;
        if (compare_Col_type(ptr, col->data[i], col->column_type) > 0)
            occ++;
    }
    return occ;
}

int get_occurrences_equal_raw(Column *col, void *x) {
    int occ = 0;
    Col_type *ptr = (Col_type *) x;
    for (indexation i = 0; i < col->size; i++) {
        if (col->data[i]->struct_value == NULL)
            continue;
        if (compare_Col_type(ptr, col->data[i], col->column_type) == 0)
            occ++;
    }
    return occ;
}

int get_occurrences_equal_by_index(Column *col, void *x) {
    if (check_index(col) != SORTED)
        return -1;
    int occ = 0;
    Col_type *ptr = (Col_type *) x;
    indexation left = 0, right = col->size - 1, pivot;
    //printf("==========\n");
    //print_col_raw(col);
    //print_col_by_index(col);
    while (left <= right) {
        pivot = (left + right) / 2;
        int cmp = compare_Col_type(col->data[col->index[pivot]], ptr, col->column_type);
        if (cmp == 0) {
            // Obtain near value on the right
            occ++;
            indexation i = pivot + 1;
            //printf("i=%lld & pivot=%lld\n", i, pivot);
            while (i < col->size && compare_Col_type(col->data[col->index[i]], ptr, col->column_type) == 0)
                i++;
            occ += i - pivot - 1;
            //printf("occ=%d->", occ);
            i = pivot - 1;
            while (i > 0 && compare_Col_type(col->data[col->index[i]], ptr, col->column_type) == 0)
                i--;
            occ += pivot - 1 - i;
            //printf("occ=%d\n", occ);
            break;
        } else if (cmp < 0) {
            //printf("Inf 0 ");
            left = pivot + 1;
        } else {
            //printf("Sup 0 ");
            right = pivot - 1;
        }
        //printf("pass pivot=%lld\n", pivot);
    }
    return occ;
}

int search_value_in_column(Column *col, void *val) {
    if (check_index(col) != SORTED)
        return -1;
    return (get_value_in_column_indexed(col, val) != NULL);
}

Col_type *get_value_in_column_indexed(Column *col, void *val) {
    if (check_index(col) != SORTED)
        return NULL;
    indexation left = 0, right = col->size - 1, pivot;
    Col_type *ptr = (Col_type *) val;
    while (left <= right) {
        pivot = (left + right) / 2;
        int cmp = compare_Col_type(col->data[col->index[pivot]], ptr, col->column_type);
        if (cmp == 0) {
            return col->data[col->index[pivot]];
        } else if (cmp < 0) {
            left = pivot + 1;
        } else {
            right = pivot - 1;
        }
    }
    return NULL;
}

Col_type *get_value_in_column_unindexed(Column *col, void *val) {
    Col_type *ptr = (Col_type *) val;
    for (indexation i = 0; i < col->size; i++) {
        if (compare_Col_type(col->data[i], ptr, col->column_type)) {
            return col->data[i];
        }
    }
    return NULL;
}

void propagate_index(Column *colA, Column *colB) {
    if (colA->size != colB->size)
        return;
    for (indexation i = 0; i < colA->size; i++)
        colB->index[i] = colA->index[i];
}
