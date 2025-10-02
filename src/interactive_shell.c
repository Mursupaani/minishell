/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_shell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 17:40:17 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/02 12:48:22 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	interactive_shell(t_shell *shell)
{
	t_token *tokens;
	t_command *commands;

	while (1)
	{
		g_signal_received = 0;
		shell->input = readline("minishell$ ");
		if (g_signal_received == SIGINT)
		{
			if (shell->input)
				free(shell->input);
			continue ;
		}
		if (!shell->input)
			exit_builtin(NULL, shell);
		if (shell->input[0] == '\0')
		{
			free(shell->input);
			continue ;
		}
		// Check if input contains only whitespace
		int i = 0;
		while (shell->input[i] && (shell->input[i] == ' ' || shell->input[i] == '\t'))
			i++;
		if (shell->input[i] == '\0')
		{
			free(shell->input);
			continue ;
		}
		add_history(shell->input);
		tokens = tokenize(shell->input, shell->command_arena);
		if (!tokens)
		{
			ft_fprintf(STDERR_FILENO, "Tokenization failed \n");
			free(shell->input);
			continue;
		}
		commands = parse_pipeline(tokens, shell);
		if (!commands)
		{
			ft_fprintf(STDERR_FILENO, "minishell: syntax error\n");
			arena_reset(shell->command_arena);
			free(shell->input);
			continue;
		}
		// print_commands(commands);
		execute_commands(commands, shell);
		if (cleanup_after_execution(shell, commands))
			return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
