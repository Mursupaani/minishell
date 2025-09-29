/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:33:51 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/22 13:31:48 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
#include <unistd.h>

static void	execute_input_redirection(t_redir *redir, t_shell *shell);
static void	execute_output_redirection(t_redir *redir, t_shell *shell);
static void	execute_heredoc(t_redir *redir, t_command *cmd, t_shell *shell);

void	execute_redirection(t_redir *redir, t_command *cmd, t_shell *shell)
{
	if (redir->type == REDIR_INPUT)
		execute_input_redirection(redir, shell);
	else if (redir->type == REDIR_OUTPUT
		|| redir->type == REDIR_APPEND)
		execute_output_redirection(redir, shell);
	else if (redir->type == REDIR_HEREDOC)
		execute_heredoc(redir, cmd, shell);
	if (redir->next)
		return (execute_redirection(redir->next, cmd, shell));
}

static void	execute_input_redirection(t_redir *redir, t_shell *shell)
{
	if (!redir || !shell)
		return ;
	//FIXME: Try the file before coming this far!
	redir->fd = open(redir->target, O_RDONLY);
	if (redir->fd == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
			strerror(errno), redir->target);
		shell->last_exit_status = 1;
	}
	close(STDIN_FILENO);
	dup(redir->fd);
	close(redir->fd);
	return ;
}

static void	execute_output_redirection(t_redir *redir, t_shell *shell)
{
	if (!redir || !shell)
		return ;
	if (redir->type == REDIR_OUTPUT)
		redir->fd = open(redir->target, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (redir->type == REDIR_APPEND)
		redir->fd = open(redir->target, O_CREAT | O_APPEND | O_RDWR, 0644);
	if (redir->fd == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
			strerror(errno), redir->target);
		shell->last_exit_status = 1;
		return ;
	}
	close(STDOUT_FILENO);
	dup(redir->fd);
	close(redir->fd);
	return ;
}

static void	execute_heredoc(t_redir *redir, t_command *cmd, t_shell *shell)
{
	if (!redir || !cmd || !shell)
		return ;
	redir->fd = open(cmd->heredoc_filename, O_RDONLY);
	if (redir->fd == -1)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: heredoc: %s\n", strerror(errno));
		shell->last_exit_status = 1;
		return ;
	}
	close(STDIN_FILENO);
	dup(redir->fd);
	close(redir->fd);
	return ;
}
