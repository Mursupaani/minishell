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

void	ft_echo(t_command *cmd, t_shell *shell)
{
	// TODO: Add support for envp. Check if can be made more tidy.
	if (!cmd->argv[1])
		printf("\n");
	else if (strncmp(cmd->argv[1], "-n", ft_strlen(cmd->argv[1])) == 0)
	{
		if (cmd->argv[2] && cmd->argv[3])
		{
			ft_putstr_fd("Too many arguments\n", STDERR_FILENO);
			shell->last_exit_status = 1;
			exit(1);
		}
		if (cmd->argv[2])
			printf("%s", cmd->argv[2]);
	}
	else if (cmd->argv[1] && cmd->argv[2])
	{
		ft_putstr_fd("Too many arguments\n", STDERR_FILENO);
		shell->last_exit_status = 1;
		exit(1);
	}
	else
	{
		if (cmd->argv[1])
			printf("%s\n", cmd->argv[1]);
	}
	shell->last_exit_status = 0;
}
