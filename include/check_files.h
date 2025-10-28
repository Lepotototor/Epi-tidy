#ifndef CHECK_FILES_H
#define CHECK_FILES_H

#include <stddef.h>
#include <stdio.h>

#define DEFAULT_MAX_SIZE 30
#define DEFAULT_MAX_ARGS 4
#define DEFAULT_MAX_FUNC 10

int check_file(FILE *file, size_t max_size , size_t max_args, size_t max_func);

#endif /* ! CHECK_FILES_H */
