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

// include char **envp inside cmd?
int	execute_command(t_command cmd, char **envp)
{
	cmd.pid = fork();
	if (cmd.pid == 0)
	{
		if (execve(cmd.argv[0], cmd.argv, envp) == -1)
		{
		// Replace this with similar to bash?
			cmd.error = errno;
			perror(strerror(cmd.error));
		}
	}
	// How does the last parameter work?
	if (waitpid(cmd.pid, &cmd.status, 0))
	{
		// Replace this with proper error handling
		cmd.error = errno;
		perror(strerror(cmd.error));

	}
	return (cmd.status);
}
