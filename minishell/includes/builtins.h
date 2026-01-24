/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: you <you@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23                                 #+#    #+#             */
/*   Updated: 2026/01/23                                 #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <errno.h>
# include <string.h>

typedef struct s_shell  t_shell;
typedef struct s_cmd    t_cmd;

/* builtin IDs */
typedef enum e_builtin
{
	BI_NONE = 0,
	BI_ECHO,
	BI_CD,
	BI_PWD,
	BI_EXPORT,
	BI_UNSET,
	BI_ENV,
	BI_EXIT
}	t_builtin;

int exec_echo(t_cmd *cmds);
int exec_cd(t_shell *sh, t_cmd *cmd);
int exec_pwd(void *sh);
int exec_env(t_shell *sh, t_cmd *cmd);

#endif
