/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:53:52 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/30 17:08:30 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

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
	if (isatty(STDIN_FILENO))
		exit_code = interactive_shell(shell);
	else
		exit_code = non_interactve_shell(shell, argv);
	free_memory_at_exit(shell);
    return (exit_code);
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
