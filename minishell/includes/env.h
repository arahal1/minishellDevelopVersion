#ifndef ENV_H
# define ENV_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "../libft/libft.h"

typedef struct s_env
{
    char            *key;
    char            *value;
    int             has_value;
    struct s_env    *next;
}   t_env;

typedef struct s_shell
{
    t_env   *env;
    int     last_status;
}   t_shell;

/* init */
int     env_init_from_envp(t_shell *sh, char **envp);

/* lookup */
char    *env_get(t_shell *sh, const char *key);
int     env_has(t_shell *sh, const char *key);

/* mutation */
int     env_set(t_shell *sh, const char *key, const char *value, int has_value);
int     env_unset(t_shell *sh, const char *key);

/* execve bridge */
char    **env_to_envp(t_shell *sh);
void    envp_free(char **envp);

/* debug */
void    env_print(t_shell *sh);

/* cleanup (call on exit) */
void    env_clear(t_env **env);

t_env   *env_find(t_env *env, const char *key);
char    *env_find_alias(t_env *env, const char *key);
int     key_match(const char *a, const char *b);
int     count_printable(t_env *env);

#endif
