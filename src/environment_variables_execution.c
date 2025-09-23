/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_variables_execution.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 13:58:42 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/12 16:34:32 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_environment_variables(t_shell *shell)
{
	if (!shell)
		return ;
	if (!shell->env_array)
	{
		//FIXME: Error handling
		printf("Fail\n");
		shell->last_exit_status = 1;
		return ;
	}
	while (shell->env_array)
		printf("%s\n", *shell->env_array);
	shell->last_exit_status = 0;
}

void	export_environment_variable(t_command *cmd, t_shell *shell)
{
	if (!shell)
		return ;
	if (!cmd)
	{
		shell->last_exit_status = 1;
		return ;
	}

}

void	prepare_cmd(t_command *cmd, t_shell *shell)
{
	t_command *current = cmd;
	while (current)
	{
		expand_cmd(current, shell);
		current = current->next;
	}
}

void	expand_cmd(t_command *cmd, t_shell *shell)
{
	int i;
	char *expanded;

	i = 0;
	if(!cmd->argv)
		return ;
	while (cmd->argv[i])
	{
		if(ft_strchr(cmd->argv[i], '$'))
		{
			expanded = expand_var(cmd->argv[i], shell, shell->command_arena);
			if(expanded)
				cmd->argv[i] = expanded;
		}
		i++;
	}
}

char	*expand_var(char *str, t_shell *shell, t_arena *arena)
{
	char *res;

	if(!str || !ft_strchr(str, '$'))
		return(arena_strdup(str, arena));
	res = process_var_expand(str, shell, arena);
	return (res);
}

static char	*expand_single_variable(char **src, t_shell *shell, char *dst)
{
	char *var_start;
	char *value;
	size_t var_len;
	char var_name[256];

	(*src)++;
	if (**src == '?')
	{
		dst += sprintf(dst, "%d", shell->last_exit_status);
		(*src)++;
	}
	else
	{
		var_start = *src;
		while (**src && (ft_isalnum(**src) || **src == '_'))
			(*src)++;
		var_len = *src - var_start;
		strncpy(var_name, var_start, var_len);
		var_name[var_len] = '\0';

		value = hash_table_get(shell->env_table, var_name);
		if (value)
			dst += sprintf(dst, "%s", value);
	}
	return (dst);
}

char	*process_var_expand(char *str, t_shell *shell, t_arena *arena)
{
	char *res;
	char *src;
	char *dst;

	res = arena_alloc(arena, ft_strlen(str) * 2 + 1);
	src = str;
	dst = res;
	while (*src)
	{
		if (*src == '$')
			dst = expand_single_variable(&src, shell, dst);
		else
			*dst++ = *src++;
	}
	*dst = '\0';
	return (res);
}
