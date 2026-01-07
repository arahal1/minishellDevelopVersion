/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbouk <adbouk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 15:54:36 by adbouk            #+#    #+#             */
/*   Updated: 2026/01/07 17:42:49 by adbouk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/minishell.h"
# include "../../libft/libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	c1;
	unsigned char	c2;

	while (*s1 || *s2)
	{
		c1 = (unsigned char)*s1;
		c2 = (unsigned char)*s2;
		if (c1 != c2)
			return (c1 - c2);
		if (*s1)
			s1++;
		if (*s2)
			s2++;
	}
	return (0);
}


void    free_strs(char **strs)
{
    int i;

    i = 0;
    while (strs[i])
    {
        free(strs[i]);
        i++;
    }
    free(strs);
}

char *ft_str_cut (char *str, int start, int end)
{
    int size;
    int i;
    int j;

    size = end - start + 1;
    if (size == 0)
        return (NULL);
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
        return (NULL);
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

void    print_token(t_token *head)
{
    t_token *cur;
 
    if (!head)
        return ;
    cur = head;
    while (cur)
    {
        printf("Token: %s, Type: %d\n", cur->value, cur->type);
        cur = cur->next;
    }
}

t_token    *ft_split_line (char * line)
{
    int i = 0;
    int size = ft_strlen(line);
    t_token *head = NULL;
    char *str;
    int j;

    while (line [i] && line[i] == ' ')
        i++;
    while (line [i])
    {
        if (line [i] && line[i] == '\'')
        {
            j = ++i;
            while (line[i] && line[i] != '\'')
                i++;
            if (i >= size)
            {
                printf("error, there is no closing (')\n");
                free_token(head);
                return (NULL);
            }
            str = ft_str_cut(line, j, i);
            if (str)
                head = insert_at_tail(head, str, get_toktype(str));
            i++;
        }
        if (line[i] && line[i] == '\"')
        {
            j = ++i;
            while (line[i] && line[i] != '\"')
                i++;
            if (i >= size)
            {
                printf("error, there is no closing (\")\n");
                free_token(head);
                return (NULL);
            }
            str = ft_str_cut(line, j, i);
            if (str)
                head = insert_at_tail(head, str, get_toktype(str));
            i++;
        }
        if (line [i] && line[i] == '|')
        {
            str = ft_str_cut(line, i, i + 1);
            head = insert_at_tail(head, str, get_toktype(str));
            i++;
        }
        if (line [i] && line[i] == '<')
        {
            i++;
            if (line [i] && line[i] == '<')
            {
                str = ft_str_cut(line, i - 1, i + 1);
                i++;
                head = insert_at_tail(head, str, get_toktype(str));
            }
            else
            {
                str = ft_str_cut(line, i - 1, i);
                head = insert_at_tail(head, str, get_toktype(str));
            }
        }
        if (line [i] && line[i] == '>')
        {
            i++;
            if (line[i] == '>')
            {
                str = ft_str_cut(line, i - 1, i + 1);
                i ++;
                head = insert_at_tail(head, str, get_toktype(str));
            }
            else
            {
                str = ft_str_cut(line, i - 1, i);
                head = insert_at_tail(head, str, get_toktype(str));
            }
        }
        if (line[i] && line[i] != '\'' && line[i] != '\"' && line[i] != '|' && line[i] != '<' && line[i] != '>' && line[i] != ' ')
        {
            j = i;
            while (line[i] && line[i] != '\'' && line[i] != '\"' && line[i] != '|' && line[i] != '<' && line[i] != '>' && line[i] != ' ')
            {
                i++;
            }
            str = ft_str_cut(line, j, i);
            head = insert_at_tail(head, str, get_toktype(str));
        }
        while (line[i] && line[i] == ' ')
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

t_token *init_token (char *line)
{
    t_token *head;

    head = NULL;
    head = ft_split_line(line);
    print_tokens(head);
    return (head);
}


// int main(void)
// {
    
//     char	*tests[] = {
//         "echo 'hello' |",
//         "echo \"a | b\" | wc",
//         "cat < in > out >> log",
//         "cat << EOF | wc",
//         "echo 'unterminated'",
// 		NULL
// 	};
//     t_token *head = NULL;
//     int i = 0;
//     while (tests[i])
//     {
//         head = init_token(tests[i]);
//         free_token(head);
//         head = NULL;
//         i++;
//     }
//     return (0);
// }
