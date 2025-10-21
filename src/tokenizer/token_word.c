/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:40:40 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/20 18:55:25 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		process_quoted_word(t_token *token, char *start, char *end,
					t_arena *arena);
static int		process_unquoted_word(t_token *token, char *start,
					size_t word_len, t_arena *arena);
static char		*find_word_end(char *start);
static char		*skip_quoted_section(char *end);

t_token	*tokenize_word(char **pos, t_arena *arena)
{
	t_token	*token;
	char	*start;
	char	*end;
	size_t	word_len;

	start = *pos;
	end = find_word_end(start);
	if (!end || start == end)
		return (NULL);
	word_len = end - start;
	token = arena_alloc(arena, sizeof(t_token));
	if (!token)
		return (NULL);
	if (ft_strchr_range(start, '"', word_len) || ft_strchr_range(start, '\'',
			word_len))
	{
		if (!process_quoted_word(token, start, end, arena))
			return (NULL);
	}
	else if (!process_unquoted_word(token, start, word_len, arena))
		return (NULL);
	token->type = TOKEN_WORD;
	token->next = NULL;
	*pos = end;
	return (token);
}

static int	process_quoted_word(t_token *token, char *start, char *end,
		t_arena *arena)
{
	size_t	word_len;

	word_len = end - start;
	token->value = arena_substr(start, 0, word_len, arena);
	if (!token->value)
		return (0);
	set_token_quote_flags(token, start, word_len);
	return (1);
}

static int	process_unquoted_word(t_token *token, char *start, size_t word_len,
		t_arena *arena)
{
	token->value = arena_substr(start, 0, word_len, arena);
	token->quoted = 0;
	token->expandable = (ft_strchr(token->value, '$') != NULL);
	if (!token->value)
		return (0);
	return (1);
}

static char	*find_word_end(char *start)
{
	char	*end;

	end = start;
	while (*end && !ft_is_special_char(*end) && !ft_isspace(*end))
	{
		if (is_quote(*end))
		{
			end = skip_quoted_section(end);
			if (!end)
				return (NULL);
			continue ;
		}
		if (*end == '=' && is_quote(*(end + 1)))
		{
			end++;
			end = skip_quoted_section(end);
			if (!end)
				return (NULL);
			continue ;
		}
		end++;
	}
	return (end);
}

static char	*skip_quoted_section(char *end)
{
	char	quote_char;

	quote_char = *end;
	end++;
	while (*end && *end != quote_char)
		end++;
	if (*end == quote_char)
		end++;
	else
		return (NULL);
	return (end);
}
