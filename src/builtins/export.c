/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 17:23:17 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/16 18:16:34 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_and_set_entries_to_hashtable(t_command *cmd, t_shell *shell);
static int	handle_export(t_command *cmd, int i, bool append, t_shell *shell);
static bool	check_for_variable_append(char *argument);

void	export_environment_variable(t_command *cmd, t_shell *shell)
{
	char	**temp;
	int		i;

	if (!shell || !cmd)
		return ;
	if (!cmd->argv[1])
	{
		temp = copy_env_array(shell, shell->command_arena, &i);
		quick_sort_string_array(temp, 0, i - 1);
		print_environment_variables(temp, shell, true);
		return ;
	}
	shell->last_exit_status = 0;
	get_and_set_entries_to_hashtable(cmd, shell);
	update_env_table_and_arr(shell);
}

static int	get_and_set_entries_to_hashtable(t_command *cmd, t_shell *shell)
{
	int			i;
	bool		append;

	i = 0;
	while (cmd->argv[++i])
	{
		append = check_for_variable_append(cmd->argv[i]);
		handle_export(cmd, i, append, shell);
	}
	return (0);
}

static int	handle_export(t_command *cmd, int i, bool append, t_shell *shell)
{
	t_env_entry	t;
	char		*prev_val;

	t.key = get_entry_key(cmd->argv[i], shell->command_arena);
	if (!t.key)
	{
		shell->last_exit_status = 1;
		return (1);
	}
	t.value = ft_strchr(cmd->argv[i], '=');
	if (t.value)
	{
		t.value = get_entry_value(t.value + 1, shell->command_arena);
		if (!t.value)
		{
			shell->last_exit_status = 1;
			return (1);
		}
		prev_val = hash_table_get(shell->env_table, t.key);
		if (append && prev_val)
			t.value = ft_strjoin_arena(prev_val, t.value, shell->command_arena);
	}
	hash_table_set(shell->env_table, t.key, t.value, shell->session_arena);
	return (0);
}

static bool	check_for_variable_append(char *argument)
{
	if (!argument)
		return (false);
	while (*argument)
	{
		if (*argument == '+')
		{
			argument++;
			if (*argument == '=')
				return (true);
			return (false);
		}
		if (*argument == '=')
			return (false);
		argument++;
	}
	return (false);
}
