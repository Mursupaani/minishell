/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_working_directory.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 15:59:58 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/20 12:17:07 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_working_directory(t_command *cmd)
{
	char	buf[1024];

	if (cmd->argv[1] != NULL)
	{
		ft_putstr_fd("Too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (!getcwd(buf, sizeof(buf)))
	{
		perror(strerror(errno));
	}
	printf("%s\n", buf);
	return (0);
}
