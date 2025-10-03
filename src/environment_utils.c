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

//NOTE: OK!
void	print_environment_variables(t_shell *shell)
{
	int		i;
	int		j;
	bool	no_value;

	if (!shell->env_array)
		return ;
	i = 0;
	while (shell->env_array[i])
	{
		j = 0;
		no_value = false;
		while (shell->env_array[i][j])
		{
			if (shell->env_array[i][j] == '=')
			{
				if (shell->env_array[i][j + 1] == '\0')
					no_value = true;
				break ;
			}
			j++;
		}
		printf("%s", shell->env_array[i]);
		if (no_value)
			printf("''");
		printf("\n");
		i++;
	}
	shell->last_exit_status = 0;
}

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
	temp_env_arr = copy_env_array(shell);
	if (!temp_env_arr)
		return ;
	arena_reset(shell->session_arena);
	shell->env_table = populate_env_from_envp( temp_env_arr, shell->session_arena);
	if (!shell->env_table)
		return ;
	shell->env_array = env_array_from_hashtable(shell);
	if (!shell->env_array)
	{
		cleanup_shell_partial(shell, 2);
		return ;
	}
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
		temp_env_arr[i] = arena_strdup(
				shell->env_array[i], shell->command_arena);
		if (!temp_env_arr[i])
			return (NULL);
		i++;
	}
	return (temp_env_arr);
}
