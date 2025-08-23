/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:21:55 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/23 21:04:26 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_command *cmd)
{
	if (!cmd->argv[1])
		printf("\n");
	else if (strncmp(cmd->argv[1], "-n", ft_strlen(cmd->argv[1])) == 0)
	{
		if (cmd->argv[2] && cmd->argv[3])
		{
			ft_putstr_fd("Too many arguments\n", STDERR_FILENO);
			return (1);
		}
		if (cmd->argv[2])
			printf("%s", cmd->argv[2]);
	}
	else if (cmd->argv[1] && cmd->argv[2])
	{
		ft_putstr_fd("Too many arguments\n", STDERR_FILENO);
		return (1);
	}
	else
	{
		if (cmd->argv[1])
			printf("%s\n", cmd->argv[1]);
	}
	return (0);
}
