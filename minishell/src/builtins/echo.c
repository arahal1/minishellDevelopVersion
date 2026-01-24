/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouk <adbouk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 14:51:06 by adbouk            #+#    #+#             */
/*   Updated: 2026/01/24 16:53:35 by adbouk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int get_argv_len (t_cmd *cmds)
{
    int i;

    i = 0;
    while (cmds->argv[i])
        i++;
    return (i);
}

int exec_echo(t_cmd *cmds)
{
    int size;
    int is_n;
    int i;
    int j;

    size = get_argv_len(cmds);
    if (size == 1)
    {
        printf("\n");
        return (0);
    }
    i = 1;
    is_n = 1;
    while (cmds->argv[i] && cmds->argv[i][0] == '-')
    {
        j = 1;
        while (cmds->argv[i][j] == 'n')
            j++;
        if (j == 1 || cmds->argv[i][j] != '\0')
            break;
        i++;
    }
    if (i > 1)
        is_n = 0;
    while (cmds->argv[i])
    {
        printf("%s", cmds->argv[i]);
        if (cmds->argv[i + 1])
            printf(" ");
        i++;
    }
    if (is_n)
        printf("\n");
    return (0);
}
