/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_environment_variables.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:45:07 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/14 12:56:05 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env_array(char **env_array, bool export);
static void	print_in_format(
			char *str, bool export, bool no_value, bool has_equals);

void	print_environment_variables(char **env, t_shell *shell, bool export)
{
	if (!shell)
		return ;
	if (!env)
	{
		shell->last_exit_status = 1;
		return ;
	}
	print_env_array(env, export);
	shell->last_exit_status = 0;
}

static void	print_env_array(char **env_array, bool export)
{
	int		i;
	int		j;
	bool	no_value;
	bool	has_equals;

	i = -1;
	while (env_array[++i])
	{
		j = -1;
		no_value = false;
		has_equals = false;
		while (env_array[i][++j])
		{
			if (env_array[i][j] == '=')
			{
				has_equals = true;
				if (env_array[i][j + 1] == '\0')
					no_value = true;
				break ;
			}
			j++;
		}
		print_in_format(env_array[i], export, no_value, has_equals);
	}
}

static void	print_in_format(
			char *str, bool export, bool no_value, bool has_equals)
{
	if (export)
	{
		printf("declare -x ");
		printf("%s", str);
		if (no_value)
			printf("''");
		printf("\n");
	}
	else
	{
		if (!has_equals)
			return ;
		printf("%s", str);
		printf("\n");
	}
}
