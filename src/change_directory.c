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
	//FIXME: User deleted parent folders?
	if (cmd->argv[2] != NULL)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: cd: too many arguments\n");
		shell->last_exit_status = 1;
		return ;
	}
	if (cmd->argv[1] == NULL
		|| ft_strncmp(cmd->argv[1], "~", ft_strlen(cmd->argv[1])) == 0)
	{
		if (chdir(hash_table_get(shell->env_table, "HOME")) == -1)
		{
			ft_fprintf(STDERR_FILENO, "minishell: cd: %s: %s\n",
				cmd->argv[1], strerror(errno));
			shell->last_exit_status = 1;
			return ;
		}
	}
	else if (chdir(cmd->argv[1]) == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: cd: %s: %s\n",
			cmd->argv[1], strerror(errno));
		shell->last_exit_status = 1;
		return ;
	}
}
