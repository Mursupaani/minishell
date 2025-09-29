/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive_shell.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 17:49:15 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/29 08:42:26 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_argv_to_single_input(char **argv, t_arena *arena);

int	non_interactve_shell(t_shell *shell, char **argv)
{
	char		*input;
	t_token		*tokens;
	t_command	*commands;

	input = join_argv_to_single_input(argv, shell->command_arena);
	if (!input)
		return (EXIT_FAILURE);
	tokens = tokenize(input, shell->command_arena);
	if (!tokens)
	{
		ft_fprintf(STDERR_FILENO, "Tokenization failed \n");
		return (EXIT_FAILURE);
	}
	commands = parse_pipeline(tokens, shell);
	if (!commands)
	{
		ft_fprintf(STDERR_FILENO, "minishell: syntax error\n");
		return (EXIT_FAILURE);
	}
	execute_commands(commands, shell);
	if (cleanup_after_execution(shell, commands))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static char	*join_argv_to_single_input(char **argv, t_arena *arena)
{
	char	*input;
	char	*temp;

	if (!argv || !arena)
		return (NULL);
	input = arena_strdup("", arena);
	while (++argv && *argv)
	{
		temp = ft_strjoin_arena(*argv, " ", arena);
		if (!temp)
			return (NULL);
		input = ft_strjoin_arena(input, temp, arena);
		if (!input)
			return (NULL);
	}
	return (input);
}
