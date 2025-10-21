/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:21:36 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/20 15:29:32 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_quote_char(char **src, char **dst, char *in_quote)
{
	if ((**src == '"' && *in_quote != '\'')
		|| (**src == '\'' && *in_quote != '"'))
	{
		if (**src == *in_quote)
			*in_quote = 0;
		else if (!*in_quote)
			*in_quote = **src;
		(*src)++;
	}
	else
		*(*dst)++ = *(*src)++;
}

char	*strip_quotes(char *str, t_arena *arena)
{
	char	*res;
	char	*src;
	char	*dst;
	char	in_quote;

	if (!str)
		return (NULL);
	res = arena_alloc(arena, ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	src = str;
	dst = res;
	in_quote = 0;
	while (*src)
		process_quote_char(&src, &dst, &in_quote);
	*dst = '\0';
	return (res);
}

static int	count_quote_aware_words(char *str)
{
	int		count;
	char	in_quote;
	bool	in_word;

	count = 0;
	in_quote = 0;
	in_word = false;
	while (*str)
	{
		if ((*str == '"' || *str == '\'') && !in_quote)
			in_quote = *str;
		else if (*str == in_quote)
			in_quote = 0;
		if (!ft_isspace(*str) && !in_word)
		{
			in_word = true;
			count++;
		}
		else if (ft_isspace(*str) && !in_quote && in_word)
			in_word = false;
		str++;
	}
	return (count);
}

char	**quote_aware_split(char *str)
{
	char	**result;
	int		count;
	int		i;

	if (!str)
		return (NULL);
	count = count_quote_aware_words(str);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (*str && i < count)
	{
		while (ft_isspace(*str))
			str++;
		if (*str)
			result[i++] = extract_word(str, &str);
	}
	result[i] = NULL;
	return (result);
}

char	*extract_word(char *str, char **end)
{
	char	*start;
	char	in_quote;
	int		len;

	start = str;
	in_quote = 0;
	while (*str && (in_quote || !ft_isspace(*str)))
	{
		if ((*str == '"' || *str == '\'') && !in_quote)
			in_quote = *str;
		else if (*str == in_quote)
			in_quote = 0;
		str++;
	}
	len = str - start;
	*end = str;
	return (ft_substr(start, 0, len));
}
