/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:40:56 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/29 16:15:41 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	choose_execution_type(t_command *cmd, t_shell *shell)
{
	//FIXME: CD must be called by parent
	if (create_fork() == 0)
	{
		if (cmd->cmd_type == CMD_BUILTIN)
			execute_builtin_command(cmd);
		else if (cmd->cmd_type == CMD_EXEC)
			execute_command(cmd);
		else if (cmd->cmd_type == CMD_REDIR)
			execute_redirection(cmd, shell);
		else if (cmd->cmd_type == CMD_PIPE)
			execute_pipe(cmd, shell);
		ft_putstr_fd("Testing\n", 2);
	}
	// if (waitpid(cmd->pid, WEXITSTATUS(cmd->status), 0) == -1)
	if (waitpid(cmd->pid, &cmd->status, 0) == -1)
		perror(strerror(errno));
	shell->last_exit_status = cmd->status;
}

void	execute_builtin_command(t_command *cmd)
{
	if (ft_strncmp(cmd->argv[0], "cd", ft_strlen(cmd->argv[0])) == 0)
		change_directory(cmd);
	else if (ft_strncmp(cmd->argv[0], "pwd", ft_strlen(cmd->argv[0])) == 0)
		print_working_directory(cmd);
	else if (ft_strncmp(cmd->argv[0], "echo", ft_strlen(cmd->argv[0])) == 0)
		ft_echo(cmd);
}

void	execute_command(t_command *cmd)
{
	if (execve(cmd->argv[0], cmd->argv, cmd->envp))
		perror(strerror(errno));
}

void	execute_redirection(t_command *cmd, t_shell *shell)
{
	// FIXME: Need to define the fd to close
	close(cmd->redirections->fd);
	// FIXME: Need to have specific redir types
	if (open(cmd->redirections->target, cmd->redirections->type) < 0)
	{
		perror(strerror(errno));
		exit(1);
	}
	choose_execution_type(cmd->redirections->cmd_to_redir, shell);
}
