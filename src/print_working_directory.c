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

void	print_working_directory(t_command *cmd, t_shell *shell)
{
	char	buf[2048];

	if (cmd->argv[1] != NULL)
	{
		ft_fprintf(STDERR_FILENO, "pwd: too many arguments\n");
		shell->last_exit_status = 1;
		return ;
	}
	if (!getcwd(buf, sizeof(buf)))
	{
		perror(strerror(errno));
		shell->last_exit_status = 1;
		return ;
	}
	printf("%s\n", buf);
}
