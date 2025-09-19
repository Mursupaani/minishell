/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:06:30 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/29 16:07:37 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_directory(t_command *cmd, t_shell *shell)
{
	if (cmd->argv[2] != NULL)
	{
		ft_putstr_fd("Too many arguments\n", STDERR_FILENO);
		//FIXME: Error handling
		shell->last_exit_status = 1;
		return ;
	}
	if (chdir(cmd->argv[1]) == -1)
	{
		perror(strerror(errno));
		shell->last_exit_status = 1;
		return ;
	}
}
