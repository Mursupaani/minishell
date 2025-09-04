/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:53:52 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/28 19:51:41 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

static void print_tokens(t_token *tokens)
{
    t_token *current = tokens;
    
    while (current)
    {
        printf("Token: type=%d, value='%s', quoted=%d, expandable=%d\n",
               current->type, current->value, current->quoted, current->expandable);
        current = current->next;
    }
    printf("--- End of tokens ---\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*input;
    t_token *tokens;

	(void)argv;
	if (argc != 1)
	{
		write(STDERR_FILENO, "Usage: ./minishell\n", 20);
		return (EXIT_FAILURE);
	}
	// Initialize shell first
	shell = shell_init(envp);
	if (!shell)
	{
		write(STDERR_FILENO, "Failed to initialize shell\n", 28);
		return (EXIT_FAILURE);
	}
	setup_signals();
	// if (isatty(STDIN_FILENO))
	// 	interactive_shell(argc, argv, envp);
	// else
	// 	non_interactve_shell(argc, argv, envp);
	while (1)
	{
		g_signal_received = 0;
		input = readline("minishell$ ");
        if (g_signal_received == SIGINT)
        {
            if (input)
                free(input);
            continue ;
        }
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			free(shell);
			exit(0);
		}
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		tokens = tokenize(input, shell->command_arena);
        if (tokens)
        {
            printf("Tokenized input:\n");
            print_tokens(tokens);
        }
        else
        {
            printf("Tokenization failed\n");
        }
        
        arena_reset(shell->command_arena);
        free(input);
    }
    //shell_free(shell);
    return (EXIT_SUCCESS);
}
