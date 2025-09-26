/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 09:30:04 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/26 09:44:02 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**copy_env_array(t_shell *shell);

t_arena	*update_env_table_and_arr(t_shell *shell)
{
	char	**temp_env_arr;

	if (!shell)
		return (NULL);
	temp_env_arr = copy_env_array(shell);
	arena_reset(shell->session_arena);
	shell->env_table =
		populate_env_from_envp(temp_env_arr, shell->session_arena);
	shell->env_array =
		env_array_from_hashtable(shell);
	return (shell->session_arena);
}

static char	**copy_env_array(t_shell *shell)
{
	char	**temp_env_arr;
	int		i;

	if (!shell)
		return (NULL);
	i = 0;
	while (shell->env_array[i])
		i++;
	temp_env_arr = arena_alloc(shell->command_arena, sizeof(char *) * (i + 1));
	if (!temp_env_arr)
		return (NULL);
	i = 0;
	while (shell->env_array[i])
	{
		temp_env_arr[i] = arena_strdup(shell->env_array[i], shell->command_arena);
		if (!temp_env_arr[i])
			return (NULL);
		i++;
	}
	return (temp_env_arr);
}
