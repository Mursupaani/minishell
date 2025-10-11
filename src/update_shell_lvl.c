/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_shell_lvl.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 17:39:28 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/11 17:45:44 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_shell_lvl(t_shell *shell)
{
	char	*shlvl;
	int		error;
	int		shlvl_num;

	if (!shell)
		return ;
	shlvl = hash_table_get(shell->env_table, "SHLVL");
	if (!shlvl)
		hash_table_set(shell->env_table, "SHLVL", "1", shell->session_arena);
	else
	{
		shlvl_num = ft_atol_safe(shlvl, &error);
		if (error != 0)
			hash_table_set(
				shell->env_table, "SHLVL", "1", shell->session_arena);
		else
		{
			shlvl_num++;
			shlvl = ft_itoa(shlvl_num);
			hash_table_set(
				shell->env_table, "SHLVL", shlvl, shell->session_arena);
		}
	}
	update_env_table_and_arr(shell);
}
