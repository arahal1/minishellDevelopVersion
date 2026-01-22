#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
# include <stdlib.h>

/* forward declare: we only need the name t_shell here */
typedef struct s_shell t_shell;

typedef enum e_toktype
{
    TOK_WORD,
    TOK_PIPE,
    TOK_REDIR_IN,
    TOK_REDIR_OUT,
    TOK_APPEND,
    TOK_HEREDOC
}   t_toktype;

typedef struct s_token
{
    t_toktype       type;
    char            *value;
    struct s_token  *next;
} t_token;

t_token *init_token(char *line, t_shell *sh);
void    print_tokens(t_token *head);
void    free_token(t_token *head);

#endif
