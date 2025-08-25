/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_shell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 17:40:17 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/23 20:47:39 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_shell	*initialize_shell(void);
static void	free_memory(t_shell *shell);
static int	choose_command_execution(t_shell *shell, t_command *cmd);

int	interactive_shell(int argc, char **argv, char **envp)
{
	t_shell		*shell;
	t_command	*cmd;

	shell = initialize_shell();
	if (!shell)
		// What should we return to main? Just exit?
		return (EXIT_FAILURE);
	// WARN: Do we need these anywhere?
	(void)argc;
	(void)argv;
	while (1)
	{
		g_signal_received = 0;
		if (!shell->command_arena)
			shell->command_arena = arena_init(COMMAND_ARENA_SIZE);
		shell->input = readline("minishell$ ");
		if(!shell->input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		if(g_signal_received == SIGINT)
		{
			if(shell->input)
				free(shell->input);
			continue;
		}
		if(shell->input[0] == '\0')
		{
			free(shell->input);
			continue;
		}
		cmd = parse_args(shell->input, envp, shell->command_arena);
		choose_command_execution(shell, cmd);
		// printf("Exit status: %d\n", shell->last_exit_status);
		add_history(shell->input);
		arena_free(&shell->command_arena);
		free(shell->input);
	}
	free_memory(shell);
	return (EXIT_SUCCESS);
}

static t_shell	*initialize_shell(void)
{
	t_shell		*shell;

	shell = ft_calloc(1, sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->session_arena = arena_init(SESSION_ARENA_SIZE);
	shell->command_arena = arena_init(COMMAND_ARENA_SIZE);
	if (!shell->session_arena || !shell->command_arena)
	{
		if (shell->session_arena)
			free(shell->session_arena);
		if (shell->command_arena)
			free(shell->command_arena);
		free(shell);
		return (NULL);
	}
	return (shell);
}

static void	free_memory(t_shell *shell)
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

static int	choose_command_execution(t_shell *shell, t_command *cmd)
{
	if (ft_strncmp(cmd->argv[0], "cd", ft_strlen(cmd->argv[0])) == 0)
		shell->last_exit_status = change_directory(cmd);
	else if (ft_strncmp(cmd->argv[0], "pwd", ft_strlen(cmd->argv[0])) == 0)
		shell->last_exit_status = print_working_directory(cmd);
	else if (ft_strncmp(cmd->argv[0], "echo", ft_strlen(cmd->argv[0])) == 0)
		shell->last_exit_status = ft_echo(cmd);
	else
		shell->last_exit_status = execute_command(cmd);
	return (shell->last_exit_status);
}
