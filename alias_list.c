#include "alias_list.h"
#include "alias.h"
#include "io.h"
#include <stdlib.h>
alias_node_t *add_to_alias_list(alias_node_t **head, alias_t *a)
{
    alias_node_t *node = NULL, *end = NULL;
    if (a == NULL)
    return (NULL);
    node = malloc(sizeof(alias_node_t));
    if (node != NULL)
    {
        node->data = a;
        node->next = NULL;
        if (*head == NULL)
        {
            *head = node;
        }
        else
        {
            end = *head;
            while (end->next != NULL)
            {
                end = end->next;
            }
            end->next = node;
        }
    }
    return (node);
}
alias_node_t *add_or_update_alias_list(alias_node_t **head,char *name, char *value)
{
    alias_node_t *v;
    if (IS_NULL_OR_EMPTY(name))
    return (NULL);
    if (*head == NULL)
    {
        return (add_to_alias_list(head, create_alias(name, value)));
    }
    else
    {
        v = *head;
        do {
            if (str_equals(v->data->name, name))
            {
                if (v->data->value != NULL)
                free(v->data->value);
                v->data->value = value;
                return (v);
            }
        } while(v != NULL);
        return (add_to_alias_list(head, create_alias(name, value)));
    }
}
void free_alias_list(alias_node_t **head)
{
    alias_node_t *end, *pre_end;

    if (head != NULL && *head != NULL)
    {
        while (*head != NULL)
        {
            end = *head;
            pre_end = NULL;
            while (end->next != NULL)
            {
                pre_end = end;
                end = end->next;
            }
            free_alias(end->data);
            free(end);
            if (pre_end == NULL)
                *head = NULL;
            else
                pre_end->next = NULL;
        }
    }
}
void print_alias_all(alias_node_t *head)
{
    while (head != NULL)
    {
        _puts("alias ");
        _puts(head->data->name);
        _puts("=");
        _putc('\'');
        _puts(head->data->value);
        _puts("\'\n");
        head = head->next;
    }
}
void print_alias(const char *name, alias_node_t *head)
{
    while (head != NULL)
    {
        if (str_equals(name, head->data->name))
        {
            _puts("alias ");
        _puts(head->data->name);
        _puts("=");
        _putc('\'');
        _puts(head->data->value);
        _puts("\'\n");
        break;
        }
        head = head->next;
    }
}