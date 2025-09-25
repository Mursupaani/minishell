/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array_from_hashtable.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:31:57 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/24 12:12:34 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_env_entries(t_env_entry *entry[HASH_TABLE_SIZE]);
static char	**allocate_env_array(t_shell *shell);
static char	**populate_env_array(t_shell *shell);
static char	*get_entry_from_table(t_env_entry *entry, t_shell *shell);

char **env_array_from_hashtable(t_shell *shell)
{
	if (!shell || !shell->env_table)
		return (NULL);
	shell->env_array = allocate_env_array(shell);
	if (!shell->env_array)
		return (NULL);
	shell->env_array = populate_env_array(shell);
	if (!shell->env_array)
		return (NULL);
	return (shell->env_array);
}

static int	count_env_entries(t_env_entry *entry[HASH_TABLE_SIZE])
{
	int			entry_count;
	int			i;
	t_env_entry	*temp_entry;

	if (!entry)
		return (-1);
	i = 0;
	entry_count = 0;
	while (i < HASH_TABLE_SIZE)
	{
		if (entry[i] != NULL)
		{
			entry_count++;
			temp_entry = entry[i]->next;
			while (temp_entry)
			{
				entry_count++;
				temp_entry = temp_entry->next;
			}
		}
		i++;
	}
	return (entry_count);
}

static char	**allocate_env_array(t_shell *shell)
{
	int	entry_count;

	entry_count = count_env_entries(shell->env_table->buckets);
	if (entry_count == -1)
		return (NULL);
	shell->env_array =
		arena_alloc(shell->session_arena, (entry_count + 1) * sizeof(char *));
	if (!shell->env_array)
		return (NULL);
	return (shell->env_array);
}

static char	**populate_env_array(t_shell *shell)
{
	int			i;
	int			j;
	t_env_entry *temp_entry;

	i = 0;
	j = 0;
	while (i < HASH_TABLE_SIZE)
	{
		if (shell->env_table->buckets[i] != NULL)
		{
			shell->env_array[j] =
				get_entry_from_table(shell->env_table->buckets[i], shell);
			if (!shell->env_array[j++])
				return (NULL);
			temp_entry = shell->env_table->buckets[i]->next;
			while (temp_entry)
			{
				shell->env_array[j] = get_entry_from_table(temp_entry, shell);
				if (!shell->env_array[j++])
					return (NULL);
				temp_entry = temp_entry->next;
			}
		}
		i++;
	}
	return (shell->env_array);
}

static char	*get_entry_from_table(t_env_entry *entry, t_shell *shell)
{
	char		*temp1;
	char		*temp2;

	temp1 = ft_strjoin(entry->key, "=");
	if (!temp1)
		return (NULL);
	temp2 = ft_strjoin(temp1, entry->value);
	free(temp1);
	if (!temp2)
		return (NULL);
	temp1 = arena_strdup(temp2, shell->session_arena);
	free(temp2);
	if (!temp1)
		return (NULL);
	return (temp1);
}
