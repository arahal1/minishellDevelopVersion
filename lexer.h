#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
# include "../libft/libft.h"

typedef enum e_toktype
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_APPEND,
	TOK_HEREDOC
}	t_toktype;

typedef struct s_token
{
	t_toktype		type;
	char			*value;
	struct s_token	*next;
}	t_token;

/* token utils */
t_token	*tok_new(t_toktype type, char *value);
void	tok_add_back(t_token **head, t_token *node);
void	tok_free_all(t_token *head);
void	tok_debug_print(t_token *head);

/* lexer */
t_token	*lex_line(const char *line, int *out_status);

#endif

