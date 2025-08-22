/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_shell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 17:40:17 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/22 11:44:26 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

int	interactive_shell(int argc, char **argv, char **envp)
{
	t_shell		*shell;
	char		*input;
	t_command	*cmd;
	int		status;

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
		cmd = (t_command *)malloc(sizeof(t_command));
		cmd = parse_args(input, envp);
		status = execute_command(*cmd, envp);
		printf("Exit status: %d\n", status);
		// change_directory(input);
		// print_working_directory();
		add_history(input);
		free(input);
	}
	return (EXIT_SUCCESS);
}
