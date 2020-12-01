/*
 * Task #6
 *
 * Azim Muradov, group #171
 */

#include "lines_comparators.h"

#include "sortings.h"


int comp_lines_asc(const char *line_a, const char *line_b) {
    for (size_t i = 0; i < MAX_INPUT_STRING_SIZE; ++i) {
        if (line_a[i] != line_b[i]) return line_a[i] < line_b[i] ? -1 : 1;
    }

    return 0;
}

int comp_lines_des(const char *line_a, const char *line_b) {
    return comp_lines_asc(line_b, line_a);
}
