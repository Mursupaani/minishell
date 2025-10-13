/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:57:16 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/13 14:15:08 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Forward declarations for static functions
static t_token	*create_next_token(char **pos, t_arena *arena);
static t_token	*tokenize_pipe(char **pos, t_arena *arena);
static t_token	*tokenize_input_redirect(char **pos, t_arena *arena);
static t_token	*tokenize_output_redirect(char **pos, t_arena *arena);
static t_token	*tokenize_word(char **pos, t_arena *arena);
static char		*build_assignment_token(char *start, char *end, t_arena *arena);
static char		*remove_quotes_from_word(char *start, size_t word_len,
					t_arena *arena);
static char		*ft_strchr_range(char *str, char c, size_t len);

static int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

t_token	*tokenize(char *input, t_arena *arena)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	char	*pos;

	head = NULL;
	current = NULL;
	pos = input;
	while (*pos)
	{
		pos = skip_whitespace(pos);
		if (!*pos)
			break ;
		new_token = create_next_token(&pos, arena);
		if (!new_token)
			return (NULL);
		if (!head)
			head = new_token;
		else
			current->next = new_token;
		current = new_token;
	}
	return (head);
}

static t_token	*create_next_token(char **pos, t_arena *arena)
{
	if (**pos == '|')
		return (tokenize_pipe(pos, arena));
	else if (**pos == '<')
		return (tokenize_input_redirect(pos, arena));
	else if (**pos == '>')
		return (tokenize_output_redirect(pos, arena));
	else
		return (tokenize_word(pos, arena));
}

static t_token	*tokenize_pipe(char **pos, t_arena *arena)
{
	t_token	*token;

	token = arena_alloc(arena, sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = arena_strdup("|", arena);
	if (!token->value)
		return (NULL);
	token->type = TOKEN_PIPE;
	token->quoted = 0;
	token->expandable = 0;
	token->next = NULL;
	(*pos)++;
	return (token);
}

static t_token	*tokenize_input_redirect(char **pos, t_arena *arena)
{
	t_token	*token;

	token = arena_alloc(arena, sizeof(t_token));
	if (!token)
		return (NULL);
	if (**pos == '<' && *(*pos + 1) == '<')
	{
		token->type = TOKEN_HEREDOC;
		token->value = arena_strdup("<<", arena);
		if (!token->value)
			return (NULL);
		*pos += 2;
	}
	else
	{
		token->type = TOKEN_REDIR_IN;
		token->value = arena_strdup("<", arena);
		if (!token->value)
			return (NULL);
		(*pos)++;
	}
	token->quoted = 0;
	token->expandable = 0;
	token->next = NULL;
	return (token);
}

static t_token	*tokenize_output_redirect(char **pos, t_arena *arena)
{
	t_token	*token;

	token = arena_alloc(arena, sizeof(t_token));
	if (!token)
		return (NULL);
	if (**pos == '>' && *(*pos + 1) == '>')
	{
		token->type = TOKEN_REDIR_APPEND;
		token->value = arena_strdup(">>", arena);
		if (!token->value)
			return (NULL);
		*pos += 2;
	}
	else
	{
		token->type = TOKEN_REDIR_OUT;
		token->value = arena_strdup(">", arena);
		if (!token->value)
			return (NULL);
		(*pos)++;
	}
	token->quoted = 0;
	token->expandable = 0;
	token->next = NULL;
	return (token);
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

static int	check_expandable_in_context(char *start, size_t word_len)
{
	size_t	i;
	char	in_quote;

	i = 0;
	in_quote = 0;
	while (i < word_len)
	{
		if (is_quote(start[i]) && !in_quote)
			in_quote = start[i];
		else if (start[i] == in_quote)
			in_quote = 0;
		else if (start[i] == '$' && i + 1 < word_len)
		{
			if (is_quote(start[i + 1]))
			{
				i++;
				continue ;
			}
			if (ft_isalnum(start[i + 1]) || start[i + 1] == '_' || start[i
				+ 1] == '?')
			{
				if (in_quote != '\'')
					return (1);
			}
		}
		i++;
	}
	return (0);
}

static void	set_token_quote_flags(t_token *token, char *start, size_t word_len)
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

static int	process_quoted_word(t_token *token, char *start, char *end,
		t_arena *arena)
{
	size_t	word_len;

	word_len = end - start;
	if (ft_strchr(start, '='))
		token->value = build_assignment_token(start, end, arena);
	else
		token->value = remove_quotes_from_word(start, word_len, arena);
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

static t_token	*tokenize_word(char **pos, t_arena *arena)
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

static void	copy_quoted_content(char **src, char **dst, char *end,
		char quote_char)
{
	(*src)++;
	while (*src < end && **src != quote_char)
	{
		*(*dst)++ = *(*src)++;
	}
	if (*src < end && **src == quote_char)
		(*src)++;
}

static char	*remove_quotes_from_word(char *start, size_t word_len,
		t_arena *arena)
{
	char	*result;
	char	*src;
	char	*dst;
	char	*end;

	result = arena_alloc(arena, word_len + 2);
	if (!result)
		return (NULL);
	src = start;
	dst = result;
	end = start + word_len;
	while (src < end)
	{
		if (*src == '$' && src + 1 < end && is_quote(*(src + 1)))
			src++;
		else if (is_quote(*src))
			copy_quoted_content(&src, &dst, end, *src);
		else
			*dst++ = *src++;
	}
	*dst = '\0';
	return (result);
}

static char	*ft_strchr_range(char *str, char c, size_t len)
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

static void	copy_value_part(char **src, char **dst, char *end)
{
	while (*src < end)
	{
		if (is_quote(**src))
			copy_quoted_content(src, dst, end, **src);
		else
			*(*dst)++ = *(*src)++;
	}
}

char	*build_assignment_token(char *start, char *end, t_arena *arena)
{
	char	*equals_pos;
	size_t	prefix_len;
	char	*src;
	char	*dst;
	char	*result;

	equals_pos = start;
	while (*equals_pos != '=' && equals_pos < end)
		equals_pos++;
	prefix_len = (equals_pos - start) + 1;
	result = arena_alloc(arena, (end - start) + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, prefix_len + 1);
	src = equals_pos + 1;
	dst = result + prefix_len;
	copy_value_part(&src, &dst, end);
	*dst = '\0';
	return (result);
}
