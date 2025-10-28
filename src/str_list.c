#include "str_list.h"

#include <stdio.h>
#include <stdlib.h>

str_list *str_list_append(str_list *l, char *str)
{
    str_list *new_node = malloc(sizeof(str_list));

    if (new_node == NULL)
    {
        fprintf(stderr, "My gaaad Malloc failed \n");
        exit(6942);
    }

    new_node->s = str;
    new_node->next = NULL;

    if (l == NULL)
        return new_node;

    struct str_list *last = l;
    for (; last && last->next; last = last->next)
        ;

    last->next = new_node;

    return l;
}

void str_list_destroy(str_list *l)
{
    if (l == NULL)
        return;

    str_list_destroy(l->next);
    free(l);
}
