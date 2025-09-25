/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:40:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/09/24 12:13:30 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static unsigned int hash_function(const char *key)
{
	unsigned int hash;

	hash = 0;
	while (*key)
	{
		hash = (hash * 31 + *key) % HASH_TABLE_SIZE;
		key++;
	}
	return (hash);
}

t_hash_table *hash_table_create(t_arena *arena)
{
    t_hash_table *table;
    int i;
    
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

void hash_table_set(t_hash_table *table, char *key, char *value, t_arena *arena)
{
    unsigned int index;
    t_env_entry *entry;
    t_env_entry *current;

    if (!table || !key)
    // if (!table || !key || !value)
        return;
    index = hash_function(key);
    current = table->buckets[index];
    while (current)
    {
        if(ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
        {
			if (value)
				current->value = arena_strdup(value, arena);
			else
				current->value = arena_strdup("", arena);
            return;
        }
        current = current->next;
    }
    entry = arena_alloc(arena, sizeof(t_env_entry));
    if (!entry)
        return;
    entry->key = arena_strdup(key, arena);
	if (value)
		entry->value = arena_strdup(value, arena);
	else
		entry->value = arena_strdup("", arena);
	if (!entry->key || !entry->value)
		return;
    entry->next = table->buckets[index];
    table->buckets[index] = entry;
}

char *hash_table_get(t_hash_table *table, char *key)
{
    unsigned int index;
    t_env_entry *current;
	
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

t_hash_table	*populate_env_from_envp(char **envp, t_arena *arena)
{
	t_hash_table	*table;
	int i;
	char *key;
	char *value;
	char *equal_pos;

	table = hash_table_create(arena);
	if (!table)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		equal_pos = ft_strchr(envp[i], '=');
		if (!equal_pos)
		{
			i++;
			continue;
		}
		key = arena_alloc(arena, (equal_pos - envp[i]) + 1);
		if (!key)
			return (NULL);
		ft_strlcpy(key, envp[i], (equal_pos - envp[i]) + 1);
		key[equal_pos - envp[i]] = '\0';
		value = equal_pos + 1;
		hash_table_set(table, key, value, arena);
		i++;
	}
	return (table);
}

t_shell	*shell_init(char **env)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->last_exit_status = 0;
	shell->interactive = isatty(STDIN_FILENO);
	shell->mode = MODE_PROMPT;
	shell->path_dirty = 1;
	shell->heredoc_counter = 0;
	shell->stdin_fd = STDIN_FILENO;
	shell->stdout_fd = STDOUT_FILENO;
	shell->stderr_fd = STDERR_FILENO;
	shell->tmp_dir = "/tmp";
	shell->env_table = NULL;
	shell->env_array = NULL;
	shell->path_dirs = NULL;
	shell->session_arena = arena_init(8192);
	if (!shell->session_arena)
	{
		cleanup_shell_partial(shell, 1);
		return (NULL);
	}
	shell->command_arena = arena_init(4096);
	if (!shell->command_arena)
	{
		cleanup_shell_partial(shell, 2);
		return (NULL);
	}
	shell->env_table = populate_env_from_envp(env, shell->session_arena);
	if (!shell->env_table)
	{
		cleanup_shell_partial(shell, 3);
		return (NULL);
	}
	shell->env_array = env_array_from_hashtable(shell);
	if (!shell->env_array)
	{
		cleanup_shell_partial(shell, 3);
		return (NULL);
	}
	return (shell);
}
