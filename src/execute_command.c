/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:40:56 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/26 17:33:15 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_builtin_redirections(t_command *cmd, t_shell *shell);
static int	reset_std_fds(t_shell *shell);

void	execute_commands(t_command *cmd, t_shell *shell)
{
	classify_commands(cmd);
	prepare_cmd(cmd, shell);
	handle_heredocs(cmd);
	if (cmd->next)
		execute_pipe(cmd, shell);
	else if (cmd->cmd_type == CMD_BUILTIN_PARENT)
		execute_builtin_command(cmd, shell);
	else if (cmd->cmd_type == CMD_EXTERNAL)
	{
		shell->child_pid = create_fork(shell);
		if (shell->child_pid == 0)
			execute_external_command(cmd, shell);
		waitpid(shell->child_pid, &shell->last_exit_status, 0);
	}
}

void	execute_builtin_command(t_command *cmd, t_shell *shell)
{
	if (cmd->redirections)
		execute_builtin_redirections(cmd, shell);
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
	else if (ft_strncmp(cmd->argv[0], "unset", ft_strlen(cmd->argv[0])) == 0)
		unset_environment_variable(cmd, shell);
	if (cmd->redirections)
		reset_std_fds(shell);
}

void	execute_external_command(t_command *cmd, t_shell *shell)
{
	char	*executable_path;

	//FIXME: Fix "./ls" execution
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
		execute_redirection(cmd->redirections, cmd, shell);
	if (execve(executable_path, cmd->argv, shell->env_array))
	{
		shell->last_exit_status = 1;
		ft_fprintf(STDERR_FILENO,
			"minishell: %s: %s\n", cmd->argv[0], strerror(errno));
	}
}

static int	execute_builtin_redirections(t_command *cmd, t_shell *shell)
{
	if (!cmd || !shell)
		return (1);
	shell->stdin_fd = dup(STDIN_FILENO);
	shell->stdout_fd = dup(STDOUT_FILENO);
	execute_redirection(cmd->redirections, cmd, shell);
	return (0);
}

static int	reset_std_fds(t_shell *shell)
{
	dup2(shell->stdin_fd, STDIN_FILENO);
	dup2(shell->stdout_fd, STDOUT_FILENO);
	close(shell->stdin_fd);
	close(shell->stdout_fd);
	return (0);
}
