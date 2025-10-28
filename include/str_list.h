#ifndef STR_LIST_H
#define STR_LIST_H

typedef struct str_list
{
    char *s;
    struct str_list *next;
} str_list;

str_list *str_list_append(str_list *l, char *str);

void str_list_destroy(str_list *l);

#endif /* ! STR_LIST_H */
