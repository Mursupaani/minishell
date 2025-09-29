/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:53:52 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/29 08:40:54 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

void	free_memory_at_exit(t_shell *shell);
int		cleanup_after_execution(t_shell *shell, t_command *cmd);

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argc;
	shell = shell_init(envp);
	if (!shell)
	{
		write(STDERR_FILENO, "Failed to initialize shell\n", 28);
		return (EXIT_FAILURE);
	}
	setup_signals();
	// if (isatty(STDIN_FILENO))
	if (argc == 1)
		interactive_shell(shell);
	else
		non_interactve_shell(shell, argv);
	free_memory_at_exit(shell);
    return (EXIT_SUCCESS);
}

void	free_memory_at_exit(t_shell *shell)
{
	rl_clear_history();
	if (!shell)
		return ;
	if (shell->input)
		free(shell->input);
	if (shell->command_arena)
		arena_free(&shell->command_arena);
	if (shell->session_arena)
		arena_free(&shell->session_arena);
	free(shell);
}

int	cleanup_after_execution(t_shell *shell, t_command *cmd)
{
	while(cmd)
	{
		if (cmd->heredoc_filename != NULL)
		{
			if(unlink(cmd->heredoc_filename))
			{
				perror(strerror(errno));
				return (1);
			}
		}
		cmd = cmd->next;
	}
	if (shell->input)
		free(shell->input);
	arena_reset(shell->command_arena);
	return (0);
}
