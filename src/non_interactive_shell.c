/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive_shell.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 17:49:15 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/30 17:25:49 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_argv_to_single_input(char **argv, t_arena *arena);
static int count_args(char **argv);
static char *read_line_from_stdin(t_arena *arena);

static int is_whitespace_only(char *str)
{
	int i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	return (str[i] == '\0');
}

static int process_input_line(char *input, t_shell *shell)
{
	t_token		*tokens;
	t_command	*commands;

	if (!input || input[0] == '\0' || is_whitespace_only(input))
		return (EXIT_SUCCESS);
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
		arena_reset(shell->command_arena);
		return (EXIT_FAILURE);
	}
	execute_commands(commands, shell);
	if (cleanup_after_execution(shell, commands))
		return (EXIT_FAILURE);
	return (shell->last_exit_status);
}

int	non_interactve_shell(t_shell *shell, char **argv)
{
	char		*input;
	int			status;

	if (count_args(argv) > 2 && strcmp(argv[1], "-c") == 0)
	{
		// Script mode: execute single command from args
		input = join_argv_to_single_input(argv + 2, shell->command_arena);
		return (process_input_line(input, shell));
	}
	else
	{
		// Piped input mode: read and execute all lines from stdin
		status = EXIT_SUCCESS;
		while ((input = read_line_from_stdin(shell->command_arena)) != NULL)
		{
			status = process_input_line(input, shell);
			arena_reset(shell->command_arena);
		}
		return (status);
	}
}

static int count_args(char **argv)
{
    int count = 0;
    while (argv[count])
        count++;
    return (count);
}

static char *read_line_from_stdin(t_arena *arena)
{
    char *line = get_next_line(STDIN_FILENO);
    if (!line)
        return (NULL);
    
    // Remove trailing newline if present
    size_t len = ft_strlen(line);
    if (len > 0 && line[len-1] == '\n')
        line[len-1] = '\0';
    
    // Copy to arena memory
    char *arena_line = arena_strdup(line, arena);
    free(line);  // get_next_line allocates with malloc
    
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
