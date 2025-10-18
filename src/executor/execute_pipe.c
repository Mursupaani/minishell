/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:06:25 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/11 12:56:01 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_pipeline(t_shell *shell, t_command *cmd, int cmd_count);
static void	wait_pipeline_to_finish(t_shell *shell, int cmd_count);
static void	execute_child_process(
				int i, int cmd_count, t_command *cmd, t_shell *shell);

void	execute_pipe(t_command *cmd, t_shell *shell)
{
	int	cmd_count;

	cmd_count = count_commands(cmd);
	shell->pipe_pids
		= arena_alloc(shell->command_arena, sizeof(int) * cmd_count);
	if (!shell->pipe_pids)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: execute_pipe: error initializing pipes\n");
		shell->last_exit_status = 1;
		return ;
	}
	execute_pipeline(shell, cmd, cmd_count);
	wait_pipeline_to_finish(shell, cmd_count);
}

static void	execute_pipeline(t_shell *shell, t_command *cmd, int cmd_count)
{
	int	i;

	i = -1;
	while (++i < cmd_count)
	{
		if (cmd->next)
			pipe(shell->pipe_array[i % 2]);
		shell->pipe_pids[i] = create_fork(shell);
		if (shell->pipe_pids[i] == 0)
			execute_child_process(i, cmd_count, cmd, shell);
		cmd = cmd->next;
		close_unused_fds(shell->pipe_array, cmd_count, i, true);
	}
}

static void	wait_pipeline_to_finish(t_shell *shell, int cmd_count)
{
	int	i;

	setup_execution_signals();
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
	setup_parent_signals();
}

static void	execute_child_process(
	int i, int cmd_count, t_command *cmd, t_shell *shell)
{
	setup_child_signals();
	if (i < cmd_count - 1)
		dup2(shell->pipe_array[i % 2][1], STDOUT_FILENO);
	if (i != 0)
		dup2(shell->pipe_array[(i - 1) % 2][0], STDIN_FILENO);
	close_unused_fds(shell->pipe_array, cmd_count, i, false);
	choose_execution_type(cmd, shell);
}
