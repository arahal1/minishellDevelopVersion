#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "env.h"
# include "lexer.h"
# include "parser.h"
# include "exec.h"

int ms_loop(char **envp);
int exec_external(t_shell *sh, t_cmd *cmd, char **envp, char *path);
int exec_one_pipeline(t_shell *sh, t_cmd *cmd);
char *resolve_cmd_path(t_shell *sh, char *name, int *out_status);
void free_2d(char **str);

#endif
