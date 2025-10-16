/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredocs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 11:40:45 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/13 14:13:14 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	generate_heredoc_file(
				t_redir *redirection, t_command *cmd, t_shell *shell);
static int	get_user_input_to_heredoc(
				t_redir *redirection, int fd, t_shell *shell);
static int	process_heredocs(t_command *cmd, t_shell *shell);
static int	handle_heredoc_input(
				char **input, t_redir *redirection, int fd, t_shell *shell);

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

static int	process_heredocs(t_command *cmd, t_shell *shell)
{
	t_redir	*temp;
	int		status;

	while (cmd && g_signal_received != SIGINT)
	{
		temp = cmd->redirections;
		while (temp && g_signal_received != SIGINT)
		{
			if (temp->type == REDIR_HEREDOC)
			{
				status = generate_heredoc_file(temp, cmd, shell);
				if (status != 0)
				{
					if (status == 2)
						return (2);
					cmd->heredoc_filename = NULL;
					return (1);
				}
			}
			temp = temp->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

static int	generate_heredoc_file(
				t_redir *redirection, t_command *cmd, t_shell *shell)
{
	int		fd;
	int		status;

	if (!shell || !cmd || !redirection)
		return (1);
	fd = open(cmd->heredoc_filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd == -1)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: heredoc: failed to create input file\n");
		return (-1);
	}
	status = get_user_input_to_heredoc(redirection, fd, shell);
	close(fd);
	if (status == -1)
		return (-1);
	else if (status == 1)
		ft_fprintf(STDERR_FILENO,
			"minishell: warning: here-document delimited by end-of-file \
(wanted `%s')\n", redirection->target);
	else if (status == 2)
		return (2);
	return (0);
}

static int	get_user_input_to_heredoc(
				t_redir *redirection, int fd, t_shell *shell)
{
	char	*input;
	int		status;

	while (g_signal_received != SIGINT)
	{
		input = readline("> ");
		if (!input)
			return (1);
		status = handle_heredoc_input(&input, redirection, fd, shell);
		if (status == -1)
			return (1);
		else if (status == 1)
			return (0);
	}
	return (2);
}

static int	handle_heredoc_input(
				char **input, t_redir *redirection, int fd, t_shell *shell)
{
	if (**input == '\0')
	{
		free(*input);
		*input = ft_calloc(1, 2);
		if (!*input)
			return (-1);
		*input[0] = '\n';
	}
	if (ft_strncmp(*input, redirection->target,
			ft_strlen(redirection->target) + 1) == 0
		&& ft_strlen(*input) == ft_strlen(redirection->target))
	{
		free(*input);
		return (1);
	}
	if (redirection->heredoc_expand)
		*input = expand_variables_from_input(*input, shell);
	if (!*input)
		return (-1);
	write(fd, *input, ft_strlen(*input));
	write(fd, "\n", 1);
	free(*input);
	return (0);
}
