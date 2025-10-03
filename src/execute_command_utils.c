/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:04:52 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/01 14:35:36 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_non_empty_argument(t_command *cmd)
{
	if (!cmd || !cmd->argv)
		exit(EXIT_FAILURE);
	while (*cmd->argv)
	{
		if (*cmd->argv[0] != '\0')
			break ;
		cmd->argv++;
	}
	if (!*cmd->argv)
		exit(EXIT_SUCCESS);
}
