/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouk <adbouk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 09:23:22 by adbouk            #+#    #+#             */
/*   Updated: 2026/01/22 19:56:41 by adbouk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/env.h"
#include "../includes/exec.h"

void create (char *str)
{
    int i = 0;
    while (i < 4)
    {
        str[i] = 'a';
        i++;
    }
    str[i] = '\0';
}

void print_str_str (char *str)
{
    printf("%s\n", str);
}

void    free_2d(char **str)
{
    int i;

    i = 0;
    if (!str)
        return;
    while (str && str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
}

void    print_argv (char **str, int size)
{
    int i = 1;
    while (i < size)
    {
        printf("%s", str[i]);
        i++;
    }
}

void printtt (char **strs)
{
    int i = 0;
    while (strs[i])
    {
        printf("%s\n", strs[i]);
        i++;
    }
}

int	main(int argc, char **argv, char **envp)
{
    char *str;
    t_shell sh;
    t_token *head;
    t_cmd *p;

    (void)argc;
    (void)argv;
    str = "wc -l < files|\0";
    head = NULL;
    if (env_init_from_envp(&sh, envp) == -1)
        return (0);
    head = init_token(str, &sh);
    p = parse_token(head);
    // print_cmd_list(p);
    apply_redirs(&sh, p);
    exec_one_pipeline(&sh, p);
    free_cmds(p);
    env_clear(&sh.env);
    return (0);
}
