/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 08:40:21 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/26 09:02:47 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

void	unset_environment_variable(t_command *cmd, t_shell *shell)
{
	int	i;

	if (!cmd || !shell)
		return ;
	if (!cmd->argv[1])
		return ;
	i = 1;
	while (cmd->argv[i])
	{
		hash_table_delete(shell->env_table, cmd->argv[i]);
		i++;
	}
	shell->env_array = env_array_from_hashtable(shell);
	if (!shell->env_array)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: export: failed to update environment variables\n");
		error_exit_and_free_memory(shell);
	}
	shell->last_exit_status = 0;
	shell->session_arena = update_env_table_and_arr(shell);
}
