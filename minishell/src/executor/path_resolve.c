/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouk <adbouk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 17:06:32 by adbouk            #+#    #+#             */
/*   Updated: 2026/01/15 21:10:16 by adbouk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


static int  is_dir(const char *path)
{
    struct stat st;

    if (stat(path, &st) == -1)
        return (0);
    return (S_ISDIR(st.st_mode));
}

int has_slash(char *str)
{
    int i;
 
    i = 0;
    while (str[i])
    {
        if (str[i] == '/')
            return (1);
        i++;
    }
    return (0);
}

char    *cmd_with_slash (char *name, int *out_status)
{
    char *str;

    if (access(name, F_OK) == -1)
    {
        *out_status = 127;
        return (NULL);
    }
    if (is_dir(name))
        return (*out_status = 126, NULL);
    if (access(name, X_OK) == -1)
    {
        *out_status = 126;
        return (NULL);
    }
    str = ft_strdup(name);
    if (!str)
        return (NULL);
    return (str);
}

char    *return_path (char **paths, char *name, int *out_status)
{
    int i;
    char *tmp;
    char *tmp1;

    i = -1;
    *out_status = 127;
    while (i++, paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        if (!tmp)
            return (NULL);
        tmp1 = ft_strjoin(tmp, name);
        free(tmp);
        if (!tmp1)
            return (NULL);
        if (access (tmp1, F_OK) == 0)
            *out_status = 0;
        if (!is_dir(tmp1) && access (tmp1, X_OK) == 0)
            return (tmp1);
        free(tmp1);
    }
    if (*out_status == 0)
        *out_status = 126;
    return (NULL);
}

char    *cmd_without_slash(t_shell *sh, char *name, int *out_status)
{
    char **paths;
    char *path;
    char *tmp;

    tmp = env_get(sh, "PATH");
    if (!tmp || tmp[0] == '\0')
    {
        *out_status = 127;
        return (NULL);
    }
    paths = ft_split(tmp, ':');
    if (!paths)
        return (NULL);
    path = return_path(paths, name, out_status);
    free_2d(paths);
    return (path);
}

char *resolve_cmd_path(t_shell *sh, char *name, int *out_status)
{
    if (!name || name[0] == '\0')
        return (*out_status = 127, NULL);
    *out_status = 0;
    if (has_slash(name))
        return (cmd_with_slash(name, out_status));
    return (cmd_without_slash(sh, name, out_status));
}
