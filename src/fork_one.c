/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_one.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 15:52:39 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/29 15:56:21 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	fork_one(t_command *cmd)
{
	cmd->pid = fork();
	if (cmd->pid == -1)
	{
		// Check this
		perror(strerror(errno));
		exit(1);
	}
	return (cmd->pid);
}
