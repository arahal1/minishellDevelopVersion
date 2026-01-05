#include "../../include/env.h"

static char	*join_kv(const char *k, const char *v)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(k, "=");
	if (!tmp)
		return (0);
	res = ft_strjoin(tmp, v);
	free(tmp);
	return (res);
}

static void	free_partial(char **arr, int i)
{
	while (i > 0)
	{
		i--;
		free(arr[i]);
	}
	free(arr);
}

static int	fill_envp(char **arr, t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		if (env->has_value)
		{
			arr[i] = join_kv(env->key, env->value);
			if (!arr[i])
				return (free_partial(arr, i), -1);
			i++;
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (0);
}

char	**env_to_envp(t_shell *sh)
{
	char	**arr;

	if (!sh)
		return (0);
	arr = (char **)malloc(sizeof(char *) * (count_printable(sh->env) + 1));
	if (!arr)
		return (0);
	if (fill_envp(arr, sh->env) < 0)
		return (0);
	return (arr);
}

void	envp_free(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}