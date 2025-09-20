/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:40:56 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/12 15:00:27 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execut_builtin_redirections(t_command *cmd, t_shell *shell);
static int	reset_std_fds(t_shell *shell);

void	execute_commands(t_command *cmd, t_shell *shell)
{
	int	pid;

	classify_commands(cmd);
	if (cmd->next)
		execute_pipe(cmd, shell);
	else if (cmd->cmd_type == CMD_BUILTIN_PARENT)
		execute_builtin_command(cmd, shell);
	else if (cmd->cmd_type == CMD_BUILTIN_CHILD)
	{
		// FIXME: Why pwd child command?
		fprintf(stderr, "Builtin child\n");
		// FIXME: Error handling
		pid = fork();
		if (pid == 0)
		{
			execute_builtin_command(cmd, shell);
			exit(EXIT_SUCCESS);
		}
		waitpid(pid, &shell->last_exit_status, 0);
	}
	else if (cmd->cmd_type == CMD_EXTERNAL)
	{
		fprintf(stderr, "External\n");
		// FIXME: Error handling
		pid = fork();
		if (pid == 0)
			execute_external_command(cmd, shell);
		waitpid(pid, &shell->last_exit_status, 0);
	}
}

void	choose_execution_type(t_command *cmd, t_shell *shell)
{
	if (cmd->cmd_type == CMD_BUILTIN_PARENT)
	{
		// fprintf(stderr, "Builtin parent\n");
		execute_builtin_command(cmd, shell);
	}
	else if (cmd->cmd_type == CMD_BUILTIN_CHILD)
	{
		// fprintf(stderr, "Builtin child\n");
		execute_builtin_command(cmd, shell);
		exit(shell->last_exit_status);
	}
	else if (cmd->cmd_type == CMD_EXTERNAL)
	{
		// fprintf(stderr, "External\n");
		execute_external_command(cmd, shell);
	}
}

void	execute_builtin_command(t_command *cmd, t_shell *shell)
{
	if (cmd->redirections)
		execut_builtin_redirections(cmd, shell);
	if (ft_strncmp(cmd->argv[0], "cd", ft_strlen(cmd->argv[0])) == 0)
		change_directory(cmd, shell);
	else if (ft_strncmp(cmd->argv[0], "pwd", ft_strlen(cmd->argv[0])) == 0)
		print_working_directory(cmd, shell);
	else if (ft_strncmp(cmd->argv[0], "echo", ft_strlen(cmd->argv[0])) == 0)
		ft_echo(cmd, shell);
	else if (ft_strncmp(cmd->argv[0], "env", ft_strlen(cmd->argv[0])) == 0)
		print_environment_variables(shell);
	else if (ft_strncmp(cmd->argv[0], "export", ft_strlen(cmd->argv[0])) == 0)
		export_environment_variable(cmd, shell);
	if (cmd->redirections)
		reset_std_fds(shell);
}

void	execute_external_command(t_command *cmd, t_shell *shell)
{
	char	*executable_path;

	if (access(cmd->argv[0], F_OK) == 0)
		executable_path = cmd->argv[0];
	else
		executable_path = find_file_from_path(cmd->argv[0], shell);
	if (!executable_path)
	{
		shell->last_exit_status = 1;
		return ;
	}
	if (cmd->redirections)
		execute_redirection(cmd->redirections, shell);
	if (execve(executable_path, cmd->argv, shell->env_array))
	{
		shell->last_exit_status = 1;
		perror(strerror(errno));
	}
}

static int	execut_builtin_redirections(t_command *cmd, t_shell *shell)
{
	if (!cmd || !shell)
		return (1);
	shell->stdin_fd = dup(STDIN_FILENO);
	shell->stdout_fd = dup(STDOUT_FILENO);
	shell->stderr_fd = dup(STDERR_FILENO);
	execute_redirection(cmd->redirections, shell);
	return (0);
}

static int	reset_std_fds(t_shell *shell)
{
	dup2(shell->stdin_fd, STDIN_FILENO);
	dup2(shell->stdout_fd, STDOUT_FILENO);
	dup2(shell->stderr_fd, STDERR_FILENO);
	close(shell->stdin_fd);
	close(shell->stdout_fd);
	close(shell->stderr_fd);
	return (0);
}
