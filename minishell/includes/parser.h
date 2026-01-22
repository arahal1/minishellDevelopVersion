#ifndef PARSER_H
# define PARSER_H

# include <stdlib.h>
# include <unistd.h>
# include "../libft/libft.h"
# include "lexer.h"   // for t_token

typedef enum e_redir_type
{
    R_IN,
    R_OUT,
    R_APPEND,
    R_HEREDOC
}   t_redir_type;

typedef struct s_redir
{
    t_redir_type        type;
    char                *target;
    int                 heredoc_fd;
    struct s_redir      *next;
}   t_redir;

typedef struct s_cmd
{
    char            **argv;
    t_redir         *redirs;
    struct s_cmd    *next;
}   t_cmd;

typedef struct s_word { char *str; struct s_word *next; } t_word;

t_cmd   *parse_token(t_token *token);
void    print_cmd_list(t_cmd *head);
void    free_cmds(t_cmd *cmd);

#endif
