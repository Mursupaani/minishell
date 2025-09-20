/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:06:25 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/29 16:12:44 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static int	count_commands(t_command *cmd);
static int	close_unused_pipe_fds(int *pipe_fd);

int	execute_pipe(t_command *cmd, t_shell *shell)
{
	// FIXME: Error management! What to return?
	int	*pids;
	int	pipe_fd[2][2];
	int	cmd_count;
	int	i;

	cmd_count = count_commands(cmd);
	printf("cmd count: %d\n", cmd_count);
	pids = arena_alloc(shell->command_arena, sizeof(int) * (cmd_count + 1));
	if (!pids)
		return (1);
	pids[cmd_count] = -2;
	i = 0;
	while (cmd)
	{
		printf("i %% 2 = %d\n", (i % 2));
		pipe(pipe_fd[i % 2]);
		// FIXME: Error management?
		pids[i] = fork();
		if (pids[i] == 0)
		{
			if (i < cmd_count - 1)
			{
				fprintf(stderr, "%d closing stdout\n", i);
				// FIXME: Error management?
				close(STDOUT_FILENO);
				dup(pipe_fd[i % 2][1]);
				close(pipe_fd[i % 2][1]);
			}
			if (i % 2 != 0)
			{
				fprintf(stderr, "%d closing stdin\n", i);
				// FIXME: Error management?
				close(STDIN_FILENO);
				dup(pipe_fd[i % 2 - 1][0]);
				close(pipe_fd[i % 2][0]);
			}
			close_unused_pipe_fds(pipe_fd[i % 2]);
			choose_execution_type(cmd, shell);
		}
		close_unused_pipe_fds(pipe_fd[i % 2]);
		cmd = cmd->next;
		i++;
	}
	i = 0;
	while (pids[i] != -2)
	{
		//FIXME: Check flags!
		waitpid(pids[i], &shell->last_exit_status, 0);
		i++;
	}
	return (0);
}

static int	count_commands(t_command *cmd)
{
	int	cmd_count;

	cmd_count = 0;
	while (cmd)
	{
		cmd_count++;
		cmd = cmd->next;
	}
	return (cmd_count);
}

static int	close_unused_pipe_fds(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (0);
}
