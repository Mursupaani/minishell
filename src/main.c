/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:53:52 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/22 13:06:10 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

static void	free_memory_at_exit(t_shell *shell);
static int	cleanup_after_execution(t_shell *shell, t_command *cmd);

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

static void print_commands(t_command *commands)
{
    t_command *cmd = commands;
    int cmd_num = 1;
    
    while (cmd)
    {
        printf("Command %d:\n", cmd_num++);
        
        // Print argv
        if (cmd->argv)
        {
            printf("  cmd_type: %d\n", cmd->cmd_type);
            printf("  argv: ");
            for (int i = 0; cmd->argv[i]; i++)
                printf("'%s' ", cmd->argv[i]);
            printf("\n");
        }
        
        // Print redirections
        t_redir *redir = cmd->redirections;
        while (redir)
        {
            printf("  redirection: type=%d target='%s'\n", 
                   redir->type, redir->target);
            redir = redir->next;
        }
		if (cmd->heredoc_filename)
            printf("  cmd heredoc filename: '%s'\n", cmd->heredoc_filename); 
		printf("command type: %d\n", cmd->cmd_type);
        
        cmd = cmd->next;
        if (cmd)
            printf("  | (pipe to next command)\n");
    }
    printf("--- End of command chain ---\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
    t_token *tokens;
    t_command *commands;

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
		shell->input = readline("minishell$ ");
        if (g_signal_received == SIGINT)
        {
            if (shell->input)
                free(shell->input);
            continue ;
        }
		if (!shell->input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (shell->input[0] == '\0')
		{
			free(shell->input);
			continue ;
		}
		add_history(shell->input);
		
		// Tokenize
		tokens = tokenize(shell->input, shell->command_arena);
        if (!tokens)
        {
            // printf("Tokenization failed\n");
            arena_reset(shell->command_arena);
            free(shell->input);
            continue;
        }

        // write(STDOUT_FILENO, "Tokenized input:\n", 18);
        // print_tokens(tokens);
        
        // Parse tokens into commands
        commands = parse_pipeline(tokens, shell);
        if (!commands)
        {
			print_commands(commands);
			print_tokens(tokens);
            write(STDERR_FILENO, "minishell: syntax error\n", 24);
            arena_reset(shell->command_arena);
            free(shell->input);
            continue;
        }
		//FIXME: Need to fix this in parsing!
		shell->env_array = envp;
		// commands->redirections->heredoc_delimiter = "eof";
		// commands->redirections->next->heredoc_delimiter = "eof";
		print_commands(commands);
		execute_commands(commands, shell);
		//FIXME: Handle clean exit
		if (cleanup_after_execution(shell, commands))
			exit(EXIT_FAILURE);
        
    }
	free_memory_at_exit(shell);
    // shell_free(shell);
    return (EXIT_SUCCESS);
}

static void	free_memory_at_exit(t_shell *shell)
{
	rl_clear_history();
	if (!shell)
		return ;
	if (shell->input)
		free(shell->input);
	if (shell->command_arena)
		arena_free(&shell->command_arena);
	if (shell->session_arena)
		arena_free(&shell->session_arena);
	free(shell);
}

static int	cleanup_after_execution(t_shell *shell, t_command *cmd)
{
	while(cmd)
	{
		if (cmd->heredoc_filename != NULL)
		{
			if(unlink(cmd->heredoc_filename))
			{
				perror(strerror(errno));
				return (1);
			}
		}
		cmd = cmd->next;
	}
	arena_reset(shell->command_arena);
	free(shell->input);
	return (0);
}
