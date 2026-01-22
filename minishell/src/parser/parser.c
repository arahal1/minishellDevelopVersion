#include "../../includes/parser.h"

int is_redirToken(t_toktype type)
{
    if (type == TOK_REDIR_IN || type == TOK_REDIR_OUT || type == TOK_APPEND || type == TOK_HEREDOC)
	    return (1);
    return (0);
}

t_redir_type return_redir_type(t_toktype type)
{
	if (type == TOK_REDIR_IN)
		return (R_IN);
	if (type == TOK_REDIR_OUT)
		return (R_OUT);
	if (type == TOK_APPEND)
		return (R_APPEND);
	if (type == TOK_HEREDOC)
		return (R_HEREDOC);
	return (0);
}

t_redir	*redir_new(t_redir_type type, char *target)
{
	t_redir *new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->type = type;
	new->target = ft_strdup(target);
	if (!new->target)
	{
		free(new);
		return (NULL);
	}
	new->heredoc_fd = -1;
	new->next = NULL;
	return (new);
}

t_redir	*insert_redir_at_tail(t_redir *redir, t_redir *new_redir)
{
	t_redir	*cur;

	if (!redir)
		return (new_redir);
	cur = redir;
	while (cur->next)
		cur = cur->next;
	cur->next = new_redir;
	return (redir);
}

void	free_cmd_args(char	**argv)
{
	int i;

	if (!argv)
		return;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
	argv = NULL;
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;
	t_redir	*tmp2;

	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		free_cmd_args(tmp->argv);
		while (tmp->redirs)
		{
			tmp2 = tmp->redirs;
			tmp->redirs = tmp->redirs->next;
			free(tmp2->target);
			if (tmp2->heredoc_fd != -1)
				close(tmp2->heredoc_fd);
			free(tmp2);
		}
		free(tmp);
	}
}
void	free_argv(char **argv, int count)
{
	for (int i = 0; i < count; i++)
		free(argv[i]);
	free(argv);
}
char	**convert_to_argv(t_word *words)
{
	int	argc;
	char	**argv;
	t_word	*cur;

	argc = 0;
	cur = words;
	while (cur)
	{
		argc++;
		cur = cur->next;
	}
	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL);
	cur = words;
	for (int i = 0; i < argc; i++)
	{
		argv[i] = ft_strdup(cur->str);
		if (!argv[i])
			return (free_argv(argv, i), NULL);
		cur = cur->next;
	}
	argv[argc] = NULL;
	return (argv);
}


t_cmd	*insert_cmd_at_tail(t_cmd *cmd, t_cmd *new_cmd)
{
	t_cmd	*cur;

	if (!cmd)
		return (new_cmd);
	cur = cmd;
	while (cur->next)
		cur = cur->next;
	cur->next = new_cmd;
	return (cmd);
}

int init_new_cmd (t_cmd **new_cmd)
{
	*new_cmd = malloc (sizeof(t_cmd));
	if (!*new_cmd)
		return (1);
	(*new_cmd)->argv = NULL;
	(*new_cmd)->next =NULL;
	(*new_cmd)->redirs = NULL;
	return (0);
}
int init_cmd (t_cmd **new_cmd, t_cmd **cmd)
{
	*new_cmd = malloc (sizeof(t_cmd));
	if (!*new_cmd)
		return (1);
	(*new_cmd)->argv = NULL;
	(*new_cmd)->next =NULL;
	(*new_cmd)->redirs = NULL;
	(*cmd) = NULL;
	return (0);
}

t_word	*word_new(char *value)
{
	t_word *new;

	new = malloc(sizeof(t_word));
	if (!new)
		return (NULL);
	new->str = ft_strdup(value);
	if (!new->str)
	{
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

t_word	*insert_word_at_tail(t_word *word, t_word *new_word)
{
	t_word	*cur;

	if (!word)
		return (new_word);
	cur = word;
	while (cur->next)
		cur = cur->next;
	cur->next = new_word;
	return (word);
}

void free_words(t_word *word)
{
	t_word	*cur;

	while (word)
	{
		cur = word;
		word = word->next;
		free(cur->str);
		free(cur);
	}
}
void	free_redirs(t_redir *r)
{
	t_redir	*next;

	while (r)
	{
		next = r->next;
		free(r->target);
		free(r);
		r = next;
	}
}
void	free_argv2(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}
void	free_cmd_one(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_argv2(cmd->argv);
	free_redirs(cmd->redirs);
	free(cmd);
}
void	free_cmds2(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		free_cmd_one(cmd);
		cmd = next;
	}
}

void	free_parser(t_token *token, t_cmd *cmd, t_word *words)
{
	free_token(token);
	free_words(words);
	free_cmds(cmd);
}


char	**alloc_null_args(void)
{
	char **args;

	args = malloc(1 * (sizeof(char *)));
	if (!args)
		return (NULL);
	args[0] = NULL;
	return (args);
}

t_cmd	*parse_token(t_token *token)
{
	t_cmd		*cmd;
	t_cmd		*new_cmd;
	t_token		*cur;
	t_word		*words;
	t_word		*new_word;

	if (token == NULL)
		return (NULL);
	words = NULL;
	if (!token)
		return (NULL);
	cur = token;
	if (init_cmd (&new_cmd, &cmd))
		return (free_token(token), NULL);
	while (cur)
	{
		if (cur->type == TOK_PIPE)
		{
			if (words)
			{
				new_cmd->argv = convert_to_argv(words);
				if (!new_cmd->argv)
				{
					free_cmds(new_cmd);
					free_parser(token, cmd, words);
					return (NULL);
				}
				free_words(words);
				words = NULL;
			}
			else
			{
				new_cmd->argv = alloc_null_args();
				if (!new_cmd->argv)
				{
					free_parser(token, cmd, words);
					free_cmds(new_cmd);
					return (NULL);
				}
			}
			cmd = insert_cmd_at_tail(cmd, new_cmd);
			if (init_new_cmd(&new_cmd))
				return (free_parser(token, cmd, words), NULL);
			
		}
		else if (is_redirToken(cur->type))
		{
			t_redir *redir = redir_new(return_redir_type(cur->type), cur->next->value); //first one for redir type next one for target
			if (!redir)
			{
				free_parser(token, cmd, words);
				free_cmds(new_cmd);
				return (NULL);
			}
			new_cmd->redirs = insert_redir_at_tail(new_cmd->redirs, redir);
			cur = cur->next;
		}
		else
		{
			new_word = word_new(cur->value);
			if (!new_word)
			{
				free_parser(token, cmd, words);
				free_cmds(new_cmd);
				return (NULL);
			}
			words = insert_word_at_tail(words, new_word);
		}
		cur = cur->next;
	}
	// I think I should initialize the new _cmd  but it is already intialized
	if (words)
	{
		new_cmd->argv = convert_to_argv(words);
		if (!new_cmd->argv)
		{
			free_cmds(new_cmd);
			free_parser(token, cmd, words);
			return (NULL);
		}
	}
	else
	{
		new_cmd->argv = alloc_null_args();
		if (!new_cmd->argv)
		{
			free_parser(token, cmd, words);
			free_cmds(new_cmd);
			return (NULL);
		}
	}
	cmd = insert_cmd_at_tail(cmd, new_cmd);
	free_words(words);
	free_token(token);
	return (cmd);
}