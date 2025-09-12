/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_variables_execution.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 13:58:42 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/12 16:34:32 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_environment_variables(t_shell *shell)
{
	if (!shell)
		return ;
	if (!shell->env_array)
	{
		printf("Fail\n");
		shell->last_exit_status = 1;
		return ;
	}
	while (shell->env_array)
		printf("%s\n", *shell->env_array);
	shell->last_exit_status = 0;
}

void	export_environment_variable(t_command *cmd, t_shell *shell)
{
	if (!shell)
		return ;
	if (!cmd)
	{
		shell->last_exit_status = 1;
		return ;
	}

}
