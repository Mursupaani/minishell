/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive_shell.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:02:31 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/17 10:30:27 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static char	*join_argv_to_single_input(char **argv, t_arena *arena);
static int	count_args(char **argv);
static char	*read_line_from_stdin(t_arena *arena);

static int	process_input_line(char *input, t_shell *shell)
{
	t_token		*tokens;
	t_command	*commands;

	if (!input || input[0] == '\0' || is_whitespace_only(input))
		return (EXIT_SUCCESS);
	tokens = tokenize(input, shell->command_arena);
	if (!tokens)
	{
		ft_fprintf(STDERR_FILENO, "minishell: syntax error\n");
		shell->last_exit_status = 2;
		return (shell->last_exit_status);
	}
	commands = parse_pipeline(tokens, shell);
	if (!commands)
	{
		ft_fprintf(STDERR_FILENO, "minishell: syntax error\n");
		shell->last_exit_status = 2;
		arena_reset(shell->command_arena);
		return (shell->last_exit_status);
	}
	execute_commands(commands, shell);
	if (cleanup_after_execution(shell, commands))
		exit_and_free_memory(EXIT_FAILURE, shell, commands);
	return (shell->last_exit_status);
}

int	non_interactve_shell(t_shell *shell, char **argv)
{
	char	*input;
	int		status;

	if (count_args(argv) > 2 && strcmp(argv[1], "-c") == 0)
	{
		input = join_argv_to_single_input(argv + 2, shell->command_arena);
		return (process_input_line(input, shell));
	}
	else
	{
		status = EXIT_SUCCESS;
		input = read_line_from_stdin(shell->command_arena);
		while (input)
		{
			status = process_input_line(input, shell);
			arena_reset(shell->command_arena);
			input = read_line_from_stdin(shell->command_arena);
		}
		return (status);
	}
}

static int	count_args(char **argv)
{
	int	count;

	count = 0;
	while (argv[count])
		count++;
	return (count);
}

static char	*read_line_from_stdin(t_arena *arena)
{
	char	*line;
	char	*arena_line;
	size_t	len;

	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	arena_line = arena_strdup(line, arena);
	free(line);
	return (arena_line);
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
