/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredocs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 11:40:45 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/03 11:54:38 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	generate_heredoc_file(t_redir *redirection, t_command *cmd, t_shell *shell);
static int	get_user_input_to_heredoc(t_redir *redirection, int fd, t_shell *shell);
static int	process_heredocs(t_command *cmd, t_shell *shell);

int	handle_heredocs(t_command *cmd, t_shell *shell)
{
	//FIXME: How to reproduce "bash: warning: here-document at line 1 delimited by end-of-file (wanted `eof')"
	// with ctrl-d?
	if (!cmd)
		return (1);
	// shell->child_pid = create_fork(shell);
	// if (shell->child_pid == 0)
	process_heredocs(cmd, shell);
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

static int	process_heredocs(t_command *cmd, t_shell *shell)
{
	t_redir	*temp;

	while (cmd)
	{
		temp = cmd->redirections;
		while (temp)
		{
			if (temp->type == REDIR_HEREDOC)
			{
				if (generate_heredoc_file(temp, cmd, shell) != 0)
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

static int	generate_heredoc_file(t_redir *redirection, t_command *cmd, t_shell *shell)
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
(wanted `%s')\n",redirection->target);
	return (0);
}

static int	get_user_input_to_heredoc(t_redir *redirection, int fd, t_shell *shell)
{
	char	*input;

	while (true)
	{
		input = readline("> ");
		if (!input)
			return (1);
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
		input = expand_variables_from_input(input, shell);
		if (!input)
			return (1);
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
}

char	*expand_variables_from_input(char *input, t_shell *shell)
{
	int		i;
	char	**inputv;
	char	*expanded;
	char	*temp;

	if (!input || !shell)
		return (NULL);
	inputv = split_input(input, shell);
	if (!inputv)
		return (NULL);
	free(input);
	i = 0;
	temp = NULL;
	expanded = ft_strdup("");
	while (inputv[i])
	{
		if (inputv[i][0] == '$' && inputv[i][1] != '\0')
		{
			temp = hash_table_get(shell->env_table, &inputv[i][1]);
			if (temp)
				temp = ft_strjoin(expanded, temp);
			else
				temp = ft_strjoin(expanded, "");
		}
		else
			temp = ft_strjoin(expanded, inputv[i]);
		free(expanded);
		expanded = ft_strdup(temp);
		free(temp);
		i++;
	}
	return (expanded);
}
