#include <stdlib.h>
#include "../../include/env.h"

static int	parse_one(t_shell *sh, const char *s)
{
	size_t	i;
	char	*key;
	char	*val;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	if (i == 0 || s[i] == '\0')
		return (0);
	key = ft_substr(s, 0, i);
	val = ft_strdup(s + i + 1);
	if (!key || !val)
	{
		free(key);
		free(val);
		return (-1);
	}
	if (env_set(sh, key, val, 1) < 0)
		return (free(key), free(val), -1);
	free(key);
	free(val);
	return (0);
}

int	env_init_from_envp(t_shell *sh, char **envp)
{
	int	i;

	if (!sh)
		return (-1);
	sh->env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		if (parse_one(sh, envp[i]) < 0)
			return (-1);
		i++;
	}
	return (0);
}
