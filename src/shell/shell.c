/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:40:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/04 16:05:30 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static unsigned int	hash_function(const char *key)
{
	unsigned int	hash;

	hash = 0;
	while (*key)
	{
		hash = (hash * 31 + *key) % HASH_TABLE_SIZE;
		key++;
	}
	return (hash);
}

t_hash_table	*hash_table_create(t_arena *arena)
{
	t_hash_table	*table;
	int				i;

	table = arena_alloc(arena, sizeof(t_hash_table));
	if (!table)
		return (NULL);
	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		table->buckets[i] = NULL;
		i++;
	}
	return (table);
}

static t_env_entry	*find_entry(t_hash_table *table, char *key,
		unsigned int index)
{
	t_env_entry	*current;

	current = table->buckets[index];
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	hash_table_set(t_hash_table *table, char *key, char *value,
		t_arena *arena)
{
	unsigned int	index;
	t_env_entry		*entry;

	if (!table || !key)
		return ;
	index = hash_function(key);
	entry = find_entry(table, key, index);
	if (entry)
	{
		if (value)
			entry->value = arena_strdup(value, arena);
		else
			entry->value = NULL;
		return ;
	}
	entry = arena_alloc(arena, sizeof(t_env_entry));
	if (!entry || !(entry->key = arena_strdup(key, arena)))
		return ;
	if (value)
		entry->value = arena_strdup(value, arena);
	else
		entry->value = NULL;
	entry->next = table->buckets[index];
	table->buckets[index] = entry;
}

void	hash_table_delete(t_hash_table *table, char *key)
{
	unsigned int	index;
	t_env_entry		*current;
	t_env_entry		*prev;

	if (!table || !key)
		return ;
	index = hash_function(key);
	current = table->buckets[index];
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				table->buckets[index] = current->next;
			return ;
		}
		prev = current;
		current = current->next;
	}
}

char	*hash_table_get(t_hash_table *table, char *key)
{
	unsigned int	index;
	t_env_entry		*current;

	index = hash_function(key);
	current = table->buckets[index];
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static char	*extract_key(char *env_str, char *equal_pos, t_arena *arena)
{
	char	*key;
	size_t	len;

	if (equal_pos)
		len = equal_pos - env_str;
	else
		len = ft_strlen(env_str);
	key = arena_alloc(arena, len + 1);
	if (!key)
		return (NULL);
	ft_strlcpy(key, env_str, len + 1);
	return (key);
}

t_hash_table	*populate_env_from_envp(char **envp, t_arena *arena)
{
	t_hash_table	*table;
	int				i;
	char			*key;
	char			*equal_pos;

	table = hash_table_create(arena);
	if (!table)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		equal_pos = ft_strchr(envp[i], '=');
		key = extract_key(envp[i], equal_pos, arena);
		if (!key)
			return (NULL);
		if (equal_pos)
			hash_table_set(table, key, equal_pos + 1, arena);
		else
			hash_table_set(table, key, NULL, arena);
		i++;
	}
	return (table);
}

static t_shell	*shell_static_init(void)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->last_exit_status = 0;
	shell->heredoc_counter = 0;
	shell->stdin_fd = STDIN_FILENO;
	shell->stdout_fd = STDOUT_FILENO;
	shell->tmp_dir = "/tmp";
	shell->env_table = NULL;
	shell->env_array = NULL;
	shell->path_dirs = NULL;
	shell->input = NULL;
	return (shell);
}

static int	shell_dynamic_init(t_shell *shell, char **env)
{
	shell->session_arena = arena_init(SESSION_ARENA_SIZE);
	if (!shell->session_arena)
	{
		cleanup_shell_partial(shell, 1);
		return (0);
	}
	shell->command_arena = arena_init(COMMAND_ARENA_SIZE);
	if (!shell->command_arena)
	{
		cleanup_shell_partial(shell, 2);
		return (0);
	}
	shell->env_table = populate_env_from_envp(env, shell->session_arena);
	if (!shell->env_table)
	{
		cleanup_shell_partial(shell, 3);
		return (0);
	}
	shell->env_array = env_array_from_hashtable(shell);
	if (!shell->env_array)
	{
		cleanup_shell_partial(shell, 3);
		return (0);
	}
	return (1);
}

t_shell	*shell_init(char **env)
{
	t_shell	*shell;

	shell = shell_static_init();
	if (!shell)
		return (NULL);
	if (!shell_dynamic_init(shell, env))
		return (NULL);
	return (shell);
}
