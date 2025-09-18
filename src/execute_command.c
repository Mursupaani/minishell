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

void	execute_commands(t_command *cmd, t_shell *shell)
{
	int	pid;

	classify_commands(cmd);
	if (cmd->next)
		execute_pipe(cmd, shell);
	else if (cmd->cmd_type == CMD_BUILTIN_PARENT)
	{
		printf("Builtin parent\n");
		execute_builtin_command(cmd, shell);
	}
	else if (cmd->cmd_type == CMD_BUILTIN_CHILD)
	{
		// FIXME: Why pwd child command?
		printf("Builtin child\n");
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
		printf("External\n");
		// FIXME: Error handling
		pid = fork();
		if (pid == 0)
			execute_external_command(cmd, shell);
		waitpid(pid, &shell->last_exit_status, 0);
	}
}

void	choose_execution_type(t_command *cmd, t_shell *shell)
{
	if (cmd->cmd_type == CMD_BUILTIN_PARENT
		|| cmd->cmd_type == CMD_BUILTIN_CHILD)
		execute_builtin_command(cmd, shell);
	else if (cmd->cmd_type == CMD_EXTERNAL)
		execute_external_command(cmd, shell);
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
