/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:21:45 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/06 13:18:41 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_builtin(t_command *cmd, t_shell *shell)
{
	if (!cmd || !shell)
		return ;
	if (cmd->argv[1] != NULL)
	{
		ft_fprintf(STDERR_FILENO, "minishell: env: too many arguments\n");
		shell->last_exit_status = 127;
		return ;
	}
	else
		print_environment_variables(shell->env_array, shell, false);
}
