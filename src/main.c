/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:53:52 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/15 16:06:08 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_str_array(char **str_array);

static volatile sig_atomic_t	signal_received = false;

void signal_handler(int signal)
{
	if (signal == SIGINT)
		signal_received = true;
}

int	main(int ac, char **av, char **env)
{
	char	*input;

	if (ac != 1)
		return (EXIT_FAILURE);
	signal(SIGINT, &signal_handler);
	while (true)
	{
		printf("signal_received: %d\n", signal_received);
		input = readline("Please input: \n");
		printf("signal_received: %d\n", signal_received);
		if (signal_received)
			return (EXIT_SUCCESS);
		if (!input)
			return (EXIT_FAILURE);
		printf("%s\n", input);
	}
	(void)av;
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
