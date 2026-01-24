/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouk <adbouk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 09:34:03 by adbouk            #+#    #+#             */
/*   Updated: 2026/01/23 18:46:21 by adbouk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_handle_line(char *line)
{
	printf("You typed: %s\n", line);
}

/* Minimal line reader for non-interactive mode (stdin not a tty).
   Reads until '\n' or EOF. Returns malloc'd string without '\n'.
   Returns NULL on EOF with no data or on read error. */
static char	*ms_read_stdin_line(void)
{
	char	c;
	size_t	len = 0;
	size_t	cap = 64;
	int		got_any = 0;
	char	*buf;
	ssize_t	r;

	buf = (char *)malloc(cap);
	if (!buf)
		return (NULL);
	while (1)
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r == 0)
			break;
		if (r < 0)
		{
			free(buf);
			return (NULL);
		}
		got_any = 1;
		if (c == '\n')
			break;
		if (len + 1 >= cap)
		{
			size_t	newcap = cap * 2;
			char	*newbuf = (char *)malloc(newcap);
			size_t	i = 0;
			if (!newbuf)
			{
				free(buf);
				return (NULL);
			}
			while (i < len)
			{
				newbuf[i] = buf[i];
				i++;
			}
			free(buf);
			buf = newbuf;
			cap = newcap;
		}
		buf[len++] = c;
	}
	if (!got_any)
	{
		free(buf);
		return (NULL);
	}
	buf[len] = '\0';
	return (buf);
}

int	ms_loop(char **envp)
{
	char    *line;
	int     interactive;
	t_shell sh;
	t_token *head;
	t_cmd *p;

	if (env_init_from_envp(&sh, envp) == -1)
		return (0);
	interactive = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
	while (1)
	{
		if (interactive)
			line = readline("minishell$ ");
		else
			line = ms_read_stdin_line();
		if (!line)
		{
			if (interactive)
				printf("exit\n");
			break;
		}
		if (interactive && line[0] != '\0')
		{
			add_history(line);
			head = NULL;
			head = init_token(line, &sh);
			p = parse_token(head);
			//print_cmd_list(p);
			// apply_redirs(&sh, p);
			// exec_one_pipeline(&sh, p);
			exec_pipeline(&sh, p);
			free_cmds(p);
		}
		//ms_handle_line(line);
		free(line);
	}
	if (interactive)
		rl_clear_history();
	env_clear(&sh.env);
	return (0);
}
