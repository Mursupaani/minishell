/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 17:42:27 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/20 23:22:03 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	attach_heredoc_filename_to_command(t_command *cmd, t_arena *arena)
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

int	process_heredoc_redir(t_command *cmd, t_shell *shell, int *error)
{
	if (++shell->heredoc_counter > 16)
	{
		write(2, "minishell: maximum here-document count exceeded\n", 48);
		*error = 1;
		shell->heredoc_counter = 0;
		return (1);
	}
	if (attach_heredoc_filename_to_command(cmd, shell->command_arena) != 0)
	{
		shell->heredoc_counter = 0;
		return (1);
	}
	return (0);
}

int	is_redir(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_REDIR_APPEND || token->type == TOKEN_HEREDOC);
}
