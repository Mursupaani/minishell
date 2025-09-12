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

void	execute_pipe(t_command *cmd, t_shell *shell)
{
	if (pipe(cmd->pipe_in) == -1)
	{
		perror(strerror(errno));
		// FIXME: No exit. How to approach?
		exit(1);
	}
	while (cmd->cmd_type == CMD_PIPE)
	{
		if (create_fork() == 0)
		{
			close(STDOUT_FILENO);
			dup(cmd->pipe_in[STDOUT_FILENO]);
			close(cmd->pipe_in[STDOUT_FILENO]);
			close(cmd->pipe_in[STDIN_FILENO]);
			execute_command(cmd, shell);
		}
		if (create_fork() == 0)
		{
			close(STDIN_FILENO);
			dup(cmd->pipe_in[STDIN_FILENO]);
			close(cmd->pipe_in[STDIN_FILENO]);
			close(cmd->pipe_in[STDOUT_FILENO]);
			execute_command(cmd->next, shell);
		}
		cmd += 2;
	}
	close(cmd->pipe_in[STDIN_FILENO]);
	close(cmd->pipe_in[STDOUT_FILENO]);
	waitpid(0, &shell->last_exit_status, 0);
	waitpid(0, &shell->last_exit_status, 0);
}
