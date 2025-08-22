/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:53:52 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/22 10:55:10 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

void	sigint_handler(int sig)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals()
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int argc, char **argv, char **envp)
{
	// No need for this because of non-interactive shell?
	// if(argc != 1)
	// {
	// 	write(STDERR_FILENO, "Usage: ./minishell\n", 20);
	// 	return (EXIT_FAILURE);
	// }
	setup_signals();
	if (isatty(STDIN_FILENO))
		interactive_shell(argc, argv, envp);
	else
		non_interactve_shell(argc, argv, envp);
}

void	print_str_array(char **str_array)
{
	int	i;

	if (!str_array)
		return ;
	i = 0;
	while (str_array[i])
		printf("%s\n", str_array[i++]);
}
