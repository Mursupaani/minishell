/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 15:59:58 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/15 16:06:55 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** =============================================================================
** PWD BUILT-IN COMMAND IMPLEMENTATION
** =============================================================================
**
** This file implements the pwd (print working directory) built-in command,
** which is one of the simpler built-ins that can run in either parent or
** child processes.
**
** The pwd command prints the current working directory to stdout.
** It uses getcwd() system call to retrieve the current directory path.
*/

#include "minishell.h"

/*
** pwd: Print working directory built-in command
**
** Prints the absolute path of the current working directory to stdout.
** This implementation follows the POSIX standard for pwd behavior.
**
** Behavior:
** - Retrieves current directory using getcwd()
** - Prints full absolute path followed by newline
** - Returns success/failure status
**
** Error handling:
** - If getcwd() fails, returns 0 (failure)
** - Possible failures: permission denied, directory deleted, path too long
**
** TODO: Add proper error reporting to stderr
** TODO: Handle the -L and -P options (logical vs physical path)
** TODO: Integrate with shell's exit status system
**
** @return: 1 on success, 0 on failure
*/

int	pwd(void)
{
	char	buf[1024];  // Buffer for directory path (PATH_MAX would be better)

	// Get current working directory
	if (!getcwd(buf, sizeof(buf)))
		return (0);  // getcwd() failed
		
	// Print directory path with newline
	printf("%s\n", buf);
	return (1);  // Success
}
