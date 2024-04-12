#include "CDataFrame.h"

/* Internal functions */

/**
 * @brief : Check if it's necessary to realloc a new chunk of data
 * @param1 : Column
 */
int ensure_allocation_size(Column *col);

int ensure_allocation_size(Column *col) {
    if (col->size + 1 > col->physical_size) {
        int *newPtr;

        if (col->data == NULL) {
            // Allocation
            newPtr = calloc(REALOC_SIZE, sizeof(int));
        } else {
            // Reallocation
            newPtr = realloc(col->data, (col->physical_size + REALOC_SIZE));
            if (newPtr != NULL) col->physical_size += REALOC_SIZE;
        }

        if (newPtr == NULL) return 0;
        col->data = newPtr;
    }
    return 1;
}

/* Column Interactions */

Column *create_column(char *title) {
    Column *col = malloc(sizeof(Column));
    col->title = title;
    col->physical_size = 0;
    col->size = 0;
    col->data = NULL;
    return col;
}

int insert_value(Column *col, int value) {
    if (!ensure_allocation_size(col)) return 0;

    // Insertion
    col->data[col->size] = value;
    col->size++;
    return 1;
}

void delete_column(Column **col) {
    free((*col)->data);
    (*col)->data = NULL;
    free(*col);
    *col = NULL;
}

void print_col(Column *col) {
    if (col->data != NULL) {
        for (int i = 0; i < col->size; i++) {
            printf("[%d] %d\n", i, col->data[i]);
        }
    } else {
        printf("Column is empty.\n");
    }
}

int get_value(Column *col, int index) {
    if (0 <= index && index <= col->size)
        return col->data[index];
    else {
        fprintf(stderr, "%s", "Invalid Idx\n");
        exit(1);
    }
}


int get_occurrences_inferior(Column *col, int x) {
    int occ = 0;
    if (col->data != NULL) {
        for (int i = 0; i < col->size; i++) {
            if (col->data[i] < x) {
                occ++;
            }
        }
    } else {
        fprintf(stderr, "%s", "Empty Array\n");
        exit(1);
    }
    return occ;
}

int get_occurrences_superior(Column *col, int x) {
    int occ = 0;
    if (col->data != NULL) {
        for (int i = 0; i < col->size; i++) {
            if (col->data[i] > x) {
                occ++;
            }
        }
    } else {
        fprintf(stderr, "%s", "Empty Array\n");
        exit(1);
    }
    return occ;
}

int get_occurrences_equal(Column *col, int x) {
    int occ = 0;
    if (col->data != NULL) {
        for (int i = 0; i < col->size; i++) {
            if (col->data[i] == x) {
                occ++;
            }
        }
    } else {
        fprintf(stderr, "%s", "Empty Array\n");
        exit(1);
    }
    return occ;
}

/* CDataFrame Interactions */

CDataframe *create_cdataframe(int size) {
    return (CDataframe *) malloc(size * sizeof(CDataframe));
}

void write_cdataframe(CDataframe *cDataframe, int size) {

}