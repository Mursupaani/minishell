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

int	non_interactve_shell(t_shell *shell, char **argv)
{
	char		*input;
	t_token		*tokens;
	t_command	*commands;

	if (count_args(argv) > 2 && strcmp(argv[1], "-c") == 0)
		input = join_argv_to_single_input(argv + 2, shell->command_arena);  // Script mode
	else
		input = read_line_from_stdin(shell->command_arena);  // Piped input mode
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
