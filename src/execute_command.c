/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:40:56 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/22 11:43:56 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	choose_execution_type(t_command *cmd, t_shell *shell)
{
	if (cmd->cmd_type == CMD_BUILTIN)
		execute_builtin_command(cmd, shell);
	else
		execute_external_command(cmd, shell);
}
// include char **envp inside cmd?
void	execute_external_command(t_command *cmd, t_shell *shell)
{
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		if (execve(cmd->argv[0], cmd->argv, cmd->envp) == -1)
		{
		// Replace this with similar to bash?
			perror(strerror(errno));
			shell->last_exit_status = errno;
		}
	}
	// How does the last parameter work?
	if (waitpid(cmd->pid, &cmd->status, 0) == -1)
	{
		// Replace this with proper error handling
		perror(strerror(errno));
	}
	shell->last_exit_status = errno;
}

void	execute_builtin_command(t_command *cmd, t_shell *shell)
{
	if (ft_strncmp(cmd->argv[0], "cd", ft_strlen(cmd->argv[0])) == 0)
		change_directory(cmd, shell);
	else if (ft_strncmp(cmd->argv[0], "pwd", ft_strlen(cmd->argv[0])) == 0)
		print_working_directory(cmd, shell);
	else if (ft_strncmp(cmd->argv[0], "echo", ft_strlen(cmd->argv[0])) == 0)
		ft_echo(cmd, shell);
}
