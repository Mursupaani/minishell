/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:21:55 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/29 16:08:57 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_command *cmd)
{
	// TODO: Add support for envp. Check if can be made more tidy.
	if (!cmd->argv[1])
		printf("\n");
	else if (strncmp(cmd->argv[1], "-n", ft_strlen(cmd->argv[1])) == 0)
	{
		if (cmd->argv[2] && cmd->argv[3])
		{
			ft_putstr_fd("Too many arguments\n", STDERR_FILENO);
			exit(1);
		}
		if (cmd->argv[2])
			printf("%s", cmd->argv[2]);
	}
	else if (cmd->argv[1] && cmd->argv[2])
	{
		ft_putstr_fd("Too many arguments\n", STDERR_FILENO);
		exit(1);
	}
	else
	{
		if (cmd->argv[1])
			printf("%s\n", cmd->argv[1]);
	}
}
