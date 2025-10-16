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

static bool	is_valid_argument(char *arg, t_shell *shell);
static void	exit_if_env_array_fails(t_shell *shell);

void	unset_environment_variable(t_command *cmd, t_shell *shell)
{
	int	i;

	if (!cmd || !shell)
		return ;
	shell->last_exit_status = 0;
	if (!cmd->argv[1])
		return ;
	i = 1;
	while (cmd->argv[i])
	{
		if (is_valid_argument(cmd->argv[i], shell))
		{
			hash_table_delete(shell->env_table, cmd->argv[i]);
			i++;
		}
		else
			break ;
	}
	shell->env_array = env_array_from_hashtable(shell);
	if (!shell->env_array)
		exit_if_env_array_fails(shell);
	update_env_table_and_arr(shell);
}

static bool	is_valid_argument(char *arg, t_shell *shell)
{
	if (!arg || arg[0] == '-')
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: unset: %s: invalid option\n", arg);
		shell->last_exit_status = 2;
		return (false);
	}
	return (true);
}

static void	exit_if_env_array_fails(t_shell *shell)
{
	ft_fprintf(STDERR_FILENO,
		"minishell: export: failed to update environment variables\n");
	error_exit_and_free_memory(shell);
}
