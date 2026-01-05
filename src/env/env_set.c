#include "../../include/env.h"

static void env_add_back(t_env **env, t_env *new)
{
	t_env	*cur;

	if (!env || !new)
		return ;
	if (!*env)
	{
		*env = new;
		return ;
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

static t_env   *env_new(const char *key, const char *value, int has_value)
{
    t_env   *new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = NULL;
	new->has_value = has_value;
	new->next = NULL;
	if (!new->key)
		return (free(new), NULL);
	if (has_value)
	{
		new->value = ft_strdup(value);
		if (!new->value)
			return (free(new->key), free(new), NULL);
	}

	return (new);
}

static int	update_node(t_env *node, const char *value, int has_value)
{
	char	*new_val;

	node->has_value = has_value;
	if (!has_value)
		return(free(node->value), node->value = NULL, 0);
	new_val = ft_strdup(value);
	if (!new_val)
		return(-1);
	free(node->value);
	node->value = new_val;
	return (0);
}

int env_set(t_shell *sh, const char *key, const char *value, int has_value)
{
	t_env	*n;
	char	*new_val;

	if (!sh || !key)
		return (-1);
	new_val = (char *)value;
	if (has_value && !new_val)
		new_val= "";
	n = env_find(sh->env, key);
	if (n)
		return (update_node(n, new_val, has_value));
	n = env_new(key, new_val, has_value);
	if (!n)
		return (-1);
	env_add_back(&sh->env, n);
	return (0);
}