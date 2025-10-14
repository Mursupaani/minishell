/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 09:30:04 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/14 12:45:33 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env_table_and_arr(t_shell *shell)
{
	char	**temp_env_arr;

	if (!shell)
		return ;
	shell->env_array = env_array_from_hashtable(shell);
	if (!shell->env_array)
	{
		ft_fprintf(STDERR_FILENO, "minishell: failed to update environment\n");
		shell->last_exit_status = 1;
		error_exit_and_free_memory(shell);
	}
	temp_env_arr = copy_env_array(shell, shell->command_arena, NULL);
	if (!temp_env_arr)
		return ;
	arena_reset(shell->session_arena);
	shell->env_table
		= populate_env_from_envp(temp_env_arr, shell->session_arena);
	if (!shell->env_table)
		return ;
	shell->env_array = env_array_from_hashtable(shell);
	if (!shell->env_array)
	{
		cleanup_shell_partial(shell, 2);
		return ;
	}
}

char	**copy_env_array(t_shell *shell, t_arena *arena, int *count)
{
	char	**temp_env_arr;
	int		i;

	if (!shell)
		return (NULL);
	i = 0;
	while (shell->env_array[i])
		i++;
	temp_env_arr = arena_alloc(arena, sizeof(char *) * (i + 1));
	if (!temp_env_arr)
		return (NULL);
	i = 0;
	while (shell->env_array[i])
	{
		temp_env_arr[i] = arena_strdup(
				shell->env_array[i], shell->command_arena);
		if (!temp_env_arr[i])
			return (NULL);
		i++;
	}
	if (count)
		*count = i;
	return (temp_env_arr);
}
