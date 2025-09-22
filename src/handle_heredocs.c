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

static int	generate_heredoc_file(t_redir *redirection);

int handle_heredocs(t_command *cmd)
{
	t_redir	*temp;

	if (!cmd)
		return (1);
	while (cmd)
	{
		temp = cmd->redirections;
		while (temp)
		{
			if (temp->type == REDIR_HEREDOC)
			{
				if (generate_heredoc_file(temp) != 0)
					// FIXME: Unlink all possible heredocs
					return (1);
			}
			temp = temp->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

static int	generate_heredoc_file(t_redir *redirection)
{
	int		fd;
	char	*input;

	fd = open(redirection->target, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd == -1)
		return (1);
	while (true)
	{
		input = readline("> ");
		if (ft_strncmp(input, redirection->heredoc_delimiter, ft_strlen(input)) == 0)
		{
			free(input);
			break ;
		}
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	close(fd);
	return (0);
}
