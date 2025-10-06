/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_shell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/10/06 15:18:18 by magebreh         ###   ########.fr       */
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
	if (g_signal_received == SIGINT)
	{
		free(shell->input);
		shell->last_exit_status = 130;
		return (1);
	}
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
		ft_fprintf(STDERR_FILENO, "Tokenization failed \n");
		free(shell->input);
		return (1);
	}
	commands = parse_pipeline(tokens, shell);
	if (!commands)
	{
		ft_fprintf(STDERR_FILENO, "minishell: syntax error\n");
		shell->last_exit_status = 2;
		arena_reset(shell->command_arena);
		free(shell->input);
		return (1);
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
		g_signal_received = 0;
		shell->input = readline("minishell$ ");
		if (handle_input_validation(shell))
			continue ;
		result = process_input(shell);
		if (result == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
