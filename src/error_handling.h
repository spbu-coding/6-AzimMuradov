/*
 * Task #6
 *
 * Azim Muradov, group #171
 */

#ifndef TASK_6_ERROR_HANDLING_H
#define TASK_6_ERROR_HANDLING_H


typedef enum {
    TASK_6_E_OK,
    TASK_6_E_WRONG_CL_ARGS_FORMAT,
    TASK_6_E_MEM_ALLOC,
    TASK_6_E_FOPEN,
    TASK_6_E_FREAD_FEOF,
    TASK_6_E_FREAD_FERROR,
    TASK_6_E_FWRITE_FERROR,
    TASK_6_E_OTHER,
} task_6_error_code_t;

extern const char *TASK_6_ERROR_MESSAGES[];


extern task_6_error_code_t curr_error;


void print_task_6_error(task_6_error_code_t error_code);

#endif // TASK_6_ERROR_HANDLING_H
