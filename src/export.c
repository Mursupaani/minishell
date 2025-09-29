/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 17:23:17 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/26 08:55:28 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_and_set_entries_to_hashtable(t_command *cmd, t_shell *shell);
static char	*get_entry_key(char *entry, t_arena *arena, t_shell *shell);
static char	*get_entry_value(char *entry, t_arena *arena);
static bool	valid_format(char *key);

void	export_environment_variable(t_command *cmd, t_shell *shell)
{
	if (!shell || !cmd)
		return ;
	if (!cmd->argv[1])
	{
		print_environment_variables(shell);
		return ;
	}
	shell->last_exit_status = 0;
	get_and_set_entries_to_hashtable(cmd, shell);
	shell->env_array = env_array_from_hashtable(shell);
	if (!shell->env_array)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: export: failed to update environment variables\n");
		shell->last_exit_status = 1;
		error_exit_and_free_memory(shell);
	}
	shell->session_arena = update_env_table_and_arr(shell);
}

static int	get_and_set_entries_to_hashtable(t_command *cmd, t_shell *shell)
{
	t_env_entry	t;
	int			i;

	i = 0;
	while (cmd->argv[++i])
	{
		t.key = get_entry_key(cmd->argv[i], shell->command_arena, shell);
		if (!t.key)
			return (1);
		t.value = ft_strchr(cmd->argv[i], '=');
		if (t.value)
		{
			t.value = get_entry_value(t.value + 1, shell->command_arena);
			if (!t.value)
			{
				ft_fprintf(STDERR_FILENO, "Failed to export variable\n");
				shell->last_exit_status = 1;
				return (1);
			}
		}
		hash_table_set(shell->env_table, t.key, t.value, shell->session_arena);
	}
	return (0);
}

static char	*get_entry_key(char *entry, t_arena *arena, t_shell *shell)
{
	int		i;
	char	*key;

	if (!entry || !arena)
		return (NULL);
	// if (!ft_isalpha(entry[0]) && entry[0] != '_')
	if (!valid_format(entry))
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n", entry);
		shell->last_exit_status = 1;
		return (NULL);
	}
	i = 0;
	while (entry[i] && entry[i] != '=')
		i++;
	key = arena_alloc(arena, i + 1);
	if (!key)
		return (NULL);
	i = -1;
	while (++i && entry[i] && entry[i] != '=')
		key[i] = entry[i];
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
		return (NULL);
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
	if (!key)
		return (false);
	if (!ft_isalpha(*key) && *key != '_')
		return (false);
	while (*key && *key!= '=')
	{
		if (!ft_isalnum(*key)
		&& *key != '_')
			return (false);
		key++;
	}
	return (true);
}
