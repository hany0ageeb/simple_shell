#include "token.h"
#include <stdlib.h>
#include <errno.h>
/**
 * create_token_list - create token list object
 * Return: a pointer to newly create token_list object or NULL
 */
struct token_list *create_token_list()
{
	struct token_list *lst = malloc(sizeof(struct token_list));

	if (lst != NULL)
	{
		lst->head = NULL;
		errno = 0;
	}
	else
	{
		errno = ENOMEM;
	}
	return (lst);
}
/**
 * add_token_to_list - add token to token list
 * @lst: token list object
 * @token: token
 * Return: void
 */
void add_token_to_list(struct token_list *lst, struct token *token)
{
	struct token_node *node = NULL, *ptr = NULL;

	if (lst != NULL)
	{
		node = create_token_node(token);
		if (node != NULL)
		{
			if (lst->head == NULL)
			{
				lst->head = node;
			}
			else
			{
				ptr = lst->head;
				while (ptr->next != NULL)
					ptr = ptr->next;
				ptr->next = node;
			}
		}
	}
}
/**
 * free_token_list - free token list object
 * @lst: token list object
 * Return: void
 */
void free_token_list(struct token_list **lst)
{
	struct token_node *end = NULL, *pre = NULL;

	if (lst == NULL)
		errno = EINVAL;
	else
	{
		if (*lst != NULL)
		{
			while ((*lst)->head != NULL)
			{
				pre = NULL;
				end = (*lst)->head;
				while (end->next != NULL)
				{
					pre = end;
					end = end->next;
				}
				free_token_node(&end);
				if (pre != NULL)
					pre->next = NULL;
				else
					(*lst)->head = NULL;
			}
			free(*lst);
			*lst = NULL;
		}
	}
}

