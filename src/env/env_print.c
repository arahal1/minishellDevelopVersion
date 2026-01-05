
#include "../../include/env.h"

static void	puts1(const char *s)
{
	if (!s)
		return ;
	write(1, s, ft_strlen(s));
}

void	env_print(t_shell *sh)
{
	t_env	*cur;

	if (!sh)
		return ;
	cur = sh->env;
	while (cur)
	{
		if (cur->has_value)
		{
			puts1(cur->key);
			puts1("=");
			puts1(cur->value);
			puts1("\n");
		}
		cur = cur->next;
	}
}
