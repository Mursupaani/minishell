/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredocs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 11:40:45 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/18 18:14:27 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredocs(t_command *cmd, t_shell *shell)
{
	int	status;

	if (!cmd)
		return (1);
	if (shell->is_a_tty)
		setup_heredoc_signals();
	status = process_heredocs(cmd, shell);
	if (shell->is_a_tty)
		setup_parent_signals();
	if (g_signal_received == SIGINT)
	{
		shell->last_exit_status = 130;
		return (130);
	}
	return (status);
}
