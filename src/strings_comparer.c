/*
 * Task #6
 *
 * Azim Muradov, group #171
 */

#include "sortings.h"
#include "lines_comparators.h"
#include "error_handling.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


/*-------------------------------------------- Types missing in sortings.h -------------------------------------------*/

typedef void (*sorting_func_t)(strings_array_t, array_size_t, comparator_func_t);


/*----------------------------------------------- Program Configuration ----------------------------------------------*/

typedef struct {
    array_size_t line_number;
    char *input_filename;
    char *output_filename;
    sorting_func_t algorithm;
    comparator_func_t comparator;
} program_configuration_t;


/*---------------------------------------------------- Algorithms ----------------------------------------------------*/

static const char *const ALGORITHM_NAMES[] = {
    "bubble",
    "insertion",
    "merge",
    "quick",
    "radix",
};

static const sorting_func_t ALGORITHM_FUNCTIONS[] = {
    bubble,
    insertion,
    merge,
    quick,
    radix,
};

static const size_t ALGORITHM_NUMBER = sizeof ALGORITHM_NAMES / sizeof *ALGORITHM_NAMES;


/*---------------------------------------------------- Comparators ---------------------------------------------------*/

static const char *const COMPARATOR_NAMES[] = {
    "asc",
    "des",
};

static const comparator_func_t COMPARATOR_FUNCTIONS[] = {
    comp_lines_asc,
    comp_lines_des,
};

static const size_t COMPARATOR_NUMBER = sizeof COMPARATOR_NAMES / sizeof *COMPARATOR_NAMES;


/*------------------------------------------- Parse Command Line Arguments -------------------------------------------*/

task_6_error_code_t parse_cl_arguments(int argc, char **argv, program_configuration_t *configuration) {
    *configuration = (program_configuration_t) {0};

    if (argc != 5) return TASK_6_E_WRONG_CL_ARGS_FORMAT;


    char *end_ptr = NULL;
    long long ret_val = strtoll(argv[0], &end_ptr, 0);
    if (errno == ERANGE || end_ptr != argv[0] + strlen(argv[0]) || ret_val <= 0) return TASK_6_E_WRONG_CL_ARGS_FORMAT;
    configuration->line_number = ret_val;

    configuration->input_filename = argv[1];

    configuration->output_filename = argv[2];

    char *algorithm_name = argv[3];
    for (size_t i = 0; i < ALGORITHM_NUMBER; ++i) {
        if (strcmp(algorithm_name, ALGORITHM_NAMES[i]) == 0) {
            configuration->algorithm = ALGORITHM_FUNCTIONS[i];
            break;
        }
    }
    if (configuration->algorithm == NULL) return TASK_6_E_WRONG_CL_ARGS_FORMAT;

    char *comparator_name = argv[4];
    for (size_t i = 0; i < COMPARATOR_NUMBER; ++i) {
        if (strcmp(comparator_name, COMPARATOR_NAMES[i]) == 0) {
            configuration->comparator = COMPARATOR_FUNCTIONS[i];
            break;
        }
    }
    if (configuration->comparator == NULL) return TASK_6_E_WRONG_CL_ARGS_FORMAT;

    return TASK_6_E_OK;
}


/*----------------------------------------------- Run Strings Comparer -----------------------------------------------*/

task_6_error_code_t read_lines_from_file(program_configuration_t configuration, strings_array_t lines) {
    FILE *file = fopen(configuration.input_filename, "r");
    if (file == NULL) return TASK_6_E_FOPEN;

    for (array_size_t i = 0; i < configuration.line_number; ++i) {
        if (fgets(lines[i], MAX_INPUT_STRING_SIZE, file) == NULL) {
            return feof(file) ? TASK_6_E_FREAD_FEOF : ferror(file) ? TASK_6_E_FREAD_FERROR : TASK_6_E_OTHER;
        }
    }

    char *last_line = lines[configuration.line_number - 1];
    array_size_t last_line_length = strlen(last_line);
    if (last_line[last_line_length - 1] != '\n') {
#ifdef _WIN32
        last_line[last_line_length] = '\r';
        last_line[last_line_length + 1] = '\n';
        last_line[last_line_length + 2] = '\0';
#else
        last_line[last_line_length] = '\n';
        last_line[last_line_length + 1] = '\0';
#endif
    }

    fclose(file);

    return TASK_6_E_OK;
}

task_6_error_code_t write_lines_to_file(program_configuration_t configuration, strings_array_t lines) {
    FILE *file = fopen(configuration.output_filename, "w");
    if (file == NULL) return TASK_6_E_FOPEN;

    for (array_size_t i = 0; i < configuration.line_number; ++i) {
        if (fputs(lines[i], file) == -1) return TASK_6_E_FWRITE_FERROR;
    }

    fclose(file);

    return TASK_6_E_OK;
}

task_6_error_code_t run_strings_comparer(program_configuration_t configuration) {
    task_6_error_code_t error_code;

    strings_array_t lines = malloc(configuration.line_number * sizeof *lines);
    if (lines == NULL) return TASK_6_E_MEM_ALLOC;
    for (array_size_t i = 0; i < configuration.line_number; ++i) {
        lines[i] = calloc(MAX_INPUT_STRING_SIZE, sizeof(char));
        if (lines[i] == NULL) return TASK_6_E_MEM_ALLOC;
    }

    if ((error_code = read_lines_from_file(configuration, lines))) return error_code;

    configuration.algorithm(lines, configuration.line_number, configuration.comparator);
    if (curr_error) return curr_error;

    if ((error_code = write_lines_to_file(configuration, lines))) return error_code;

    for (array_size_t i = 0; i < configuration.line_number; ++i) free(lines[i]);
    free(lines);

    return TASK_6_E_OK;
}


/*------------------------------------------------------- Main -------------------------------------------------------*/

int main(int argc, char *argv[]) {
    task_6_error_code_t error_code;

    program_configuration_t configuration;
    if ((error_code = parse_cl_arguments(argc - 1, argv + 1, &configuration))) {
        print_task_6_error(error_code);
        return -1;
    }

    if ((error_code = run_strings_comparer(configuration))) {
        print_task_6_error(error_code);
        return -1;
    }

    return 0;
}
