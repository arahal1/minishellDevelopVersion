#include "../../includes/env.h"

int	count_printable(t_env *env)
{
	int count;

	count  = 0;
	while (env)
	{
		if (env->has_value)
			count++;
		env = env->next;
	}
	return (count);
}
