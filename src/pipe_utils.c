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

int	close_unused_fds(int **pipe_array, int cmd_count, int process_index)
{
	int	pipes;

	if (process_index == -1)
	{
		pipes = cmd_count - 1;
		while (--pipes >= 0)
		{
			close(pipe_array[pipes][0]);
			close(pipe_array[pipes][1]);
		}
	}
	else
	{
		if (process_index < cmd_count -1)
			pipes = process_index + 1;
		else
			pipes = process_index;
		while (--pipes >= 0)
		{
			close(pipe_array[pipes][1]);
			close(pipe_array[pipes][0]);
		}
	}
	return (0);
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

int	**arena_alloc_pipe_arr(t_shell *shell, int cmd_count)
{
	int	pipes_count;
	int	**pipe_array;
	int	i;

	if (!shell || cmd_count <= 0)
		return (NULL);
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
