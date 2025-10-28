#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "check_files.h"
#include "io_helper.h"
#include "str_list.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        help(stderr, argv[0]);
        return 1;
    }

    size_t max_size = DEFAULT_MAX_SIZE;
    size_t max_args = DEFAULT_MAX_ARGS;
    size_t max_func = DEFAULT_MAX_FUNC;

    str_list *files = check_args(argv, &max_size, &max_args, &max_func);

    printf("%sGonna check files with following values: %s\n", BLUE, NC);
    printf("%sSize max: %s%zu\n%s", GREEN, YELLOW, max_size, NC);
    printf("%sNum args max: %s%zu\n%s", GREEN, YELLOW, max_args, NC);
    printf("%sNum func max: %s%zu\n%s", GREEN, YELLOW, max_func, NC);
    putchar('\n');

    for (str_list *f = files; f; f = f->next)
    {
        FILE *file = fopen(f->s, "r");

        if (file == NULL)
        {
            fprintf(stderr, "%sCan't open file '%s' \n\n%s", RED, f->s, NC);
            continue;
        }

        char *filename = strrchr(f->s, '/');
        filename = filename ? filename + 1 : f->s;

        printf("%sChecking file: %s\n%s", PINK, filename, NC);

        size_t nb_invalid = check_file(file, max_size, max_args, max_func);

        if (nb_invalid == 0)
        {
            printf("%sNo error to report\n\n%s", CYAN, NC);
        }

        fclose(file);
    }

    str_list_destroy(files);

    return 0;
}
