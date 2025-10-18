/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 17:47:59 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/16 18:26:37 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	valid_format(char *key);

char	*get_entry_key(char *entry, t_arena *arena)
{
	int		i;
	char	*key;

	if (!entry || !arena)
		return (NULL);
	if (!valid_format(entry))
		return (NULL);
	i = 0;
	while (entry[i] && entry[i] != '=' && entry[i] != '+')
		i++;
	key = arena_alloc(arena, i + 1);
	if (!key)
		return (NULL);
	i = 0;
	while (entry[i] && entry[i] != '=' && entry[i] != '+')
	{
		key[i] = entry[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

char	*get_entry_value(char *entry, t_arena *arena)
{
	int		i;
	int		j;
	char	*value;

	if (!entry || !arena)
		return (NULL);
	value = arena_alloc(arena, ft_strlen(entry) + 1);
	if (!value)
	{
		ft_fprintf(STDERR_FILENO, "Failed to export variable\n");
		return (NULL);
	}
	j = 0;
	i = 0;
	while (entry[i])
	{
		while (entry[i] == ' ' && entry[i + 1] == ' ')
			i++;
		value[j++] = entry[i];
		i++;
	}
	value[j] = '\0';
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
			if (key[i] == '+' && key[i + 1] == '=')
				return (true);
			ft_fprintf(STDERR_FILENO,
				"minishell: export: `%s': not a valid identifier\n", key);
			return (false);
		}
		i++;
	}
	return (true);
}
