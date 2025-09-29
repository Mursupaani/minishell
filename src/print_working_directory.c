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

void	print_working_directory(t_shell *shell)
{
	char	buf[2048];

	if (!getcwd(buf, sizeof(buf)))
	{
		perror(strerror(errno));
		shell->last_exit_status = 1;
		return ;
	}
	shell->last_exit_status = 0;
	printf("%s\n", buf);
}
