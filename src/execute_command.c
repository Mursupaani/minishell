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

void	execute_command(t_command *cmd, t_shell *shell)
{
	if (cmd->next)
		execute_pipe(cmd, shell);
	else
		choose_execution_type(cmd, shell);
}

void	choose_execution_type(t_command *cmd, t_shell *shell)
{
	if (cmd->next)
		execute_pipe(cmd, shell);
	else if (cmd->cmd_type == CMD_BUILTIN)
		execute_builtin_command(cmd, shell);
	else if (cmd->cmd_type == CMD_EXTERNAL)
		execute_external_command(cmd, shell);

	// if (cmd->cmd_type == CMD_BUILTIN)
	// {
	// 	if (cmd->redirections)
	// 		execute_redirection(cmd->redirections, shell);
	// 	execute_builtin_command(cmd, shell);
	// }
	// else
	// {
	// 	if (fork() == 0)
	// 	{
	// 		// if (cmd->is_pipe)
	// 		// 	execute_pipe(cmd, shell);
	// 		if (cmd->redirections)
	// 			execute_redirection(cmd->redirections, shell);
	// 		ft_putstr_fd("Testing\n", 2);
	// 	}
	// 	// if (waitpid(cmd->pid, WEXITSTATUS(cmd->status), 0) == -1)
	// 	if (waitpid(cmd->pid, &cmd->status, 0) == -1)
	// 		perror(strerror(errno));
	// 	shell->last_exit_status = cmd->status;
	// }
}

void	execute_builtin_command(t_command *cmd, t_shell *shell)
{
	if (ft_strncmp(cmd->argv[0], "cd", ft_strlen(cmd->argv[0])) == 0)
		change_directory(cmd);
	else if (ft_strncmp(cmd->argv[0], "pwd", ft_strlen(cmd->argv[0])) == 0)
		print_working_directory(cmd);
	else if (ft_strncmp(cmd->argv[0], "echo", ft_strlen(cmd->argv[0])) == 0)
		ft_echo(cmd);
	else if (ft_strncmp(cmd->argv[0], "env", ft_strlen(cmd->argv[0])) == 0)
		print_environment_variables(shell);
	else if (ft_strncmp(cmd->argv[0], "export", ft_strlen(cmd->argv[0])) == 0)
		export_environment_variable(cmd, shell);
}

void	execute_external_command(t_command *cmd, t_shell *shell)
{
	char	*executable_path;

	if (access(cmd->argv[0], F_OK) == 0)
		executable_path = cmd->argv[0];
	else
		executable_path = find_file_from_path(cmd->argv[0], shell);
	if (!executable_path)
		return ;
	if (execve(executable_path, cmd->argv, cmd->envp))
		perror(strerror(errno));
}
