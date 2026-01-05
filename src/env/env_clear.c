#include "../../include/env.h"

void	env_clear(t_env **env)
{
	t_env	*next;

	if (!env)
		return ;
	while (*env)
	{
		next = (*env)->next;
		free((*env)->key);
		free((*env)->value);
		free(*env);
		*env = next;
	}
}
