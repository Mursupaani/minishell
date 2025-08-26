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

int	execute_command(t_command cmd, char **envp)
{
	if (!cmd.argv || !cmd.argv[0])
		return (0);
	
	if (ft_strncmp(cmd.argv[0], "exit", 4) == 0 && ft_strlen(cmd.argv[0]) == 4)
		exit(0);
	
	cmd.pid = fork();
	if (cmd.pid == 0)
	{
		if (execve(cmd.argv[0], cmd.argv, envp) == -1)
		{
			cmd.error = errno;
			perror(strerror(cmd.error));
			exit(127);
		}
	}
	if (waitpid(cmd.pid, &cmd.status, 0) == -1)
	{
		cmd.error = errno;
		perror(strerror(cmd.error));
	}
	return (WEXITSTATUS(cmd.status));
}
