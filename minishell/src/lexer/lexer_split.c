/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouk <adbouk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 15:54:36 by adbouk            #+#    #+#             */
/*   Updated: 2026/01/20 15:32:52 by adbouk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include "../../includes/env.h"

char *ft_str_empty(void)
{
    char *str;

    str = malloc(1 * sizeof(char));
    if (!str)
        return (NULL);
    str[0] = '\0';
    return (str);
}

char *ft_str_cut (char *str, int start, int end)
{
    int size;
    int i;
    int j;

    size = end - start + 1;
    if (size <= 1)
        return (ft_str_empty());
    char *new_str = malloc(size * sizeof(char));
    if (!new_str)
        return (NULL);
    i = 0;
    j = start;
    while (j != end)
        new_str[i++] = str[j++];
    new_str[i] = '\0';
    return (new_str);
}

void    free_token (t_token *head)
{
    t_token *tmp;

    while (head)
    {
        tmp = head->next;
        free(head->value);
        free (head);
        head = tmp;
    }
}

t_toktype   get_toktype(char *str)
{
    if (ft_strcmp(str, "|") == 0)
        return (TOK_PIPE);
    if (ft_strcmp (str, "<") == 0)
        return (TOK_REDIR_IN);
    if (ft_strcmp (str, ">") == 0)
        return (TOK_REDIR_OUT);
    if (ft_strcmp(str, ">>") == 0)
        return (TOK_APPEND);
    if (ft_strcmp (str, "<<") == 0)
        return (TOK_HEREDOC);
    return (TOK_WORD);
}

t_token *insert_at_tail (t_token *head, char *val, t_toktype type)
{
    t_token *cur;
    t_token *new_node;

    new_node = malloc(sizeof(t_token));
    if (!new_node)
    {
        free(val);
        free_token(head);
        return (NULL);
    }
    new_node->value = val;
    new_node->type = type;
    new_node->next = NULL;
    cur = head;
    if (!head)
        return (new_node);
    while (cur->next)
        cur = cur->next;
    cur->next = new_node;
    return (head);
}

char *ft_str_concat (char * s1, char * s2)
{
	char		*tab;
    int i;
    int j;
    size_t size;

    if (!s1)
		return (s2);
    if (!s2)
        return (s1);
    size = (ft_strlen(s1) + ft_strlen(s2));
    tab = malloc(sizeof(char) * (size + 1));
	if (!tab)
    {
        free(s1);
        free(s2);
        return (NULL);
    }
    i = 0;
    j = 0;
    while (s1[i])
        tab[j++] = s1[i++];
    i = 0;
    while (s2[i])
        tab[j++] = s2[i++];
    tab[j] = '\0';
    free(s1);
    free(s2);
    return (tab);
}

int is_white_space (char c)
{
    if ((c >= 9 && c <= 13) || c == 32)
        return (1);
    return (0);
}


// check for free after return in case of failure of malloc in any case for the entire phase
char    *ft_expand (char *line, int *i, int *j, t_shell *sh)
{
    char *tmp;
    char *str;

    (void)sh;
    tmp = NULL;
    while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
        (*i)++;
    if ((*i) == (*j))
    {
        if (line[*i] && line[*i] == '?')
        {
            (*j) = ++(*i);
            return (ft_itoa(sh->last_status));
        }
        if (line[*i] == '{')
        {
            *j = ++(*i);
            while (line[*i] && line[*i] != '}')
                (*i)++;
            if (!line[*i])
                return (NULL);
            str = ft_str_cut(line, *j, *i);
            if (!str)
                return(NULL);
            tmp = env_find_alias(sh->env, str);
            free(str);
            (*j) = ++(*i);
            if (!tmp)
                return (ft_str_empty());
            return (ft_strdup(tmp));
        }
        else
            return (ft_strdup ("$"));
    }
    str = ft_str_cut(line, *j, *i);
    if (!str)
        return (NULL);
    tmp = env_find_alias(sh->env, str);
    free(str);
    (*j) = (*i);
    if (!tmp)
        return (ft_str_empty());
    return (ft_strdup(tmp));
}

char    *ft_split_qoutation (char *line, char *str, t_shell *sh, int *i, int *j)
{
    char *cut;
    char *exp;

    while (line[*i] && line[*i] != '\"')
    {
        if (line[*i] == '$')
        {
            cut = ft_str_cut(line, *j, *i);
            if (!cut)
            {
                free(str);
                return (NULL);
            }
            str = ft_str_concat(str, cut);
            if (!str)
                return (NULL);
            *j = ++(*i);
            exp = ft_expand(line, i, j, sh);
            if (!exp)
            {
                free(str);
                return (NULL);
            }
            str = ft_str_concat(str, exp);
            if (!str)
                return (NULL);
        }
        else
            (*i)++;
    }
    if (!line[*i])
    {
        free(str);
        return (NULL);
    }
    cut = ft_str_cut(line, *j, *i);
    if (!cut)
    {
        free(str);
        return (NULL);
    }
    str = ft_str_concat(str, cut);
    *(j) = (*i) + 1;
    return (str);
}

//Crash risk: calling get_toktype(str) when str == NULL
// || not supported
t_token    *ft_split_line (char * line, t_shell *sh)
{
    int i = 0;
    t_token *head = NULL;
    char *str;
    int j;
    char *cut;

    while (line [i] && is_white_space(line[i]))
        i++;
    while (line [i])
    {
        if (line [i] && line[i] == '|')
        {
            str = ft_str_cut(line, i, i + 1);
            if (str)
            {
                head = insert_at_tail(head, str, get_toktype(str));
                if (!head)
                    return (NULL);
            }
            else
            {
                free_token(head);
                return (NULL);
            }
            i++;
        }
        if (line [i] && line[i] == '<')
        {
            i++;
            if (line [i] && line[i] == '<')
            {
                str = ft_str_cut(line, i - 1, i + 1);
                i++;
                if (str)
                {
                    head = insert_at_tail(head, str, get_toktype(str));
                    if (!head)
                        return (NULL);
                }
                else
                {
                    free_token(head);
                    return (NULL);
                }
            }
            else
            {
                str = ft_str_cut(line, i - 1, i);
                if (str)
                {
                    head = insert_at_tail(head, str, get_toktype(str));
                    if (!head)
                        return (NULL);
                }
                else
                {
                    free_token(head);
                    return (NULL);
                }
            }
        }
        if (line [i] && line[i] == '>')
        {
            i++;
            if (line[i] == '>')
            {
                str = ft_str_cut(line, i - 1, i + 1);
                i++;
                if (str)
                {
                    head = insert_at_tail(head, str, get_toktype(str));
                    if (!head)
                        return (NULL);
                }
                else
                {
                    free_token(head);
                    return (NULL);
                }
            }
            else
            {
                str = ft_str_cut(line, i - 1, i);
                if (str)
                {
                    head = insert_at_tail(head, str, get_toktype(str));
                    if (!head)
                        return (NULL);
                }
                else
                {
                    free_token(head);
                    return (NULL);
                }
            }
        }
        if (line[i] && line[i] != '|' && line[i] != '<' && line[i] != '>' && !is_white_space(line[i]))
        {
            str = NULL;
            j = i;
            while (line[i] && line[i] != '|' && line[i] != '<' && line[i] != '>' && !is_white_space(line[i]))
            {
                if (line[i] == '\'')
                {
                    cut = ft_str_cut(line, j, i);
                    if (!cut)
                    {
                        if (str)
                            free(str);
                        free_token(head);
                        return (NULL);
                    }
                    str = ft_str_concat(str, cut);
                    if (!str)
                        return (free_token(head), NULL);
                    j = ++i;
                    while (line[i] && line[i] != '\'')
                        i++;
                    if (!line[i])
                    {
                        printf("there is no closing (').\n");
                        free(str);
                        free_token(head);
                        return (NULL);
                    }
                    cut = ft_str_cut(line, j, i);
                    if (!cut)
                    {
                        if (str)
                            free(str);
                        free_token(head);
                        return (NULL);
                    }
                    str = ft_str_concat(str, cut);
                    if (!str)
                    {
                        free_token(head);
                        return (NULL);
                    }
                    j = i + 1;
                }
                if (line[i] == '\"')
                {
                    cut = ft_str_cut(line, j, i);
                    if (!cut)
                    {
                        if (str)
                            free(str);
                        free_token(head);
                        return (NULL);
                    }
                    str = ft_str_concat(str, cut);
                    if (!str)
                        return (free_token(head), NULL);
                    i++;
                    j = i;
                    str = ft_split_qoutation(line, str, sh, &i, &j);
                    if (!str)
                        return (free_token(head), NULL);
                }
                if (line[i] == '$')
                {
                    cut = ft_str_cut(line, j, i);
                    j = ++i;
                    if (!cut)
                    {
                        if (str)
                            free(str);
                        free_token(head);
                        return (NULL);
                    }
                    str = ft_str_concat(str, cut);
                    if (!str)
                    {
                        free_token(head);
                        return (NULL);
                    }
                    cut = ft_expand(line, &i, &j, sh);
                    if (!cut)
                    {
                        if (str)
                            free(str);
                        free_token(head);
                        return (NULL);
                    }
                    str = ft_str_concat(str, cut);
                    if (!str)
                    {
                        free_token(head);
                        return (NULL);
                    }
                    i--;
                }
                i++;
            }
            cut = ft_str_cut(line, j, i);
            if (!cut)
            {
                if (str)
                    free(str);
                free_token(head);
                return (NULL);
            }
            str = ft_str_concat(str, cut);
            if (str)
            {
                head = insert_at_tail(head, str, TOK_WORD);
                if (!head)
                    return (NULL);
            }
            else
            {
                free_token(head);
                return (NULL);
            }
        }
        while (line[i] && is_white_space(line[i]))
            i++;
    }
    return (head);
}



static const char *toktype_to_str(t_toktype type)
{
	if (type == TOK_WORD)     return ("WORD");
	if (type == TOK_PIPE)     return ("PIPE");
	if (type == TOK_REDIR_IN) return ("REDIR_IN");
	if (type == TOK_REDIR_OUT)return ("REDIR_OUT");
	if (type == TOK_APPEND)   return ("APPEND");
	if (type == TOK_HEREDOC)  return ("HEREDOC");
	return ("UNKNOWN");
}

void	print_tokens(t_token *head)
{
	int	i;

	i = 0;
	while (head)
	{
		printf("[%d] type=%s (%d)", i, toktype_to_str(head->type), head->type);
		if (head->value)
			printf(" value=\"%s\"", head->value);
		printf("\n");
		head = head->next;
		i++;
	}
}

t_token *ft_verify_tokens(t_token *head)
{
    t_token *cur;

    cur = head;
    if (cur)
    {
        if (cur->type == TOK_PIPE)
        {
            free_token(head);
            return (NULL);
        }
    }
    while (cur)
    {
        if (cur->type == TOK_PIPE)
        {
            if (cur->next && cur->next->type == TOK_PIPE)
            {
                free_token(head);
                return (NULL);
            }
            if (!cur->next)
            {
                free_token(head);
                return (NULL);
            }
        }
        if (cur->type == TOK_APPEND || cur->type == TOK_HEREDOC || cur->type == TOK_REDIR_IN
            || cur->type == TOK_REDIR_OUT)
        {
            if (cur->next && cur->next->type != TOK_WORD)
            {
                free_token(head);
                return (NULL);
            }
            if (!cur->next)
            {
                free_token(head);
                return (NULL);
            }
        }
        cur = cur->next;
    }
    return (head);
}

t_token *init_token (char *line, t_shell *sh)
{
    t_token *head;

    head = NULL;
    head = ft_split_line(line, sh);
    head = ft_verify_tokens(head);
    // print_tokens(head);
    // free(line);
    return (head);
}

// int main(void)
// {
//     t_shell *sh = NULL;
//     char *tests[] = {
//         "echo $ ah\"$name\"mad",
//         "echo \"a | b\" | wc",
//         "cat < in > out >> log",
//         "cat << EOF | wc",
//         "echo 'unterminated'",
//         NULL};
//     t_token *head = NULL;
//     int i = 0;
//     while (tests[i])
//     {
//         head = init_token(tests[i], sh);
//         free_token(head);
//         head = NULL;
//         i++;
//     }
//     return (0);
// }

// if (line [i] && line[i] == '\'')
//         {
//             j = ++i;
//             while (line[i] && line[i] != '\'')
//                 i++;
//             if (!line[i])
//             {
//                 printf("error, there is no closing (')\n");
//                 free_token(head);
//                 return (NULL);
//             }
//             str = ft_str_cut(line, j, i);
//             if (str)
//                 head = insert_at_tail(head, str, TOK_WORD);
//             i++;
//         }
//         if (line[i] && line[i] == '\"')
//         {
//             j = ++i;
//             while (line[i] && line[i] != '\"')
//                 i++;
//             if (!line[i])
//             {
//                 printf("error, there is no closing (\")\n");
//                 free_token(head);
//                 return (NULL);
//             }
//             str = ft_str_cut(line, j, i);
//             if (str)
//                 head = insert_at_tail(head, str, TOK_WORD);
//             i++;
//         }
