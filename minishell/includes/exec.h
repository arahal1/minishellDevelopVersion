#ifndef EXEC_H
# define EXEC_H

/* forward declarations are enough in the header */
typedef struct s_shell t_shell;
typedef struct s_cmd   t_cmd;

typedef struct s_stdio_bak
{
    int in;
    int out;
}   t_stdio_bak;

int     apply_redirs(t_shell *sh, t_cmd *cmd);
int     stdio_backup(t_stdio_bak *bak);
void    stdio_restore(t_stdio_bak *bak);

#endif
