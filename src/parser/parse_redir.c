/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 17:41:22 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/20 23:21:50 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*create_redir(t_token *token, t_token *target, t_arena *arena)
{
	t_redir	*redir;

	redir = arena_alloc(arena, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = token_to_redir_type(token->type);
	redir->target = arena_strdup(target->value, arena);
	if (!redir->target)
		return (NULL);
	redir->fd = -1;
	if (token->type == TOKEN_HEREDOC)
		redir->heredoc_expand = (target->quoted == 0);
	else
		redir->heredoc_expand = false;
	redir->next = NULL;
	return (redir);
}

t_token	*handle_redir(t_command *current, t_token *token, t_shell *shell,
		int *error)
{
	t_redir	*redir;
	t_token	*target;

	*error = 0;
	target = token->next;
	if (!target || target->type != TOKEN_WORD)
	{
		*error = 1;
		return (NULL);
	}
	redir = create_redir(token, target, shell->command_arena);
	if (!redir)
	{
		*error = 1;
		return (NULL);
	}
	if (redir->type == REDIR_HEREDOC)
	{
		if (process_heredoc_redir(current, shell, error) != 0)
			return (NULL);
	}
	attach_redir(current, redir);
	return (target->next);
}

t_redir_type	token_to_redir_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return (REDIR_INPUT);
	else if (token_type == TOKEN_REDIR_OUT)
		return (REDIR_OUTPUT);
	else if (token_type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	else if (token_type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	else
		return (REDIR_INPUT);
}

void	attach_redir(t_command *cmd, t_redir *redir)
{
	t_redir	*tail;

	if (!cmd->redirections)
		cmd->redirections = redir;
	else
	{
		tail = cmd->redirections;
		while (tail->next)
			tail = tail->next;
		tail->next = redir;
	}
}
