/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_prompt.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 22:09:01 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/06 22:12:10 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	update_prompt_colors(char *cwd, t_shell *shell);
static void	write_pwd_to_cwd(char *cwd, t_shell *shell);

void	update_prompt(t_shell *shell)
{
	char	cwd[PWD_BUFFER];
	char	*cwd_ptr;
	char	*home;
	int		home_len;

	if (!getcwd(cwd, PWD_BUFFER))
		write_pwd_to_cwd(cwd, shell);
	if (cwd[0] == '\0')
	{
		update_prompt_colors("minishell (path unavailable)\n% ", shell);
		return ;
	}
	cwd_ptr = &cwd[0];
	home = hash_table_get(shell->env_table, "HOME");
	if (home && home[0] != '\0')
	{
		home_len = ft_strlen(home);
		if (ft_strnstr(cwd, home, home_len) == cwd)
		{
			cwd[home_len - 1] = '~';
			cwd_ptr = &cwd[home_len - 1];
		}
	}
	update_prompt_colors(cwd_ptr, shell);
}

static void	update_prompt_colors(char *cwd, t_shell *shell)
{
	char	*temp;

	if (!cwd || !shell)
		return ;
	temp = ft_strjoin("\033[1;30;47m", cwd);
	if (!temp)
		error_exit_and_free_memory(shell);
	shell->prompt = ft_strjoin_arena(
			temp, "\033[0m\n % ", shell->command_arena);
	free(temp);
	if (!shell->prompt)
		error_exit_and_free_memory(shell);
}

static void	write_pwd_to_cwd(char *cwd, t_shell *shell)
{
	char	*pwd;
	int		i;

	pwd = hash_table_get(shell->env_table, "PWD");
	if (!pwd)
	{
		cwd[0] = '\0';
		return ;
	}
	i = 0;
	while (pwd[i] && i < PWD_BUFFER - 1)
	{
		cwd[i] = pwd[i];
		i++;
	}
	cwd[i] = '\0';
}
