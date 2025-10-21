/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:45:53 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/20 17:44:59 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static void	ensure_command_exists(t_command **head, t_command **current,
		t_arena *arena)
{
	if (!*current)
	{
		*current = create_command(arena);
		if (!*head)
			*head = *current;
	}
}

static t_token	*handle_pipe_token(t_command **head, t_command **current,
		t_arena *arena)
{
	t_command	*new_cmd;

	if (!*current || !(*current)->argv || !(*current)->argv[0])
		return (NULL);
	new_cmd = create_command(arena);
	if (!new_cmd)
		return (NULL);
	if (!*head)
		*head = new_cmd;
	else
		(*current)->next = new_cmd;
	*current = new_cmd;
	return ((t_token *)1);
}

static int	process_token(t_command **head, t_command **current,
		t_token **token, t_shell *shell)
{
	int	redir_error;

	if ((*token)->type == TOKEN_PIPE)
	{
		if (!handle_pipe_token(head, current, shell->command_arena))
			return (0);
		*token = (*token)->next;
		return (1);
	}
	else if (is_redir(*token))
	{
		ensure_command_exists(head, current, shell->command_arena);
		*token = handle_redir(*current, *token, shell, &redir_error);
		if (redir_error)
			return (0);
		return (1);
	}
	else
	{
		ensure_command_exists(head, current, shell->command_arena);
		add_word_cmd(*current, *token, shell->command_arena);
		*token = (*token)->next;
		return (1);
	}
}

t_command	*parse_pipeline(t_token *tokens, t_shell *shell)
{
	t_command	*head;
	t_command	*current;
	t_token		*token;

	head = NULL;
	current = NULL;
	token = tokens;
	shell->heredoc_counter = 0;
	while (token)
	{
		if (!process_token(&head, &current, &token, shell))
		{
			shell->heredoc_counter = 0;
			return (NULL);
		}
	}
	shell->heredoc_counter = 0;
	return (head);
}

void	classify_commands(t_command *cmd)
{
	t_command	*current;
	int			is_single;

	current = cmd;
	is_single = (cmd->next == NULL);
	while (current)
	{
		if (!current->argv || !current->argv[0])
		{
			current = current->next;
			continue ;
		}
		if (is_builtin_command(current->argv[0]))
		{
			if (is_single)
				current->cmd_type = CMD_BUILTIN_PARENT;
			else
				current->cmd_type = CMD_BUILTIN_CHILD;
		}
		else
			current->cmd_type = CMD_EXTERNAL;
		current = current->next;
	}
}
