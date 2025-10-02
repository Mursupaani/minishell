/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:57:16 by magebreh          #+#    #+#             */
/*   Updated: 2025/09/15 11:36:04 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Forward declarations for static functions
static t_token *create_next_token(char **pos, t_arena *arena);
static t_token *tokenize_pipe(char **pos, t_arena *arena);
static t_token *tokenize_input_redirect(char **pos, t_arena *arena);
static t_token *tokenize_output_redirect(char **pos, t_arena *arena);
static t_token *tokenize_word(char **pos, t_arena *arena);
static char *build_assignment_token(char *start, char *end, t_arena *arena);
static char *remove_quotes_from_word(char *start, size_t word_len, t_arena *arena);
static char *ft_strchr_range(char *str, char c, size_t len);

t_token *tokenize(char *input, t_arena *arena)
{
	t_token *head;
	t_token *current;
	t_token *new_token;
	char *pos;

	head = NULL;
	current = NULL;
	pos = input;
	while (*pos)
	{
		pos = skip_whitespace(pos);
		if (!*pos)
			break;
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

static t_token *create_next_token(char **pos, t_arena *arena)
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

static t_token *tokenize_pipe(char **pos, t_arena *arena)
{
	t_token *token;
	
	token = arena_alloc(arena, sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = arena_strdup("|", arena);
	if(!token->value)
		return (NULL);
	token->type = TOKEN_PIPE;
	token->quoted = 0;
	token->expandable = 0;
	token->next = NULL;
	(*pos)++;
	return (token);
}

static t_token *tokenize_input_redirect(char **pos, t_arena *arena)
{
	t_token *token;

	token = arena_alloc(arena, sizeof(t_token));
	if (!token)
		return (NULL);
	if(**pos == '<' && *(*pos + 1) == '<')
	{
		token->type = TOKEN_HEREDOC;
		token->value = arena_strdup("<<", arena);
		if(!token->value)
			return (NULL);
		*pos += 2;
	}
	else
	{
		token->type = TOKEN_REDIR_IN;
		token->value = arena_strdup("<", arena);
		if(!token->value)
			return (NULL);
		(*pos)++;
	}
	token->quoted = 0;
	token->expandable = 0;
	token->next = NULL;
	return(token);
}

static t_token *tokenize_output_redirect(char **pos, t_arena *arena)
{
	t_token *token;
	
	token = arena_alloc(arena, sizeof(t_token));
	if(!token)
		return (NULL);
	if(**pos == '>' && *(*pos + 1) == '>')
	{
		token->type = TOKEN_REDIR_APPEND;
		token->value = arena_strdup(">>", arena);
		if(!token->value)
			return (NULL);
		*pos += 2;
	}
	else
	{
		token->type = TOKEN_REDIR_OUT;
		token->value = arena_strdup(">", arena);
		if(!token->value)
			return (NULL);
		(*pos)++;
	}
	token->quoted = 0;
	token->expandable = 0;
	token->next = NULL;
	return (token);
}


static t_token *tokenize_word(char **pos, t_arena *arena)
{
	t_token *token;
	char *start;
	char *end;
	size_t word_len;

	start = *pos;
	end = *pos;
	while(*end && !ft_is_special_char(*end) && !ft_isspace(*end))
	{
		if (*end == '\'' || *end == '"')
		{
			char quote_char = *end;
			end++;
			while (*end && *end != quote_char)
				end++;
			if (*end == quote_char)
				end++;
			else
				return (NULL);
			continue;
		}
		if (*end == '=' && (*(end + 1) == '"' || *(end + 1) == '\''))
		{
			end++;
			char quote_char = *end;
			end++;
			while (*end && *end != quote_char)
				end++;
			if (*end == quote_char)
				end++;
			else
				return (NULL);
			continue;
		}
		end++;
	}
	if(start == end)
		return (NULL);
	word_len = end - start;
	token = arena_alloc(arena, sizeof(t_token));
	if(!token)
		return (NULL);
	if (ft_strchr_range(start, '"', word_len) || ft_strchr_range(start, '\'', word_len))
	{
		if (ft_strchr(start, '='))
			token->value = build_assignment_token(start, end, arena);
		else
			token->value = remove_quotes_from_word(start, word_len, arena);

		if (ft_strchr_range(start, '\'', word_len) && !ft_strchr_range(start, '"', word_len))
		{
			token->quoted = 1;
			token->expandable = 0;
		}
		else if (ft_strchr_range(start, '"', word_len))
		{
			token->quoted = 2;
			token->expandable = (ft_strchr(token->value, '$') != NULL);
		}
		else
		{
			token->quoted = 0;
			token->expandable = (ft_strchr(token->value, '$') != NULL);
		}
	}
	else
	{
		token->value = arena_substr(start, 0, word_len, arena);
		token->quoted = 0;
		token->expandable = (ft_strchr(token->value, '$') != NULL);
	}
	if (!token->value)
		return (NULL);
	token->type = TOKEN_WORD;
	token->next = NULL;
	*pos = end;
	return (token);
}

static char *remove_quotes_from_word(char *start, size_t word_len, t_arena *arena)
{
	char *result;
	char *src;
	char *dst;
	char *end;

	result = arena_alloc(arena, word_len + 1);
	if (!result)
		return (NULL);

	src = start;
	dst = result;
	end = start + word_len;

	while (src < end)
	{
		if (*src == '\'' || *src == '"')
		{
			char quote_char = *src;
			src++;
			while (src < end && *src != quote_char)
			{
				*dst++ = *src++;
			}
			if (src < end && *src == quote_char)
				src++;
		}
		else
		{
			*dst++ = *src++;
		}
	}
	*dst = '\0';
	return (result);
}

static char *ft_strchr_range(char *str, char c, size_t len)
{
	size_t i;

	i = 0;
	while (i < len)
	{
		if (str[i] == c)
			return (&str[i]);
		i++;
	}
	return (NULL);
}

char *build_assignment_token(char *start, char *end, t_arena *arena)
{
    char *equals_pos;
    size_t prefix_len;
	char *src;
	char *dst;
	char *result;

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
	while (src < end)
	{
		if (*src == '\'' || *src == '"')
		{
			char quote_char = *src;
			src++;
			while (src < end && *src != quote_char)
				*dst++ = *src++;
			if (src < end && *src == quote_char)
				src++;
		}
		else
			*dst++ = *src++;
	}
	*dst = '\0';
    return (result);
}
