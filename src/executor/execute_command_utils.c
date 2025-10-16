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

void	find_non_empty_argument(t_command *cmd)
{
	if (!cmd)
		exit(EXIT_FAILURE);
	if (!cmd->argv)
	{
		if (has_append(cmd))
			exit(EXIT_SUCCESS);
		else
			exit(EXIT_FAILURE);
	}
	while (*cmd->argv)
	{
		if (*cmd->argv[0] != '\0')
			break ;
		cmd->argv++;
	}
	if (!*cmd->argv)
		exit(EXIT_SUCCESS);
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
