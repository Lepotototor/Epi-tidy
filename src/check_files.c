#define _POSIX_C_SOURCE 200809L

#include "check_files.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "io_helper.h"

static char *trim(char *line)
{
    char *s = line;
    while (isspace(*s) || isblank(*s))
        s++;

    if (*s == 0)
        return s;

    char *e = s + strlen(s) - 1;
    while (isspace(*e) || isblank(*e))
    {
        *e = 0;
        e--;
    }

    return s;
}

static bool is_countable_func(char *line)
{
    return strstr(line, "static ") == NULL;
}

static size_t get_arg_count(char *line)
{
    size_t res = 0;

    char *machalla = strchr(line, ',');
    while (machalla)
    {
        res = res ? res + 1 : 2;
        machalla = strchr(machalla + 1, ',');
    }

    return res;
}

static char *get_func_name(char *line)
{
    char *par = strchr(line, '(');
    *par = 0;
    char *space = strrchr(line, ' ');
    *par = '(';

    return strndup(space, par - space);
}

static char *concatenete(char *s1, char *s2)
{
    size_t l1 = strlen(s1);
    size_t l2 = strlen(s2);

    char *res = malloc(l1 + l2 + 1);
    if (res == NULL)
        return NULL;

    strcpy(res, s1);
    strcpy(res + l1, s2);

    return res;
}

static bool get_result(char *func_name, size_t line_count, size_t arg_count,
                       size_t max_line, size_t max_args)
{
    (void)max_line;
    (void)max_args;
    if (line_count > max_line || arg_count > max_args)
    {
        printf("%s Function %s%s%s  exceed\n%s", RED, YELLOW, func_name, RED,
               NC);
        printf("%sLines count: %s%zu%s\n", CYAN, RED, line_count, CYAN);
        printf("Args count: %s%zu%s\n%s", RED, arg_count, RED, NC);

        putchar('\n');

        return true;
    }

    return false;
}

size_t check_file(FILE *file, size_t max_line, size_t max_args, size_t max_func)
{
    char *line = NULL;
    size_t n;

    size_t nb_invalid = 0;

    size_t bra_count = 0;
    size_t line_count = 0;
    size_t arg_count = 0;
    size_t func_count = 0;

    char *func_name = NULL;

    bool in_comment = false;
    bool in_string = false;
    bool in_function = false;

    while ((getline(&line, &n, file)) != EOF)
    {
        bool countable_line = 0;
        char *trim_line = trim(line);

        if (trim_line[0] == 0 || trim_line[0] == '#')
            continue;

        for (size_t i = 0; trim_line[i]; i++)
        {
            char c = trim_line[i];
            char n = trim_line[i + 1];

            if (!in_comment && c == '/' && n == '/')
                break;
            else if (!in_comment && c == '/' && n == '*')
                in_comment = true;
            else if (!in_comment && c == '"')
                in_string = !in_string;
            else if (in_comment && c == '*' && n == '/')
                in_comment = false;
            else if (!in_comment && (isalnum(c) || c == ';'))
                countable_line = true;
            else if (!in_comment && c == '{' && n != '\'')
                bra_count++;
            else if (!in_comment && in_function && c == '}' && n != '\'')
                bra_count--;
        }

        if (in_function && countable_line)
        {
            line_count++;
        }
        else if (in_function && bra_count == 0)
        {
            in_function = false;
            if (get_result(func_name, line_count, arg_count, max_line,
                           max_args))
                nb_invalid++;

            line_count = 0;
            arg_count = 0;

            free(func_name);
            func_name = NULL;
        }
        else if (!in_function && (bra_count || countable_line)
                 && strchr(trim_line, '(') && strchr(trim_line, ' ') != NULL)
        {
            while (strchr(trim_line, ')') == NULL)
            {
                char *buffer = NULL;
                getline(&buffer, &n, file);
                char *new_line = concatenete(trim_line, buffer);
                free(trim_line);
                free(buffer);

                line = new_line;
                trim_line = trim(new_line);
            }

            in_function = true;
            func_name = get_func_name(trim_line);
            arg_count = get_arg_count(trim_line);

            if (is_countable_func(trim_line))
            {
                func_count++;
            }
        }

        free(line);
        line = NULL;
    }
    free(line);

    if (func_name
        && get_result(func_name, line_count, arg_count, max_line, max_args))
        nb_invalid++;

    free(func_name);

    if (func_count > max_func)
    {
        printf("%s Numbers of function exceed\n", RED);
        printf("Number of functions found: %s%zu\n%s", YELLOW, func_count, NC);
    }

    return nb_invalid;
}
