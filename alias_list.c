#include "alias_list.h"
#include "alias.h"
#include "io.h"
#include "string.h"
#include <stdlib.h>
/**
 * add_to_alias_list - add alias to list
 * @head: head node
 * @a: alias
 * Return: newly added node pointer
 */
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
/**
 * add_or_update_alias_list - add or update alias
 * @head: list head node
 * @name: alias name
 * @value: new value
 * Return: pointer to alias node
 */
alias_node_t *add_or_update_alias_list(alias_node_t **head,
		char *name, char *value)
{
	alias_t *al = NULL;

	if (IS_NULL_OR_EMPTY(name))
		return (NULL);
	if (*head == NULL)
	{
		return (add_to_alias_list(head, create_alias(name, value)));
	}
	else
	{
		al = find_alias(name, *head);
		if (al == NULL)
			return (add_to_alias_list(head, create_alias(name, value)));
		if (al->value != NULL)
			free(al->value);
		al->value = value;
		if (name != NULL)
			free(name);
	}
	return (NULL);
}
/**
 * free_alias_list - free list memory
 * @head: head node
 * Return: void
 */
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
			free_alias(&end->data);
			free(end);
			if (pre_end == NULL)
				*head = NULL;
			else
				pre_end->next = NULL;
		}
	}
}
/**
 * find_alias - search alias list by name
 * @name: alias name
 * @head: list head node
 * Return: alias if found or NULL
 */
alias_t *find_alias(const char *name, alias_node_t *head)
{
	while (head != NULL)
	{
		if (str_equals(head->data->name, name) == TRUE)
			return (head->data);
		head = head->next;
	}
	return (NULL);
}
