/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_shell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 17:40:17 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/21 17:58:11 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	interactive_shell(int argc, char **argv, char **envp)
{
	t_shell *shell;
	char *input;

	// WARN: Do we need these anywhere?
	(void)argc;
	(void)argv;
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (EXIT_FAILURE);
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
		execute_args(&input, envp);
		// change_directory(input);
		// print_working_directory();
		add_history(input);
		free(input);
	}
	return (EXIT_SUCCESS);
}
