/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_shell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:47:09 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/11 13:16:45 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_empty_input(char *input)
{
	int	i;

	if (!input || input[0] == '\0')
		return (1);
	i = 0;
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	if (input[i] == '\0')
		return (1);
	return (0);
}

static int	handle_input_validation(t_shell *shell)
{
	if (!shell->input)
		exit_builtin(NULL, shell);
	if (is_empty_input(shell->input))
	{
		free(shell->input);
		return (1);
	}
	return (0);
}

static int	process_input(t_shell *shell)
{
	t_token		*tokens;
	t_command	*commands;

	add_history(shell->input);
	tokens = tokenize(shell->input, shell->command_arena);
	if (!tokens)
	{
		ft_fprintf(STDERR_FILENO, "minishell: syntax error\n");
		shell->last_exit_status = 2;
		return (0);
	}
	commands = parse_pipeline(tokens, shell);
	if (!commands)
	{
		ft_fprintf(STDERR_FILENO, "minishell: syntax error\n");
		shell->last_exit_status = 2;
		free(shell->input);
		arena_reset(shell->command_arena);
		return (0);
	}
	execute_commands(commands, shell);
	if (cleanup_after_execution(shell, commands))
		return (EXIT_FAILURE);
	return (0);
}

int	interactive_shell(t_shell *shell)
{
	int	result;

	while (1)
	{
		setup_parent_signals();
		if (g_signal_received == SIGINT)
			shell->last_exit_status = 127;
		g_signal_received = 0;
		update_prompt(shell);
		shell->input = readline(shell->prompt);
		if (handle_input_validation(shell))
			continue ;
		result = process_input(shell);
		if (result == EXIT_FAILURE)
			return (shell->last_exit_status);
	}
	return (result);
}
