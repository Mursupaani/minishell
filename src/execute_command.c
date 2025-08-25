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

int	choose_execution_type(t_command *cmd)
{
	if (cmd->cmd_type == CMD_BUILTIN)
		execute_builtin_command(cmd);
	else
		execute_external_command(cmd);
	return (cmd->status);
}
// include char **envp inside cmd?
int	execute_external_command(t_command *cmd)
{
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		if (execve(cmd->argv[0], cmd->argv, cmd->envp) == -1)
		{
		// Replace this with similar to bash?
			perror(strerror(errno));
			return (errno);
		}
	}
	// How does the last parameter work?
	if (waitpid(cmd->pid, &cmd->status, 0) == -1)
	{
		// Replace this with proper error handling
		perror(strerror(errno));
	}
	return (cmd->status);
}

int	execute_builtin_command(t_command *cmd)
{
	if (ft_strncmp(cmd->argv[0], "cd", ft_strlen(cmd->argv[0])) == 0)
		return (change_directory(cmd));
	else if (ft_strncmp(cmd->argv[0], "pwd", ft_strlen(cmd->argv[0])) == 0)
		return (print_working_directory(cmd));
	else if (ft_strncmp(cmd->argv[0], "echo", ft_strlen(cmd->argv[0])) == 0)
		return (ft_echo(cmd));
	else return (1);
}
