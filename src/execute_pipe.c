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

static int	count_pipes(t_command *cmd);
static int	**arena_alloc_pipe_arr(t_command *cmd, t_shell *shell, int *pipes);
static int	init_pipe_array(int **pipe_array, int pipes, t_shell *shell);
static int	close_unused_fds(int **pipe_array, int pipes, int process_index);

int	execute_pipe(t_command *cmd, t_shell *shell)
{
	// FIXME: Error management! What to return?
	int	**pipe_array;
	int	*pids;
	int	pipes;
	int	i;

	pipe_array = arena_alloc_pipe_arr(cmd, shell, &pipes);
	if (!pipe_array)
		return (1);
	if (init_pipe_array(pipe_array, pipes, shell))
		return (1);
	pids = arena_alloc(shell->command_arena, sizeof(int) * (pipes + 2));
	pids[pipes + 1] = -2;
	if (!pids)
		return (1);
	i = 0;
	while (cmd)
	{
		// FIXME: Error management?
		pids[i] = fork();
		if (pids[i] == 0)
		{
			fprintf(stderr, "Child PID %d\n", getpid());
			if (i < pipes)
			{
				close(STDOUT_FILENO);
				dup(pipe_array[i][1]);
				// fprintf(stderr, "Pipe_array[%d][1] = %d [pid = %d]\n", i, pipe_array[i][1], getpid());
			}
			if (i != 0)
			{
				close(STDIN_FILENO);
				dup(pipe_array[i - 1][0]);
				// fprintf(stderr, "Pipe_array[%d][0] = %d [pid = %d]\n", i - 1, pipe_array[i][0], getpid());
			}
			close_unused_fds(pipe_array, pipes, i);
			choose_execution_type(cmd, shell);
		}
		cmd = cmd->next;
		i++;
	}
	close_unused_fds(pipe_array, pipes, -1);
	i = 0;
	while (pids[i] != -2)
	{
		//FIXME: Check flags!
		waitpid(pids[i], &shell->last_exit_status, 0);
		i++;
	}
	return (0);
}

static int	count_pipes(t_command *cmd)
{
	int	pipes;

	pipes = 0;
	while (cmd)
	{
		if (cmd->next)
			pipes++;
		cmd = cmd->next;
	}
	return (pipes);
}

static int	**arena_alloc_pipe_arr(t_command *cmd, t_shell *shell, int *pipes)
{
	int	pipes_count;
	int	**pipe_array;
	int	i;

	pipes_count = count_pipes(cmd);
	pipe_array = arena_alloc(shell->command_arena, sizeof(int *) * pipes_count);
	if (!pipe_array)
		return (NULL);
	i = 0;
	while (i < pipes_count)
	{
		pipe_array[i] = arena_alloc(shell->command_arena, sizeof(int) * 2);
		if (!pipe_array[i])
			return (NULL);
		i++;
	}
	*pipes = pipes_count;
	return (pipe_array);
}

static int	init_pipe_array(int **pipe_array, int pipes, t_shell *shell)
{
	int	i;

	i = 0;
	while (i < pipes)
	{
		pipe_array[i] = arena_alloc(shell->command_arena, sizeof(int) * 2);
		if (!pipe_array[i])
		// FIXME: Error management
			return (1);
		if (pipe(pipe_array[i]) == -1)
		{
			while (--i >= 0)
			{
				// FIXME: Error management
				if (close(pipe_array[i][0]) == -1)
					perror(strerror(errno));
				if (close(pipe_array[i][1]) == -1)
					perror(strerror(errno));
			}
			return (1);
		}
		i++;
	}
	return (0);
}

static int	close_unused_fds(int **pipe_array, int pipes, int process_index)
{
	if (process_index == -1)
	{
		while (--pipes >= 0)
		{
			close(pipe_array[pipes][0]);
			close(pipe_array[pipes][1]);
		}
	}
	else
	{
		while (--pipes >= 0)
		{
			if (process_index != pipes)
				close(pipe_array[pipes][1]);
			if (pipes - 1 != process_index)
				close(pipe_array[pipes][0]);
		}
	}
	return (0);
}
