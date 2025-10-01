/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredocs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 11:40:45 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/22 13:04:36 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	generate_heredoc_file(t_redir *redirection, t_command *cmd);
static int	get_user_input_to_heredoc(t_redir *redirection, int fd);
static int	process_heredocs(t_command *cmd);

int	handle_heredocs(t_command *cmd, t_shell *shell)
{
	//FIXME: How to reproduce "bash: warning: here-document at line 1 delimited by end-of-file (wanted `eof')"
	// with ctrl-d?
	if (!cmd)
		return (1);
	// shell->child_pid = create_fork(shell);
	// if (shell->child_pid == 0)
	process_heredocs(cmd);
	(void)shell;
	// else
	// {
	// 	waitpid(shell->child_pid, &shell->last_exit_status, 0);
	// 	if (WIFEXITED(shell->last_exit_status))
	// 		shell->last_exit_status = WEXITSTATUS(shell->last_exit_status);
	// 	else if (WIFSIGNALED(shell->last_exit_status))
	// 		shell->last_exit_status = 128 + WTERMSIG(shell->last_exit_status);
	// 	else
	// 		shell->last_exit_status = 1;
	// }
	return (0);
}

static int	process_heredocs(t_command *cmd)
{
	t_redir	*temp;

	while (cmd)
	{
		temp = cmd->redirections;
		while (temp)
		{
			if (temp->type == REDIR_HEREDOC)
			{
				if (generate_heredoc_file(temp, cmd) != 0)
				{
					cmd->heredoc_filename = NULL;
					exit(1);
				}
			}
			temp = temp->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

static int	generate_heredoc_file(t_redir *redirection, t_command *cmd)
{
	int		fd;

	if (!cmd || !redirection)
		return (1);
	fd = open(cmd->heredoc_filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd == -1)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: heredoc: failed to create input file\n");
		return (-1);
	}
	if (get_user_input_to_heredoc(redirection, fd) != 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	get_user_input_to_heredoc(t_redir *redirection, int fd)
{
	char	*input;

	while (true)
	{
		input = readline("> ");
		if (*input == '\0')
		{
			free(input);
			input = ft_calloc(1, 2);
			if (!input)
				return (-1);
			input[0] = '\n';
		}
		if (ft_strncmp(input, redirection->target, ft_strlen(input)) == 0)
		{
			free(input);
			return (0);
		}
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
}
