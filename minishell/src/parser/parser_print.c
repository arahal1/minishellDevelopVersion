#include "../../includes/parser.h"
/*
Assumes:
- t_cmd is a linked list
- cmd->argv is a NULL-terminated array of strings (like execve)
- cmd->redirs is a linked list of t_redir (optional)
- t_redir_type is an enum (we'll print it as an int unless you provide names)
*/

static void print_argv(char **argv)
{
    int i = 0;

    if (!argv)
    {
        printf("    argv: (null)\n");
        return;
    }
    printf("    argv:\n");
    while (argv[i])
    {
        printf("      [%d] \"%s\"\n", i, argv[i]);
        i++;
    }
    if (i == 0)
        printf("      (empty)\n");
}

static void print_redirs(t_redir *r)
{
    int i = 0;

    if (!r)
    {
        printf("    redirs: (none)\n");
        return;
    }

    printf("    redirs:\n");
    while (r)
    {
        printf("      (%d) type=%d, target=%s, heredoc_fd=%d\n",
               i,
               (int)r->type,
               (r->target ? r->target : "(null)"),
               r->heredoc_fd);
        r = r->next;
        i++;
    }
}

void print_cmd_list(t_cmd *head)
{
    int idx = 0;

    if (!head)
    {
        printf("cmd list: (null)\n");
        return;
    }

    while (head)
    {
        printf("CMD #%d:\n", idx);
        print_argv(head->argv);
        print_redirs(head->redirs);
        head = head->next;
        idx++;
        printf("\n");
    }
}
