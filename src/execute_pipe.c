/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:06:25 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/01 14:43:31 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	choose_execution_type(t_command *cmd, t_shell *shell);
static int	close_unused_pipe_fds(int pipe_fd[2][2], int i, int cmd_count);
static int	count_commands(t_command *cmd);

void	execute_pipe(t_command *cmd, t_shell *shell)
{
	int	*pids;
	int	pipe_fd[2][2];
	int	cmd_count;
	int	i;

	cmd_count = count_commands(cmd);
	pids = arena_alloc(shell->command_arena, sizeof(int) * cmd_count);
	if (!pids)
	{
		error_exit_and_free_memory(shell);
		ft_fprintf(STDERR_FILENO,
			"minishell: execute_pipe: error initializing pids\n");
	}
	i = 0;
	while (cmd)
	{
		if (cmd->next)
			pipe(pipe_fd[i % 2]);
		pids[i] = create_fork(shell);
		if (pids[i] == 0)
		{
			if (i < cmd_count - 1)
			{
				close(STDOUT_FILENO);
				dup(pipe_fd[i % 2][1]);
				close(pipe_fd[i % 2][1]);
				close(pipe_fd[i % 2][0]);
			}
			if (i != 0)
			{
				close(STDIN_FILENO);
				dup(pipe_fd[(i - 1) % 2][0]);
				close(pipe_fd[(i - 1) % 2][0]);
				close(pipe_fd[(i - 1) % 2][1]);
			}
			if (i == cmd_count )
				close_unused_pipe_fds(pipe_fd, i, cmd_count);
			choose_execution_type(cmd, shell);
		}
		if (cmd->next)
		{
			if (close(pipe_fd[i % 2][1]))
				ft_fprintf(STDERR_FILENO, "%d invalid close\n", i);
			if (i != 0)
				if (close(pipe_fd[(i - 1) % 2][0]))
					ft_fprintf(STDERR_FILENO, "%d invalid close\n", i);
		}
		else
		{
			if (close(pipe_fd[(i - 1) % 2][0]))
				ft_fprintf(STDERR_FILENO, "%d invalid close\n", i);
		}
		cmd = cmd->next;
		i++;
	}
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &shell->last_exit_status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(shell->last_exit_status))
				shell->last_exit_status = WEXITSTATUS(shell->last_exit_status);
			else if (WIFSIGNALED(shell->last_exit_status))
				shell->last_exit_status = 128 + WTERMSIG(shell->last_exit_status);
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

static int	close_unused_pipe_fds(int pipe_fd[2][2], int i, int cmd_count)
{
	if (i < cmd_count - 1)
	{
	}
	if (i == cmd_count - 1)
	{
	}
	if (close(pipe_fd[0][0]))
		ft_fprintf(STDERR_FILENO, "%d invalid close [0][0]\n", i);
	if (close(pipe_fd[0][1]))
		ft_fprintf(STDERR_FILENO, "%d invalid close [0][1]\n", i);
	if (close(pipe_fd[1][0]))
		ft_fprintf(STDERR_FILENO, "%d invalid close [1][0]\n", i);
	if (close(pipe_fd[1][1]))
		ft_fprintf(STDERR_FILENO, "%d invalid close [1][1]\n", i);
	return (0);
}

static void	choose_execution_type(t_command *cmd, t_shell *shell)
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
