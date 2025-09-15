/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_one.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 15:52:39 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/08 11:02:50 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	create_fork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		// Check this
		perror(strerror(errno));
		exit(1);
	}
	return (pid);
}
