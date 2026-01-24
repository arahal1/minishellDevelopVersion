#include <stdio.h>
#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    return (ms_loop(envp));
}