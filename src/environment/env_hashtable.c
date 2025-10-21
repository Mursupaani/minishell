/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_hashtable.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 17:52:37 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/20 23:18:19 by magebreh         ###   ########.fr       */
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

void	hash_table_set(t_hash_table *table, char *key, char *value,
		t_arena *arena)
{
	unsigned int	index;
	t_env_entry		*entry;

	if (!table || !key)
		return ;
	index = hash_function(key);
	entry = find_entry(table, key, index);
	if (!entry)
	{
		entry = arena_alloc(arena, sizeof(t_env_entry));
		if (!entry)
			return ;
		entry->key = arena_strdup(key, arena);
		if (!entry->key)
			return ;
		entry->next = table->buckets[index];
		table->buckets[index] = entry;
	}
	if (value)
		entry->value = arena_strdup(value, arena);
	else
		entry->value = NULL;
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
