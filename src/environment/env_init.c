/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 17:54:22 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/20 17:55:15 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_entry	*find_entry(t_hash_table *table, char *key,
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
