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

    size_t max_size = 0;
    size_t max_args = 0;
    size_t max_func = 0;

    str_list *files = check_args(argv, &max_size, &max_args, &max_func);

    for (str_list *f = files; f; f = f->next)
    {
        FILE *file = fopen(f->s, "r");

        if (file == NULL)
            fprintf(stderr, "Can't open file '%s' \n\n", f->s);

        // check_file(file, max_size, max_args, max_func);

        printf("File: %s\n", f->s);

        fclose(file);
    }

    printf("Size max: %zu\n", max_size);
    printf("Num args max: %zu\n", max_args);
    printf("Num func max: %zu\n", max_func);

    str_list_destroy(files);

    return 0;
}
