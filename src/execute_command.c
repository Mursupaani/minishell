/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:40:56 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/29 17:10:51 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_builtin_redirections(t_command *cmd, t_shell *shell);
static int	reset_std_fds(t_shell *shell);

void	execute_commands(t_command *cmd, t_shell *shell)
{
	classify_commands(cmd);
	prepare_cmd(cmd, shell);
	if (handle_heredocs(cmd) != 0)
		return ;
	if (cmd->next)
		execute_pipe(cmd, shell);
	else if (cmd->cmd_type == CMD_BUILTIN_PARENT)
		execute_builtin_command(cmd, shell);
	else if (cmd->cmd_type == CMD_EXTERNAL)
	{
		shell->child_pid = create_fork(shell);
		if (shell->child_pid == 0)
			execute_external_command(cmd, shell);
		int wait_status;
		waitpid(shell->child_pid, &wait_status, 0);
		if (WIFEXITED(wait_status))
			shell->last_exit_status = WEXITSTATUS(wait_status);
		else if (WIFSIGNALED(wait_status))
			shell->last_exit_status = 128 + WTERMSIG(wait_status);
		else
			shell->last_exit_status = 1;
	}
}

void	execute_builtin_command(t_command *cmd, t_shell *shell)
{
	if (cmd->redirections)
		execute_builtin_redirections(cmd, shell);
	if (ft_strncmp(cmd->argv[0], "cd", ft_strlen(cmd->argv[0])) == 0)
		change_directory(cmd, shell);
	else if (ft_strncmp(cmd->argv[0], "pwd", ft_strlen(cmd->argv[0])) == 0)
		print_working_directory(shell);
	else if (ft_strncmp(cmd->argv[0], "echo", ft_strlen(cmd->argv[0])) == 0)
		ft_echo(cmd, shell);
	else if (ft_strncmp(cmd->argv[0], "env", ft_strlen(cmd->argv[0])) == 0)
		print_environment_variables(shell);
	else if (ft_strncmp(cmd->argv[0], "export", ft_strlen(cmd->argv[0])) == 0)
		export_environment_variable(cmd, shell);
	else if (ft_strncmp(cmd->argv[0], "unset", ft_strlen(cmd->argv[0])) == 0)
		unset_environment_variable(cmd, shell);
	else if (ft_strncmp(cmd->argv[0], "exit", ft_strlen(cmd->argv[0])) == 0)
		exit_builtin(cmd, shell);
	if (cmd->redirections)
		reset_std_fds(shell);
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
