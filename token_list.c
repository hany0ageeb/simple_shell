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
/**
 * copy_token_node - copy token node
 * @src: source token node
 * Return: NULL if src is NULL or a deep copy of src
 */
token_node_t *copy_token_node(const token_node_t *src)
{
	token_node_t *node = NULL;

	if (src != NULL)
	{
		node = create_token_node(copy_token(src->token));
		node->next = NULL;
	}
	return (node);
}
/**
 * copy_token_list - copy token list starting from head
 * @head: start point of copying from the list
 * @end: where copying end!
 * Return: a copy of token list
 */
token_list_t *copy_token_list(const token_node_t *head,
		const token_node_t *end)
{
	token_list_t *lst = NULL;
	token_node_t *v;

	if (head == NULL)
		return (NULL);
	lst = create_token_list();
	lst->head = copy_token_node(head);
	if (end == NULL)
	{
		while (head->next != NULL)
		{
			v->next = copy_token_node(head->next);
			v = v->next;
			head = head->next;
		}
	}
	else
	{
		while (head->next != NULL && head != end->next)
		{
			v->next = copy_token_node(head->next);
			v = v->next;
			head = head->next;
		}
	}
	return (lst);
}

