/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:08:01 by magebreh          #+#    #+#             */
/*   Updated: 2025/04/26 19:17:34 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_split(char **result, int words_done)
{
	int	i;

	i = 0;
	while (i < words_done)
	{
		free(result[i]);
		i++;
	}
	free(result);
}

size_t	word_length(const char *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

int	count_words(const char *s, char c)
{
	int	i;
	int	chr_in_word;
	int	word;

	i = 0;
	chr_in_word = 0;
	word = 0;
	while (s[i])
	{
		if ((s[i] != c) && (chr_in_word == 0))
		{
			word++;
			chr_in_word = 1;
		}
		else if (s[i] == c)
			chr_in_word = 0;
		i++;
	}
	return (word);
}

int	fill_words(char **result, const char *s, char c)
{
	int	i;
	int	w;
	int	len;

	i = 0;
	w = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i])
		{
			len = word_length(&s[i], c);
			result[w] = ft_substr(s, i, len);
			if (!result[w])
			{
				free_split(result, w);
				return (0);
			}
			i += len;
			w++;
		}
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	int		word_count;
	char	**result;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	result = malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	if (!fill_words(result, s, c))
		return (NULL);
	result[word_count] = NULL;
	return (result);
}
