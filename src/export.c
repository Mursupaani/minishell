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

static void	get_and_set_entries_to_hashtable(t_command *cmd, t_shell *shell);
static char	*get_entry_key(char *entry, t_arena *arena);
static char	*get_entry_value(char *entry, t_arena *arena);
static void	print_env_array(char **env_array);

void	export_environment_variable(t_command *cmd, t_shell *shell)
{
	if (!shell || !cmd)
		return ;
	if (!cmd->argv[1])
	{
		print_env_array(shell->env_array);
		return ;
	}
	shell->last_exit_status = 0;
	get_and_set_entries_to_hashtable(cmd, shell);
	shell->env_array = env_array_from_hashtable(shell);
	if (!shell->env_array)
		//FIXME: Make a safe exit function
		exit(1);
	shell->session_arena = update_env_table_and_arr(shell);
}

static void	get_and_set_entries_to_hashtable(t_command *cmd, t_shell *shell)
{
	char	*key;
	char	*value;
	int	i;

	i = 1;
	while (cmd->argv[i])
	{
		key = get_entry_key(cmd->argv[i], shell->command_arena);
		if (!key)
		{
			ft_fprintf(STDERR_FILENO,
				"minishell: export: `%s': not a valid identifier\n",
				cmd->argv[i]);
			shell->last_exit_status = 1;
			return ;
		}
		value = ft_strchr(cmd->argv[i], '=');
		if (value)
		{
			value = get_entry_value(value + 1, shell->command_arena);
			if (!value)
			{
				perror("Failed to export variable\n");
				shell->last_exit_status = 1;
				return ;
			}
		}
		hash_table_set(shell->env_table, key, value, shell->session_arena);
		i++;
	}
}

static char	*get_entry_key(char *entry, t_arena *arena)
{
	int		i;
	char	*key;

	if (!entry || !arena)
		return (NULL);
	if (entry[0] == '=')
	{
		// minishell: export: `==test': not a valid identifier
		ft_putstr_fd("", 2);
		return (NULL);
	}
	i = 0;
	while (entry [i] && entry[i] != '=')
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

static void	print_env_array(char **env_array)
{
	int		i;
	int		j;
	bool	no_value;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		j = 0;
		no_value = false;
		while (env_array[i][j])
		{
			if (env_array[i][j] == '=')
			{
				if (env_array[i][j + 1] == '\0')
					no_value = true;
				break ;
			}
			j++;
		}
		printf("%s", env_array[i]);
		if (no_value)
			printf("''");
		printf("\n");
		i++;
	}
}
