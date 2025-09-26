/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:52:50 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/26 16:56:23 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_exit_and_free_memory(t_shell *shell)
{
	if (!shell)
		exit(EXIT_FAILURE);
	free_memory_at_exit(shell);
	exit(EXIT_FAILURE);
}
