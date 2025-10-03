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

char	*print_working_directory(t_shell *shell, bool print)
{
	char	*pwd;

	pwd = arena_alloc(shell->command_arena, PWD_BUFFER);
	if (!pwd)
		return (NULL);
	if (!getcwd(pwd, PWD_BUFFER))
	{
		perror(strerror(errno));
		shell->last_exit_status = 1;
		return (NULL);
	}
	shell->last_exit_status = 0;
	if (print)
		printf("%s\n", pwd);
	return (pwd);
}
