/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 17:39:49 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/20 17:45:46 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
