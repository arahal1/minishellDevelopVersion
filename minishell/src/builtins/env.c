#include <unistd.h>
#include "../../includes/minishell.h"

int	exec_env(t_shell *sh, t_cmd *cmd)
{
	t_env	*cur;

	if (!sh)
		return (1);
	if (cmd && cmd->argv && cmd->argv[1])
	{
		perror("minishell: env: too many arguments\n");
		return (1);
	}
	cur = sh->env;
	while (cur)
	{
		if (cur->has_value)
		{
			printf ( "%s", cur->key);
			write(1, "=", 1);
			if (cur->value)
				printf ( "%s", cur->value);
            printf("\n");
        }
        cur = cur->next;
	}
	return (0);
}