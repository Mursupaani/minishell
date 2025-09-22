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

static int	execute_input_redirection(t_redir *redirection, t_shell *shell);
static int	execute_output_redirection(t_redir *redirection, t_shell *shell);
// static int	execute_heredoc(t_redir *redirection, t_shell *shell);

int	execute_redirection(t_redir *redirection, t_shell *shell)
{
	//FIXME: What should the return value be?
	int	success;

	fprintf(stderr, "Redir type %d\n", redirection->type);
	if (redirection->type == REDIR_INPUT
		|| redirection->type == REDIR_HEREDOC)
		success = execute_input_redirection(redirection, shell);
	else if (redirection->type == REDIR_OUTPUT
		|| redirection->type == REDIR_APPEND)
		success = execute_output_redirection(redirection, shell);
	// else if (redirection->type == REDIR_HEREDOC)
	// 	success = execute_heredoc(redirection, shell);
	if (redirection->next)
		return (execute_redirection(redirection->next, shell));
	else
		return (success);
}

static int	execute_input_redirection(t_redir *redirection, t_shell *shell)
{
	if (!redirection || !shell)
		return (1);
	redirection->fd = open(redirection->target, O_RDONLY);
	if (redirection->fd == -1)
		//FIXME:Fix error handling
		perror(strerror(errno));
	close(STDIN_FILENO);
	dup(redirection->fd);
	close(redirection->fd);
	(void)shell;
	return (0);
}

static int	execute_output_redirection(t_redir *redirection, t_shell *shell)
{
	if (!redirection || !shell)
		return (1);
	if (redirection->type == REDIR_OUTPUT)
		redirection->fd = open(redirection->target, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (redirection->type == REDIR_APPEND)
		redirection->fd = open(redirection->target, O_CREAT | O_APPEND | O_RDWR, 0644);
	if (redirection->fd == -1)
		//FIXME:Fix error handling
		perror(strerror(errno));
	close(STDOUT_FILENO);
	dup(redirection->fd);
	close(redirection->fd);
	(void)shell;
	return (0);
}
//
// static int	execute_heredoc(t_redir *redirection, t_shell *shell)
// {
// 	if (!redirection || !shell)
// 		return (1);
// 	redirection->fd = open(redirection->target, O_RDONLY);
// 	close(STDIN_FILENO);
// 	dup(redirection->fd);
// 	close(redirection->fd);
// 	return (0);
// }
