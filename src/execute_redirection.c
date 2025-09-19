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

char	*execute_input_redirection(t_redir *redirection, t_shell *shell);
char	*execute_output_redirection(t_redir *redirection, t_shell *shell);
char	*execute_error_redirection(t_redir *redirection, t_shell *shell);
char	*execute_heredoc(t_redir *redirection, t_shell *shell);

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
	else if (redirection->type == REDIR_ERROR)
		target = execute_error_redirection(redirection, shell);
	else if (redirection->type == REDIR_HEREDOC)
		target = execute_heredoc(redirection, shell);
	if (redirection->next)
	{
		close(redirection->fd);
		return (execute_redirection(redirection->next, shell));
	}
	else
		return (target);
}

char	*execute_input_redirection(t_redir *redirection, t_shell *shell)
{
	(void)redirection;
	(void)shell;
	close(STDIN_FILENO);
	if (redirection->type == REDIR_INPUT)
		redirection->fd = open(redirection->target, O_RDONLY);
	return (NULL);
}

char	*execute_output_redirection(t_redir *redirection, t_shell *shell)
{
	(void)shell;
	if (!redirection || !shell)
		return (NULL);
	close(STDOUT_FILENO);
	if (redirection->type == REDIR_OUTPUT)
		redirection->fd = open(redirection->target, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (redirection->type == REDIR_APPEND)
		redirection->fd = open(redirection->target, O_CREAT | O_APPEND | O_RDWR, 0644);
	if (redirection->fd == -1)
		//FIXME:Fix error handling
		perror(strerror(errno));
	return (NULL);
}

char	*execute_error_redirection(t_redir *redirection, t_shell *shell)
{
	close(STDERR_FILENO);
	(void)shell;
	(void)redirection;
	return (NULL);
}

char	*execute_heredoc(t_redir *redirection, t_shell *shell)
{
	char	*input;

	(void)shell;
	if (!redirection || !shell)
		return (NULL);
	close(STDIN_FILENO);
	while (true)
	{
		input = readline("heredoc> ");
		if (ft_strncmp(input, redirection->target, ft_strlen(input)) == 0)
		{
			free(input);
			break ;
		}
		write (STDIN_FILENO, input, ft_strlen(input));
		free(input);
	}
	return (NULL);
}
