#ifndef ALIAS_LIST_H_
#define ALIAS_LIST_H_
#include "alias.h"
/**
 * struct alias_node - alias node
 * @data: alias
 * @next: next node
 */
typedef struct alias_node
{
	struct alias *data;
	struct alias_node *next;
} alias_node_t;
alias_node_t *add_to_alias_list(alias_node_t **head, alias_t *a);
alias_node_t *add_or_update_alias_list(alias_node_t **head,
		char *name, char *value);
void free_alias_list(alias_node_t **head);
void print_alias_all(alias_node_t *head);
void print_alias(const char *name, alias_node_t *head);
#endif

