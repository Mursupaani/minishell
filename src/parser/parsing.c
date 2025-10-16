/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:45:53 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/04 18:04:09 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int		attach_heredoc_filename_to_command(t_command *cmd,
					t_arena *arena);

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
		*token = handle_redir(*current, *token, shell->command_arena,
				&redir_error);
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
	while (token)
	{
		if (!process_token(&head, &current, &token, shell))
			return (NULL);
	}
	return (head);
}

t_command	*create_command(t_arena *arena)
{
	t_command	*cmd;

	cmd = arena_alloc(arena, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->argv_expandable = NULL;
	cmd->cmd_type = CMD_EXTERNAL;
	cmd->redirections = NULL;
	cmd->heredoc_filename = NULL;
	cmd->next = NULL;
	return (cmd);
}

int	is_redir(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_REDIR_APPEND || token->type == TOKEN_HEREDOC);
}

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

t_token	*handle_redir(t_command *current, t_token *token, t_arena *arena,
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
	redir = create_redir(token, target, arena);
	if (!redir)
	{
		*error = 1;
		return (NULL);
	}
	if (redir->type == REDIR_HEREDOC)
	{
		if (attach_heredoc_filename_to_command(current, arena) != 0)
			return (NULL);
	}
	attach_redir(current, redir);
	return (target->next);
}

static void	init_new_arrays(char **new_argv, bool *new_expandable, int capacity)
{
	int	i;

	i = 0;
	while (i < capacity)
	{
		new_argv[i] = NULL;
		new_expandable[i] = false;
		i++;
	}
}

static void	copy_existing_args(char **new_argv, bool *new_expandable,
		char **old_argv, bool *old_expandable)
{
	int	i;

	i = 0;
	while (old_argv[i])
	{
		new_argv[i] = old_argv[i];
		new_expandable[i] = old_expandable[i];
		i++;
	}
}

static int	realloc_argv_if_needed(t_command *cmd, int current_count,
		int req_capacity, t_arena *arena)
{
	char	**new_argv;
	bool	*new_expandable;

	if (!cmd->argv || needs_realloc(current_count + 1))
	{
		new_argv = arena_alloc(arena, req_capacity * sizeof(char *));
		new_expandable = arena_alloc(arena, req_capacity * sizeof(bool));
		if (!new_argv || !new_expandable)
			return (0);
		init_new_arrays(new_argv, new_expandable, req_capacity);
		if (cmd->argv)
			copy_existing_args(new_argv, new_expandable, cmd->argv,
				cmd->argv_expandable);
		cmd->argv = new_argv;
		cmd->argv_expandable = new_expandable;
	}
	return (1);
}

void	add_word_cmd(t_command *cmd, t_token *word, t_arena *arena)
{
	int	current_count;
	int	req_capacity;

	current_count = 0;
	if (cmd->argv)
	{
		while (cmd->argv[current_count])
			current_count++;
	}
	req_capacity = calculate_new_capacity(current_count + 1);
	if (!realloc_argv_if_needed(cmd, current_count, req_capacity, arena))
		return ;
	cmd->argv[current_count] = arena_strdup(word->value, arena);
	cmd->argv_expandable[current_count] = word->expandable;
	cmd->argv[current_count + 1] = NULL;
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

int	needs_realloc(int current_count)
{
	int	capacity;

	if (current_count == 0)
		return (1);
	capacity = 8;
	while (capacity <= current_count)
		capacity *= 2;
	return (current_count >= capacity / 2);
}

int	calculate_new_capacity(int current_count)
{
	int	capacity;

	if (current_count == 0)
		return (8);
	capacity = 8;
	while (capacity <= current_count)
		capacity *= 2;
	return (capacity);
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

static int	attach_heredoc_filename_to_command(t_command *cmd, t_arena *arena)
{
	static unsigned int	file_counter;
	const char			*heredoc_name_base = ".heredoc-";
	char				*temp_num;
	char				*temp_path;

	while (cmd->heredoc_filename == NULL)
	{
		temp_num = ft_itoa(file_counter++);
		if (!temp_num)
			return (1);
		temp_path = ft_strjoin(heredoc_name_base, temp_num);
		free(temp_num);
		if (!temp_path)
			return (1);
		if (access(temp_path, F_OK) == 0)
		{
			free(temp_path);
			continue ;
		}
		cmd->heredoc_filename = arena_strdup(temp_path, arena);
		free(temp_path);
	}
	return (0);
}
