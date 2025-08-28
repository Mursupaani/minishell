/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:53:52 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/27 19:49:39 by magebreh         ###   ########.fr       */
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

static void test_tokenizer(void)
{
    t_arena *test_arena;
    t_token *tokens;
    
    test_arena = arena_init(1024);
    if (!test_arena)
    {
        printf("Failed to initialize test arena\n");
        return;
    }
    
    printf("=== TOKENIZER TESTING ===\n\n");
    
    // Test case 1: Simple command
    printf("Test 1: \"echo \"$USER\"\"\n");
    tokens = tokenize("echo \"$USER\"", test_arena);
    print_tokens(tokens);
    arena_reset(test_arena);
    
    // Test case 2: Pipeline
    printf("Test 2: \"ls | grep test\"\n");
    tokens = tokenize("ls | grep test", test_arena);
    print_tokens(tokens);
    arena_reset(test_arena);
    
    // Test case 3: Double quoted string
    printf("Test 3: \"echo \\\"quoted\\\"\"\n");
    tokens = tokenize("echo \"quoted\"", test_arena);
    print_tokens(tokens);
    arena_reset(test_arena);
    
    // Test case 4: Variable expansion
    printf("Test 4: \"echo $USER\"\n");
    tokens = tokenize("echo $USER", test_arena);
    print_tokens(tokens);
    arena_reset(test_arena);
    
    // Test case 5: Single quoted string
    printf("Test 5: \"echo 'single'\"\n");
    tokens = tokenize("echo 'single'", test_arena);
    print_tokens(tokens);
    arena_reset(test_arena);
    
    // Test case 6: Redirections
    printf("Test 6: \"cat < input.txt > output.txt\"\n");
    tokens = tokenize("cat < input.txt > output.txt", test_arena);
    print_tokens(tokens);
    arena_reset(test_arena);
    
    // Test case 7: Heredoc
    printf("Test 7: \"cat << EOF\"\n");
    tokens = tokenize("cat << EOF", test_arena);
    print_tokens(tokens);
    arena_reset(test_arena);
    
    // Test case 8: Append redirection
    printf("Test 8: \"echo hello >> file.txt\"\n");
    tokens = tokenize("echo hello >> file.txt", test_arena);
    print_tokens(tokens);
    arena_reset(test_arena);
    
    arena_free(&test_arena);
    printf("=== TOKENIZER TESTING COMPLETE ===\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*input;

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
	
	// Run tokenizer tests
	test_tokenizer();
	
	setup_signals();
	if (isatty(STDIN_FILENO))
		interactive_shell(argc, argv, envp);
	else
		non_interactve_shell(argc, argv, envp);
	
	while (1)
	{
		g_signal_received = 0;
		input = readline("minishell$ ");
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			free(shell);
			exit(0);
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
	return (EXIT_SUCCESS);
}
