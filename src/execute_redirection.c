/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:33:51 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/12 14:52:27 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

static char	*execute_input_redirection(t_redir *redirection, t_shell *shell);
static char	*execute_output_redirection(t_redir *redirection, t_shell *shell);
static char	*execute_heredoc(t_redir *redirection, t_shell *shell);

char	*execute_redirection(t_redir *redirection, t_shell *shell)
{
	//FIXME: What should the return value be?
	char	*target;

	fprintf(stderr, "Redir type %d\n", redirection->type);
	if (redirection->type == REDIR_INPUT)
		target = execute_input_redirection(redirection, shell);
	else if (redirection->type == REDIR_OUTPUT
		|| redirection->type == REDIR_APPEND)
		target = execute_output_redirection(redirection, shell);
	else if (redirection->type == REDIR_HEREDOC)
		target = execute_heredoc(redirection, shell);
	if (redirection->next)
		return (execute_redirection(redirection->next, shell));
	else
		return (target);
}

static char	*execute_input_redirection(t_redir *redirection, t_shell *shell)
{
	if (!redirection || !shell)
		return (NULL);
	redirection->fd = open(redirection->target, O_RDONLY);
	if (redirection->fd == -1)
		//FIXME:Fix error handling
		perror(strerror(errno));
	close(STDIN_FILENO);
	dup(redirection->fd);
	close(redirection->fd);
	(void)shell;
	return (NULL);
}

static char	*execute_output_redirection(t_redir *redirection, t_shell *shell)
{
	if (!redirection || !shell)
		return (NULL);
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
	return (NULL);
}

static char	*execute_heredoc(t_redir *redirection, t_shell *shell)
{
	char	*input;

	if (!redirection || !shell)
		return (NULL);
	shell->tmp_dir = ".tmp/";
	while (true)
	{
		input = readline("> ");
		if (ft_strncmp(input, redirection->target, ft_strlen(input)) == 0)
		{
			free(input);
			break ;
		}
		free(input);
	}
	return (NULL);
}
