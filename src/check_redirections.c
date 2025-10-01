/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 14:35:14 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/01 14:39:19 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_input_redirection(t_redir *redir, t_shell *shell)
{
	if (!redir || !shell)
		return (-1);
	if (redir->type == REDIR_INPUT)
	{
		if (access(redir->target, R_OK == -1))
		{
			ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
				redir->target, strerror(errno));
			shell->last_exit_status = 1;
			return (-1);
		}
	}
	return (0);
}

int	check_output_redirection(t_redir *redir, t_shell *shell)
{
	if (!redir || !shell)
		return (-1);
	if (redir->type == REDIR_OUTPUT)
	{
		if (access(redir->target, F_OK) == 0
		&& access(redir->target, W_OK == -1))
		{
			ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
				redir->target, strerror(errno));
			shell->last_exit_status = 1;
			return (-1);
		}
	}
	redir = redir->next;
	return (0);
}

int	check_append_redirection(t_redir *redir, t_shell *shell)
{
	if (!redir || !shell)
		return (-1);
	if (redir->type == REDIR_APPEND)
	{
		if (access (redir->target, F_OK)
			&& access(redir->target, (R_OK & W_OK) == -1))
		{
			ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
				redir->target, strerror(errno));
			shell->last_exit_status = 1;
			return (-1);
		}
	}
	return (0);
}
