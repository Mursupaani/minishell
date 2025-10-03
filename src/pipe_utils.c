/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 15:39:43 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/03 16:07:10 by anpollan         ###   ########.fr       */
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
