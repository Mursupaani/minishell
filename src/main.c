/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:53:52 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/18 23:40:47 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

void sigint_handler(int sig)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void setup_signals()
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int argc, char **argv, char **env)
{
	t_shell *shell;
	char *input;

	(void)argv;
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (EXIT_FAILURE);
	if(argc != 1)
	{
		write(STDERR_FILENO, "Usage: ./minishell\n", 20);
		return (EXIT_FAILURE);
	}
	setup_signals();
	while (1)
	{
		g_signal_received = 0;
		input = readline("minishell$ ");
		if(!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		if(g_signal_received == SIGINT)
		{
			if(input)
				free(input);
			continue;
		}
		if(input[0] == '\0')
		{
			free(input);
			continue;
		}
		add_history(input);
		free(input);
	}
	return (EXIT_SUCCESS);
	print_str_array(env);
}

static void	print_str_array(char **str_array)
{
	int	i;

	if (!str_array)
		return ;
	i = 0;
	while (str_array[i])
		printf("%s\n", str_array[i++]);
}