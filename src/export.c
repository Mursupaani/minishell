/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 17:23:17 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/02 11:36:58 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_and_set_entries_to_hashtable(t_command *cmd, t_shell *shell);
static char	*get_entry_key(char *entry, t_arena *arena);
static char	*get_entry_value(char *entry, t_arena *arena);
static bool	valid_format(char *key);

void	export_environment_variable(t_command *cmd, t_shell *shell)
{
	//FIXME: Print variables in alphabetical order with "declare -x " -prefix
	//FIXME: += should append to the variable. Out of scope?
	if (!shell || !cmd)
		return ;
	if (!cmd->argv[1])
	{
		print_environment_variables(shell);
		return ;
	}
	shell->last_exit_status = 0;
	get_and_set_entries_to_hashtable(cmd, shell);
	update_env_table_and_arr(shell);
}

static int	get_and_set_entries_to_hashtable(t_command *cmd, t_shell *shell)
{
	t_env_entry	t;
	int			i;

	i = 0;
	while (cmd->argv[++i])
	{
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
		}
		hash_table_set(shell->env_table, t.key, t.value, shell->session_arena);
	}
	return (0);
}

static char	*get_entry_key(char *entry, t_arena *arena)
{
	int		i;
	char	*key;

	if (!entry || !arena)
		return (NULL);
	if (!valid_format(entry))
	{
		return (NULL);
	}
	i = 0;
	while (entry[i] && entry[i] != '=')
		i++;
	key = arena_alloc(arena, i + 1);
	if (!key)
		return (NULL);
	i = 0;
	while (entry[i] && entry[i] != '=')
	{
		key[i] = entry[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

static char	*get_entry_value(char *entry, t_arena *arena)
{
	int		i;
	char	*value;

	if (!entry || !arena)
		return (NULL);
	value = arena_alloc(arena, ft_strlen(entry) + 1);
	if (!value)
	{
		ft_fprintf(STDERR_FILENO, "Failed to export variable\n");
		return (NULL);
	}
	i = 0;
	while (entry[i])
	{
		value[i] = entry[i];
		i++;
	}
	value[i] = '\0';
	return (value);
}

static bool	valid_format(char *key)
{
	int	i;

	if (!key)
		return (false);
	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n", key);
		return (false);
	}
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
		{
			ft_fprintf(STDERR_FILENO,
				"minishell: export: `%s': not a valid identifier\n", key);
			return (false);
		}
		i++;
	}
	return (true);
}
