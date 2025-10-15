/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_variables_execution.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 13:58:42 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/15 10:47:03 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prepare_cmd(t_command *cmd, t_shell *shell)
{
	t_command	*current;
	t_redir		*redir;

	current = cmd;
	while (current)
	{
		redir = current->redirections;
		while (redir)
		{
			redir->target = arena_expand_variables(redir->target, shell);
			redir = redir->next;
		}
		expand_cmd(current, shell);
		current = current->next;
	}
}

void expand_cmd(t_command *cmd, t_shell *shell)
{
    int i;
    int k;
    int split_count;
    int total_args;
    char *expanded;
    char **split_result;
    char **new_argv;
    bool *new_expandable;

    i = 0;
    if (!cmd->argv)
        return;
    while (cmd->argv[i])
    {
        if (cmd->argv_expandable && cmd->argv_expandable[i])
        {
            expanded = expand_var(cmd->argv[i], shell, shell->command_arena);
            if (expanded && ft_strchr(expanded, ' ') && ft_strchr(cmd->argv[i], '$'))
            {
                split_result = ft_split(expanded, ' ');
                if (split_result && split_result[0])
                {
                    split_count = 0;
                    while (split_result[split_count])
                    	split_count++;
                    total_args = 0;
                    while (cmd->argv[total_args])
                        total_args++;
                    new_argv = arena_alloc(shell->command_arena, sizeof(char *) * (total_args + split_count));
                    new_expandable = arena_alloc(shell->command_arena, sizeof(bool) * (total_args + split_count));
                    ft_memcpy(new_argv, cmd->argv, sizeof(char *) * i);
                    ft_memcpy(new_expandable, cmd->argv_expandable, sizeof(bool) * i);
                    k = 0;
                    while (k < split_count)
                    {
                        new_argv[i + k] = arena_strdup(split_result[k], shell->command_arena);
                        new_expandable[i + k] = false;
                        free(split_result[k]);
                        k++;
	                }
                    free(split_result);
                    ft_memcpy(new_argv + i + split_count, cmd->argv + i + 1, sizeof(char *) * (total_args - i));
                    ft_memcpy(new_expandable + i + split_count, cmd->argv_expandable + i + 1, sizeof(bool) * (total_args - i));
                    cmd->argv = new_argv;
                    cmd->argv_expandable = new_expandable;
                    i += split_count;
                    continue;
                }
            }
            else if (expanded)
                cmd->argv[i] = expanded;
        }
        else
            cmd->argv[i] = expand_var(cmd->argv[i], shell, shell->command_arena);
        i++;
    }
}


char	*expand_var(char *str, t_shell *shell, t_arena *arena)
{
	char *res;

	if(!str)
		return(NULL);
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
		//FIXME: sprintf is not allowed
		dst += sprintf(dst, "%d", shell->last_exit_status);
		(*src)++;
	}
	else
	{
		var_start = *src;
		while (**src && (ft_isalnum(**src) || **src == '_')
			&& **src != '\'' && **src != '"')
			(*src)++;
		var_len = *src - var_start;

		if (var_len == 0)
		{
			*dst++ = '$';
		}
		else
		{
			//FIXME: strncpy is not allowed
			strncpy(var_name, var_start, var_len);
			var_name[var_len] = '\0';

			value = hash_table_get(shell->env_table, var_name);
			if (value)
			{
				//FIXME: sprintf is not allowed
				dst += sprintf(dst, "%s", value);
			}
		}
	}
	return (dst);
}

char	*process_var_expand(char *str, t_shell *shell, t_arena *arena)
{
	char *res;
	char *src;
	char *dst;
	size_t est_size;
	char in_quote;

	est_size = ft_strlen(str) * 10 + 1024;
	res = arena_alloc(arena, est_size);
	src = str;
	dst = res;
	in_quote = 0;

	while (*src)
	{
		if (*src == '$' && (*(src + 1) == '"' || *(src + 1) == '\'') && !in_quote)
		{
			src++;
			continue;
		}
		if ((*src == '\'' || *src == '"') && !in_quote)
		{
			in_quote = *src;
			src++;
		}
		else if (*src == in_quote)
		{
			in_quote = 0;
			src++;
		}
		else if (*src == '$' && in_quote != '\'' && *(src + 1) != '\0')
		{
			if (in_quote && (*(src + 1) == '\'' || *(src + 1) == '"'))
				*dst++ = *src++;
			else
				dst = expand_single_variable(&src, shell, dst);
		}
		else
			*dst++ = *src++;
	}
	*dst = '\0';

	return (res);
}
