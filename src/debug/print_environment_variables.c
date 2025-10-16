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
#include <unistd.h>

static void	print_env_array(char **env_array, bool export);
static void	print_in_format(char *str, bool export, bool has_equals);

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
	bool	has_equals;

	i = -1;
	while (env_array[++i])
	{
		j = -1;
		has_equals = false;
		while (env_array[i][++j])
		{
			if (env_array[i][j] == '=')
			{
				has_equals = true;
				break ;
			}
		}
		print_in_format(env_array[i], export, has_equals);
	}
}

static void	print_in_format(char *str, bool export, bool has_equals)
{
	if (export)
	{
		if (str[0] == '_' && str[1] == '=')
			return ;
		write(STDOUT_FILENO, "declare -x ", 11);
		while (*str && *str != '=')
			write(STDOUT_FILENO, str++, 1);
		if (*str)
		{
			write(STDOUT_FILENO, str++, 1);
			write(STDOUT_FILENO, "\"", 1);
			while (*str)
				write(STDOUT_FILENO, str++, 1);
			write(STDOUT_FILENO, "\"", 1);
		}
		write(STDOUT_FILENO, "\n", 1);
	}
	else
	{
		if (!has_equals)
			return ;
		printf("%s", str);
		printf("\n");
	}
}
