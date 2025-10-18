/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 15:52:39 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/26 17:00:59 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_fork(t_shell *shell)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: creat_fork: Fork failed\n");
		exit_and_free_memory(EXIT_FAILURE, shell, NULL);
	}
	return (pid);
}
