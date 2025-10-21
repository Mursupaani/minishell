/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:57:16 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/20 18:56:14 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*create_next_token(char **pos, t_arena *arena);
static t_token	*tokenize_pipe(char **pos, t_arena *arena);
static t_token	*tokenize_input_redirect(char **pos, t_arena *arena);
static t_token	*tokenize_output_redirect(char **pos, t_arena *arena);

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
	if (**pos == ';')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `;'\n", 2);
		return (NULL);
	}
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
