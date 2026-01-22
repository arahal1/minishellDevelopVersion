#include "../../includes/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>

/* ----------------- small utils ----------------- */

static void	close_safe(int fd)
{
	if (fd >= 0)
		close(fd);
}

static int	count_cmds(t_cmd *cmds)
{
	int n = 0;
	while (cmds)
	{
		n++;
		cmds = cmds->next;
	}
	return (n);
}

static int	status_to_exitcode(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

/* ----------------- child side ----------------- */

static void	child_setup_pipe_fds(int prev_read, int pipefd[2], int is_last)
{
	if (prev_read != -1)
	{
		if (dup2(prev_read, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
	}
	if (!is_last)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
	}
	/* Close everything we don't need after dup2 */
	close_safe(prev_read);
	close_safe(pipefd[0]);
	close_safe(pipefd[1]);
}

/* If you have Phase 7, apply it here */
static int	child_apply_redirs(t_cmd *cmd)
{
	/* Example:
	   return apply_redirs(cmd->redirs);
	   Must override pipe dup2 when present.
	*/
	(void)cmd;
	return (0);
}

static void	child_exec(t_shell *sh, t_cmd *cmd)
{
	/* Redirs-only command: no argv[0] */
	if (!cmd->argv || !cmd->argv[0])
		exit(0);

	/* Your Phase 8 code, but WITHOUT fork/wait:
	   - resolve path
	   - envp = env_to_envp(sh) (TEMP)
	   - execve(path, cmd->argv, envp)
	   - if execve fails -> perror + exit(126/127)
	*/
	exec_external(sh, cmd); /* <-- you implement: does execve or exits */
	exit(1);
}

/* ----------------- wait all ----------------- */

static int	wait_pipeline(pid_t *pids, int n, pid_t last_pid)
{
	int   i;
	int   status = 0;
	int   last_status = 0;
	pid_t w;

	i = 0;
	while (i < n)
	{
		do {
			w = waitpid(pids[i], &status, 0);
		} while (w == -1 && errno == EINTR);

		if (w == -1)
		{
			perror("waitpid");
			i++;
			continue;
		}
		if (w == last_pid)
			last_status = status;
		i++;
	}
	return (status_to_exitcode(last_status));
}

/* ----------------- main entry ----------------- */

int	exec_pipeline(t_shell *sh, t_cmd *cmds)
{
	int		n;
	pid_t	*pids;
	int		i;
	int		prev_read;
	int		pipefd[2];
	pid_t	pid;
	pid_t	last_pid;
	t_cmd	*cur;
	int		error_happened;

	n = count_cmds(cmds);
	if (n == 0)
		return (0);

	pids = malloc(sizeof(pid_t) * n);
	if (!pids)
		return (1);

	i = 0;
	prev_read = -1;
	last_pid = -1;
	cur = cmds;
	error_happened = 0;

	while (cur)
	{
		pipefd[0] = -1;
		pipefd[1] = -1;

		if (cur->next)
		{
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				error_happened = 1;
				break;
			}
		}

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			error_happened = 1;
			close_safe(pipefd[0]);
			close_safe(pipefd[1]);
			break;
		}

		if (pid == 0)
		{
			/* Child */
			child_setup_pipe_fds(prev_read, pipefd, (cur->next == NULL));

			/* Apply redirs AFTER pipe dup2 so redirs override */
			if (child_apply_redirs(cur) != 0)
				exit(1);

			child_exec(sh, cur);
		}

		/* Parent */
		pids[i++] = pid;
		last_pid = pid; /* last successfully forked child */

		close_safe(prev_read);     /* parent no longer needs old read end */
		close_safe(pipefd[1]);     /* parent never writes into this pipe */
		prev_read = pipefd[0];     /* keep read end for next command */

		cur = cur->next;
	}

	close_safe(prev_read);

	/* Wait only for the children we successfully forked (i count) */
	sh->last_status = wait_pipeline(pids, i, last_pid);

	/* If pipeline setup failed before completion, return non-zero */
	if (error_happened && sh->last_status == 0)
		sh->last_status = 1;

	free(pids);
	return (sh->last_status);
}
