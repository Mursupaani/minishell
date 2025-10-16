/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 15:39:43 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/11 12:51:15 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_unused_child_fds(
				int pipe_array[2][2], int cmd_count, int process_index);

void	choose_execution_type(t_command *cmd, t_shell *shell)
{
	int	exit_status;

	if (cmd->cmd_type == CMD_BUILTIN_CHILD)
	{
		execute_builtin_command(cmd, shell);
		exit_status = shell->last_exit_status;
		free_memory_at_exit(shell);
		exit(exit_status);
	}
	else if (cmd->cmd_type == CMD_EXTERNAL)
		execute_external_command(cmd, shell);
}

void	close_unused_fds(int pipe_array[2][2],
				int cmd_count, int process_index, bool parent)
{
	if (parent)
	{
		if (process_index > 0)
		{
			close(pipe_array[(process_index - 1) % 2][0]);
			close(pipe_array[(process_index - 1) % 2][1]);
		}
	}
	else
		close_unused_child_fds(pipe_array, cmd_count, process_index);
}

int	count_commands(t_command *cmd)
{
	int	cmd_count;

	if (!cmd)
		return (-1);
	cmd_count = 0;
	while (cmd)
	{
		cmd_count++;
		cmd = cmd->next;
	}
	return (cmd_count);
}

int	**arena_alloc_pipe_arr(t_shell *shell)
{
	int	pipes_count;
	int	**pipe_array;
	int	i;

	if (!shell)
		return (NULL);
	pipes_count = 2;
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

static void	close_unused_child_fds(
	int pipe_array[2][2], int cmd_count, int process_index)
{
	if (process_index == 0)
	{
		close(pipe_array[process_index][0]);
		close(pipe_array[process_index][1]);
	}
	else if (process_index < cmd_count - 1)
	{
		close(pipe_array[(process_index - 1) % 2][0]);
		close(pipe_array[(process_index - 1) % 2][1]);
		close(pipe_array[process_index % 2][0]);
		close(pipe_array[process_index % 2][1]);
	}
	else
	{
		close(pipe_array[(process_index - 1) % 2][0]);
		close(pipe_array[(process_index - 1) % 2][1]);
	}
}
