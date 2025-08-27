/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:57:16 by magebreh          #+#    #+#             */
/*   Updated: 2025/08/27 19:28:38 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	else if (**pos == '"')
		return (tokenize_double_quote(pos, arena));
	else if (**pos == '\'')
		return (tokenize_single_quote(pos, arena));
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

static t_token *tokenize_double_quote(char **pos, t_arena *arena)
{
	t_token *token;
	char *start;
	char *end;
	size_t content_len;

	*(pos)++;
	start = *pos;
	end = *pos;
	while (*end && *end != '"')
		end++;
	if(!*end)
		return (NULL);
	content_len = end - start;
	token = arena_alloc(arena, sizeof(t_token));
	if(!token)
		return (NULL);
	token->value = arena_substr(start, 0, content_len, arena);
	if (!token->value)
		return (NULL);
	token->type = TOKEN_WORD;
	token->quoted = 2;
	token->expandable = 1;
	token->next = NULL;
	*pos = end + 1;
	return (token);
}

static t_token *tokenize_single_quote(char **pos, t_arena *arena)
{
	t_token *token;
	char *start;
	char *end;
	size_t content_len;

	*(pos)++;
	start = *pos;
	end = *pos;
	while(*end && *end != "'")
		end++;
	if(!*end)
		return (NULL);
	content_len = end - start;
	token = arena_alloc(arena, sizeof(t_token));
	if(!token)
		return (NULL);
	token->value = arena_substr(start, 0, content_len, arena);
	if(!token->value)
		return (NULL);
	token->type = TOKEN_WORD;
	token->quoted = 1;
	token->expandable = 0;
	token->next = NULL;
	*pos = end + 1;
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
	while(*end && !ft_is_special_char(*end) && !ft_isspace(*end));
		end++;
	if(start == end)
		return (NULL);
	word_len = end - start;
	token = arena_alloc(arena, sizeof(t_token));
	if(!token)
		return (NULL);
	token->value = arena_substr(start, 0, word_len, arena);
	if (!token->value)
		return (NULL);
	token->type = TOKEN_WORD;
	token->quoted = 0;
	token->expandable = (ft_strchr(token->value, '$') != NULL);
	token->next = NULL;
	*pos = end;
	return (token);
}
