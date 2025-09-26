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

// NOTE: OK!
void	change_directory(t_command *cmd, t_shell *shell)
{
	if (cmd->argv[2] != NULL)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: cd: too many arguments\n");
		shell->last_exit_status = 1;
		return ;
	}
	if (chdir(cmd->argv[1]) == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: cd: %s: %s\n",
			cmd->argv[1], strerror(errno));
		shell->last_exit_status = 1;
		return ;
	}
}
