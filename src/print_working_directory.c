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

void	print_working_directory(t_command *cmd, t_shell *shell)
{
	char	buf[1024];

	if (cmd->argv[1] != NULL)
	{
		ft_putstr_fd("Too many arguments\n", STDERR_FILENO);
		shell->last_exit_status = 1;
	}
	if (!getcwd(buf, sizeof(buf)))
	{
		shell->last_exit_status = 1;
		perror(strerror(errno));
		exit(1);
	}
	printf("%s\n", buf);
	shell->last_exit_status = 1;
}
