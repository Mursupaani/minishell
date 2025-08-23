/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:53:52 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/22 16:06:27 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

int	main(int argc, char **argv, char **envp)
{
	// t_shell	*shell;
	char	*input;

	(void)argv;
	// shell = malloc(sizeof(t_shell));
	// if (!shell)
	// 	return (EXIT_FAILURE);
	if (argc != 1)
	{
		write(STDERR_FILENO, "Usage: ./minishell\n", 20);
		return (EXIT_FAILURE);
	}
	setup_signals();
	if (isatty(STDIN_FILENO))
		interactive_shell(argc, argv, envp);
	else
		non_interactve_shell(argc, argv, envp);
	// TODO: Initialize shell once before main loop, not inside it
	// shell = shell_init(envp);
	
	while (1)
	{
		g_signal_received = 0;
		input = readline("minishell$ ");
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (g_signal_received == SIGINT)
		{
			if (input)
				free(input);
			continue ;
		}
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		
		// TODO: Connect parsing and execution
		// t_command *cmd = parse_args(input, envp);
		// if (cmd)
		//     execute_command(*cmd, envp);
		
		free(input);
	}
	// TODO: Add proper cleanup - free shell and its allocated memory
	// cleanup_shell_partial(shell, 3);
	// print_str_array(envp);
	return (EXIT_SUCCESS);
}
