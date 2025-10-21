/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_arena.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 19:10:55 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/21 21:11:56 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_count_words(char const *s, char c)
{
	size_t	words;
	int		in_word;

	words = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			words++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (words);
}

static char	*ft_extract_word(char const *s, size_t start, size_t len,
		t_arena *arena)
{
	char	*word;
	size_t	i;

	word = arena_alloc(arena, len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[start + i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static int	ft_process_words(char const *s, char c, char **result,
		t_arena *arena)
{
	size_t	i;
	size_t	start;
	size_t	word_idx;

	i = 0;
	word_idx = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] && s[i] != c)
				i++;
			result[word_idx] = ft_extract_word(s, start, i - start, arena);
			if (!result[word_idx])
				return (0);
			word_idx++;
		}
		else
			i++;
	}
	result[word_idx] = NULL;
	return (1);
}

char	**ft_split_arena(char const *s, char c, t_arena *arena)
{
	char	**result;
	size_t	word_count;

	if (!s)
		return (NULL);
	word_count = ft_count_words(s, c);
	result = arena_alloc(arena, (word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	if (!ft_process_words(s, c, result, arena))
		return (NULL);
	return (result);
}
