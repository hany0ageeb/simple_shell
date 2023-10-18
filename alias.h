#ifndef ALIAS_H_
#define ALIAS_H_
/**
 * struct alias - alias
 * @name: alias name
 * @value: alias value
 */
typedef struct alias
{
	char *name;
	char *value;
} alias_t;
alias_t *create_alias(char *name, char *value);
void free_alias(alias_t **a);
#endif
