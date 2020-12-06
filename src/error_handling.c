/*
 * Task #6
 *
 * Azim Muradov, group #171
 */

#include "error_handling.h"

#include <stdio.h>


#define TASK_6_HELP_MSG                                                                                                  \
    "Strings comparer - help message:\n"                                                                                 \
    "----------------------------------------------------------------------------------------------------------------\n" \
    "You should call program according to this format:\n"                                                                \
    "\t ./strings_comparer [lines_count] [input_filename] [output_filename] [algorithm_name] [comparator_name]\n"        \
    "- lines_count - positive integer\n"                                                                                 \
    "\t number of lines in the input file\n"                                                                             \
    "- input_filename - string\n"                                                                                        \
    "\t name of the input file\n"                                                                                        \
    "- output_filename - string\n"                                                                                       \
    "\t name of the output file\n"                                                                                       \
    "- algorithm_name - string\n"                                                                                        \
    "\t name of the sorting algorithm, possible values: \"bubble\", \"insertion\", \"merge\", \"quick\", or \"radix\"\n" \
    "- comparator_name - string\n"                                                                                       \
    "\t name of the comparator function, possible values: \"asc\" (for ascending) or \"des\" (for descending)\n"


task_6_error_code_t curr_error = TASK_6_E_OK;

const char *TASK_6_ERROR_MESSAGES[] = {
    "everything is ok",
    "wrong command line arguments format\n\n" TASK_6_HELP_MSG,
    "can't allocate memory",
    "can't open the file",
    "can't read from the file - EOF",
    "can't read from the file",
    "can't write to the file",
    "unexpected error",
};


void print_task_6_error(task_6_error_code_t error_code) {
    fprintf(stderr, "Error occurred: %s\n", TASK_6_ERROR_MESSAGES[error_code]);
}


#undef TASK_6_HELP_MSG
