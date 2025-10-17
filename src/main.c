/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 09:56:58 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/17 09:57:00 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	free_memory_at_exit(t_shell *shell);
int		cleanup_after_execution(t_shell *shell, t_command *cmd);

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		exit_code;

	(void)argc;
	shell = shell_init(envp);
	if (!shell)
	{
		write(STDERR_FILENO, "Failed to initialize shell\n", 28);
		return (EXIT_FAILURE);
	}
	update_shell_lvl(shell);
	shell->is_a_tty = isatty(STDIN_FILENO);
	if (shell->is_a_tty)
		exit_code = interactive_shell(shell);
	else
		exit_code = non_interactve_shell(shell, argv);
	free_memory_at_exit(shell);
	return (exit_code);
}

int	cleanup_after_execution(t_shell *shell, t_command *cmd)
{
	while (cmd)
	{
		if (cmd->heredoc_filename != NULL)
		{
			if (unlink(cmd->heredoc_filename))
				perror(strerror(errno));
		}
		cmd = cmd->next;
	}
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	arena_reset(shell->command_arena);
	return (0);
}
