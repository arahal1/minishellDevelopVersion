#include "../includes/minishell.h"

static int  open_redir_file(t_redir *r)
{
    if (r->type == R_IN)
        return (open(r->target, O_RDONLY));
    if (r->type == R_OUT)
        return (open(r->target, O_CREAT | O_TRUNC | O_WRONLY, 0644));
    if (r->type == R_APPEND)
        return (open(r->target, O_CREAT | O_APPEND | O_WRONLY, 0644));
    return (-1);
}
 
int redir_dup_to_fd(int fd, t_redir *r)
{
    if (r->type == R_IN)
        return (dup2(fd, STDIN_FILENO));
    if (r->type == R_OUT || r->type == R_APPEND)
        return (dup2(fd, STDOUT_FILENO));
    return (-1);
}

int apply_single_redir(t_shell *sh, t_redir *r)
{
    int fd;
 
    if (r->type == R_HEREDOC)
        return (0);
    fd = open_redir_file(r);
    if (fd < 0)
    {
        printf("minishell:");
        perror(r->target);
        sh->last_status = 1;
        return (1);
    }
    if (redir_dup_to_fd(fd, r) < 0)
    {
        close (fd);
        printf("minishell:");
        perror("dup2");
        sh->last_status = 1;
        return (1);
    }
    close(fd);
    return (0);
}

int apply_redirs(t_shell *shell, t_cmd *cmd)
{
    t_redir *cur_r;

    if (cmd == NULL)
        return (1);
    cur_r = cmd->redirs;
    while (cur_r)
    {
        if (apply_single_redir(shell, cur_r))
            return (1);
        cur_r = cur_r->next;
    }
    return (0);
}