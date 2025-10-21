/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 19:08:59 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/20 23:07:21 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_shell_partial(t_shell *shell, int level)
{
	if (level >= 3)
		arena_free(&shell->command_arena);
	if (level >= 2)
		arena_free(&shell->session_arena);
	if (level >= 1)
		free(shell);
}

int	cleanup_after_execution(t_shell *shell, t_command *cmd)
{
	while (cmd)
	{
		if (cmd->heredoc_filename != NULL)
		{
			if (unlink(cmd->heredoc_filename))
				perror(strerror(errno));
		}
		cmd = cmd->next;
	}
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	arena_reset(shell->command_arena);
	return (0);
}
