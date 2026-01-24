/* ************************************************************************** */
/*                                builtin_pwd.c                               */
/* ************************************************************************** */

#include <unistd.h>   // getcwd
#include <stdlib.h>   // free
#include <errno.h>    // errno
#include <string.h>   // strerror
#include "../../includes/minishell.h"

/*
** Adjust these helpers to your project:
** - ms_putendl_fd: write string + '\n' to fd
** - ms_putstr_fd:  write string to fd
** - ms_putendl_fd / ms_putstr_fd can be your libft ones.
** - env_get: returns value for key (does NOT allocate), or NULL
*/

static void	putstr_fd(const char *s)
{
	if (!s) return;
    printf("%s", s);
}

static void	putendl_fd(const char *s)
{
	putstr_fd(s);
    printf("\n");
}

int	exec_pwd(void *sh)
{
	char		*cwd;
	const char	*pwd_fallback;

	/* 1) Primary: real cwd */
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		putendl_fd(cwd);
		free(cwd);
		return (0);
	}

	/* 2) Fallback: use $PWD if getcwd fails (e.g., directory deleted) */
	pwd_fallback = env_get(sh, "PWD");
	if (pwd_fallback && *pwd_fallback)
	{
		putendl_fd(pwd_fallback);
		return (0);
	}

	/* 3) Error */
	putstr_fd("minishell: pwd: ");
	putendl_fd(strerror(errno));
	return (1);
}