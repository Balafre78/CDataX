#include "CDataFrame.h"

/* Internal functions */

/**
 * @brief Check if it's necessary to realloc a new chunk of columns
 * @param1 Column
 */
int ensure_allocation_size(Column *col);

/**
 * @brief Empty the stdin buffer the right way
 */
void fflush_stdin();


/**
 * @brief Print a section of a CDataframe
 */
void print_section(CDataframe *cdf, int col_from, int col_to, int line_from, int line_to);

void print_columns_names_section(CDataframe *cdf, int col_from, int col_to);

void fflush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

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

void print_section(CDataframe *cdf, int col_from, int col_to, int line_from, int line_to) {
    Column** columns = cdf->columns;
    for(int i = line_from; i < line_to; i++) {
        for(int j = col_from; j < col_to; j++) {
            printf("%d\t", get_value(columns[j], i));
        }
        printf("\n");
    }
}

void print_columns_names_section(CDataframe *cdf, int col_from, int col_to) {
    Column** columns = cdf->columns;
    for(int j = col_from; j < col_to; j++) {
        printf("%s\t", columns[j]->title);
    }
}

/* Column Interactions */

Column *create_column(char *title) {
    Column *col = malloc(sizeof(Column));
    if (col == NULL) {
        fprintf(stderr, "%s", "Cannot allocate the memory\n");
        exit(1);
    }
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
        fprintf(stderr, "%s", "Invalid Index\n");
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
        return 0;
    }
    return occ;
}

/* CDataFrame Interactions */

CDataframe *create_cdataframe() {
    CDataframe *pointer = (CDataframe *) malloc(sizeof(CDataframe));
    if (pointer == NULL) {
        fprintf(stderr, "%s", "Cannot allocate the memory\n");
        exit(1);
    }
    pointer->columns = NULL;
    pointer->size = 0;
    return pointer;
}

/*
 * This function should be implement with other functions ...
 * (now it's not working SEGFAULT returned)
 void write_cdataframe(CDataframe *cDataframe) {
    int nbCol, nbLig;
    printf("Enter the amount of wanted columns : ");
    scanf("%d", &nbCol);
    printf("Enter the amount of lines wanted across the entire dataframe : ");
    scanf("%d", &nbLig); // next is gets

    // Try to realloc
    CDataframe newptr = realloc(cDataframe, nbCol);
    if (newptr == NULL) {
        fprintf(stderr, "%s", "Cannot reallocate the memory\n");
        exit(1);
    } else
        *cDataframe = newptr;

    int var;
    for (int i = 0; i < nbCol; i++) {
        char *title;
        printf("Saisir le titre de la colonne : ");
        fflush_stdin();
        gets(title);
        cDataframe[i] = create_column(title);
        for (int j = 0; j < nbLig; j++) {
            printf("[%d] <- ", j);
            scanf("%d", &var);
            if (!insert_value(cDataframe[i], var)) {
                fprintf(stderr, "%s", "Cannot allocate the memory\n");
                exit(1);
            }
        }
        printf("\n");
    }
}
*/

void print_all(CDataframe *cdf) {
    print_lines(cdf, 0, get_lines_amount(cdf));
}

void print_lines(CDataframe *cdf, int from, int to) {
    if(from < 0 || to < from || to > get_lines_amount(cdf)) {
        fprintf(stderr, "%s", "Line(s) out of range\n");
        exit(1);
    }
    print_columns_names(cdf);
    print_section(cdf, 0, cdf->size, from, to);
}

void print_columns(CDataframe *cdf, int from, int to) {
    if(from < 0 || to < from || to > cdf->size) {
        fprintf(stderr, "%s", "Column(s) out of range\n");
        exit(1);
    }
    print_columns_names_section(cdf, from, to);
    print_section(cdf, from, to, 0, get_lines_amount(cdf));
}

void print_columns_names(CDataframe *cdf) {
    print_columns_names_section(cdf, 0, cdf->size);
}

int get_lines_amount(CDataframe *cdf) {
    if (cdf->size == 0) return 0;
    return cdf->columns[0]->size;
}

int get_columns_amount(CDataframe *cdf) {
    return cdf->size;
}