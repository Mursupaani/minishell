/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:04:52 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/01 14:35:36 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	has_append(t_command *cmd);

void	find_non_empty_argument(t_command *cmd, t_shell *shell)
{
	if (!cmd)
		exit_and_free_memory(EXIT_FAILURE, shell, cmd);
	if (!cmd->argv)
	{
		if (has_append(cmd))
			exit_and_free_memory(EXIT_SUCCESS, shell, cmd);
		else
			exit_and_free_memory(EXIT_FAILURE, shell, cmd);
	}
	while (*cmd->argv)
	{
		if (*cmd->argv[0] != '\0')
			break ;
		cmd->argv++;
	}
	if (!*cmd->argv)
	{
		ft_fprintf(STDERR_FILENO, "Command '' not found\n");
		exit_and_free_memory(127, shell, cmd);
	}
}

static bool	has_append(t_command *cmd)
{
	t_redir	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == REDIR_APPEND)
			return (true);
		redir = redir->next;
	}
	return (false);
}
