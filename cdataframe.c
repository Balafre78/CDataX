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