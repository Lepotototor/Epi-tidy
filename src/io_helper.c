#include "io_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str_list.h"

void help(FILE *file, const char *pg_name)
{
    if (file == stdout)
    {
        fprintf(file, "%s\n\n", helpful);
    }

    fprintf(file, "Usage: %s <files>\n", pg_name);
    fprintf(file, "-s\t set max_size, default to 30\n");
    fprintf(file, "-a\t set max_args, default to 4\n");
    fprintf(file, "-f\t set max_function_per file, default to 10\n");
    fprintf(file, "-h\t display a helpful message\n");

    if (file == stderr)
        exit(1);
    else
        exit(0);
}

static void modify_arg(const char *arg, const char *arg_name, size_t *var,
                       str_list *files)
{
    *var = atoi(arg);
    if (*var == 0)
    {
        fprintf(stderr, "Invalid number, %s should be positive integer\n",
                arg_name);
        str_list_destroy(files);
        exit(2);
    }
}

str_list *check_args(char **argv, size_t *size_max, size_t *max_args,
                     size_t *max_func)
{
    str_list *files = NULL;

    for (int i = 1; argv[i]; i++)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            str_list_destroy(files);
            help(stdout, argv[0]);
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            modify_arg(argv[i], "max lines number", size_max, files);
        }
        else if (strcmp(argv[i], "-a") == 0)
        {
            modify_arg(argv[i], "max arguments number", max_args, files);
        }
        else if (strcmp(argv[i], "-f") == 0)
        {
            modify_arg(argv[i], "max functions number", max_func, files);
        }
        else
        {
            files = str_list_append(files, argv[i]);
        }
    }

    return files;
}
