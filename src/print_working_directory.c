/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_working_directory.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 15:59:58 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/29 16:08:25 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_working_directory(t_command *cmd)
{
	//FIXME: Change to dynamic memory?
	char	buf[2048];

	if (cmd->argv[1] != NULL)
	{
		ft_putstr_fd("Too many arguments\n", STDERR_FILENO);
		//FIXME: Error handling
	}
	if (!getcwd(buf, sizeof(buf)))
	{
		//FIXME: Error handling
		perror(strerror(errno));
	}
	printf("%s\n", buf);
}
