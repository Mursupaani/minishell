/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_shell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:47:09 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/06 15:47:11 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	update_prompt(t_shell *shell, t_arena *arena);

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
		update_prompt(shell, shell->command_arena);
		shell->input = readline(shell->prompt);
		if (handle_input_validation(shell))
			continue ;
		result = process_input(shell);
		if (result == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static void	update_prompt(t_shell *shell, t_arena *arena)
{
	char	*cwd;
	char	*home;
	int		home_len;

	cwd = arena_alloc(shell->command_arena, PWD_BUFFER);
	if (!cwd)
		error_exit_and_free_memory(shell);
	if (!getcwd(cwd, PWD_BUFFER))
		cwd = arena_strdup(hash_table_get(shell->env_table, "PWD"), arena);
	if (!cwd)
	{
		shell->prompt = "minishell\n% ";
		return ;
	}
	home = hash_table_get(shell->env_table, "HOME");
	if (home && home[0] != '\0')
	{
		home_len = ft_strlen(home);
		if (ft_strnstr(cwd, home, home_len) == cwd)
		{
			cwd[home_len - 1] = '~';
			cwd += home_len - 1;
		}
	}
	shell->prompt = ft_strjoin_arena(cwd, "\n% ", shell->command_arena);
}
