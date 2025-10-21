/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toke_quote.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:50:29 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/20 19:01:03 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static int	is_expandable_var(char *start, size_t i, size_t word_len,
		char in_quote)
{
	if (start[i] != '$' || i + 1 >= word_len)
		return (0);
	if (is_quote(start[i + 1]))
		return (-1);
	if (ft_isalnum(start[i + 1]) || start[i + 1] == '_' || start[i
			+ 1] == '?')
	{
		if (in_quote != '\'')
			return (1);
	}
	return (0);
}

static int	check_expandable_in_context(char *start, size_t word_len)
{
	size_t	i;
	char	in_quote;
	int		result;

	i = -1;
	in_quote = 0;
	while (++i < word_len)
	{
		if (is_quote(start[i]) && !in_quote)
			in_quote = start[i];
		else if (start[i] == in_quote)
			in_quote = 0;
		else
		{
			result = is_expandable_var(start, i, word_len, in_quote);
			if (result == 1)
				return (1);
			if (result == -1)
				i++;
		}
	}
	return (0);
}

void	set_token_quote_flags(t_token *token, char *start, size_t word_len)
{
	if (ft_strchr_range(start, '\'', word_len) && !ft_strchr_range(start, '"',
			word_len))
	{
		token->quoted = 1;
		token->expandable = 0;
	}
	else if (ft_strchr_range(start, '"', word_len))
	{
		token->quoted = 2;
		token->expandable = check_expandable_in_context(start, word_len);
	}
	else
	{
		token->quoted = 0;
		token->expandable = check_expandable_in_context(start, word_len);
	}
}

char	*ft_strchr_range(char *str, char c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (str[i] == c)
			return (&str[i]);
		i++;
	}
	return (NULL);
}
