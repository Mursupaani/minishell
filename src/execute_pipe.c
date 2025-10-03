/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:06:25 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/03 16:08:28 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_commands(t_command *cmd);
static int	**arena_alloc_pipe_arr(t_shell *shell, int cmd_count);
static void	execute_pipeline(t_shell *shell, t_command *cmd, int cmd_count);
static void	wait_pipeline_to_finish(t_shell *shell, int cmd_count);

void	execute_pipe(t_command *cmd, t_shell *shell)
{
	int	cmd_count;

	cmd_count = count_commands(cmd);
	shell->pipe_array = arena_alloc_pipe_arr(shell, cmd_count);
	shell->pipe_pids
		= arena_alloc(shell->command_arena, sizeof(int) * cmd_count);
	if (!shell->pipe_pids || !shell->pipe_array)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: execute_pipe: error initializing pipes\n");
		shell->last_exit_status = 1;
		return ;
	}
	execute_pipeline(shell, cmd, cmd_count);
	close_unused_fds(shell->pipe_array, cmd_count, -1);
	wait_pipeline_to_finish(shell, cmd_count);
}

static void	execute_pipeline(t_shell *shell, t_command *cmd, int cmd_count)
{
	int	i;

	i = -1;
	while (++i < cmd_count)
	{
		if (cmd->next)
			pipe(shell->pipe_array[i]);
		shell->pipe_pids[i] = create_fork(shell);
		if (shell->pipe_pids[i] == 0)
		{
			if (i < cmd_count - 1)
			{
				close(STDOUT_FILENO);
				dup(shell->pipe_array[i][1]);
			}
			if (i != 0)
			{
				close(STDIN_FILENO);
				dup(shell->pipe_array[i - 1][0]);
			}
			close_unused_fds(shell->pipe_array, cmd_count, i);
			choose_execution_type(cmd, shell);
		}
		cmd = cmd->next;
	}
}

static void	wait_pipeline_to_finish(t_shell *shell, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(shell->pipe_pids[i], &shell->last_exit_status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(shell->last_exit_status))
				shell->last_exit_status = WEXITSTATUS(shell->last_exit_status);
			else if (WIFSIGNALED(shell->last_exit_status))
				shell->last_exit_status
					= 128 + WTERMSIG(shell->last_exit_status);
			else
				shell->last_exit_status = 1;
		}
		i++;
	}
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

static int	**arena_alloc_pipe_arr(t_shell *shell, int cmd_count)
{
	int	pipes_count;
	int	**pipe_array;
	int	i;

	pipes_count = cmd_count - 1;
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
	return (pipe_array);
}
