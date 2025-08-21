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

int	print_working_directory(void)
{
	char	buf[1024];

	if (!getcwd(buf, sizeof(buf)))
		return (0);
	printf("%s\n", buf);
	return (1);
}
