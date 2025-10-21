/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 19:02:40 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/21 21:12:43 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*arena_substr(char *src, size_t start, size_t len, t_arena *arena)
{
	char	*substr;
	size_t	i;

	if (!src || !arena || start >= ft_strlen(src))
		return (NULL);
	substr = arena_alloc(arena, len + 1);
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len && src[start + i])
	{
		substr[i] = src[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

char	*arena_strdup(const char *s, t_arena *arena)
{
	char	*copy;

	if (!s || !arena)
		return (NULL);
	copy = arena_alloc(arena, ft_strlen(s) + 1);
	if (!copy)
		return (NULL);
	ft_strlcpy(copy, s, ft_strlen(s) + 1);
	return (copy);
}

char	*skip_whitespace(char *pos)
{
	if (!pos)
		return (NULL);
	while (*pos && ((*pos >= 9 && *pos <= 13) || *pos == 32))
		pos++;
	return (pos);
}

int	is_whitespace_only(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	return (str[i] == '\0');
}

int	ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}
