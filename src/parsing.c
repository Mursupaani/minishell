/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:45:53 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/29 16:47:53 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parse_args(char *input, char **envp, t_arena *arena)
{
	t_command	*cmd;
	char		**argv;

	cmd = (t_command *)arena_alloc(arena, sizeof(t_command));
	if (!cmd)
		printf("arena fail cmd\n");
	// error handling
	argv = ft_split_arena(input, ' ', arena);
	if (!argv)
		printf("arena fail argv\n");
	cmd->argv = argv;
	cmd->envp = envp;
	return (cmd);
}

t_command *parse_pipeline(t_token *tokens, t_shell *shell)
{
	t_command	*head;
	t_command	*current;
	t_command	*new_cmd;
	t_token		*token;

	head = NULL;
	current = NULL;
	token = tokens;
	while(token)
	{
		if(token->type == TOKEN_PIPE)
		{
			new_cmd = create_command(shell->command_arena);
			if(!new_cmd)
				return (NULL);
			if(!head)
				head = new_cmd;
			else
				current->next = new_cmd;
			current = new_cmd;
			token = token->next;
		}
		else if(is_redir(token))
		{
			if(!current)
			{
				current = create_command(shell->command_arena);
				if(!head)
					head = current;
			}
			token = handle_redir(current, token, shell->command_arena);
		}
		else
		{
			if(!current)
			{
				current = create_command(shell->command_arena);
				if(!head)
					head = current;
			}
			add_word_to_command(current, token, shell->command_arena);
			token = token->next;
		}
	}
	return (head);
}

t_command *create_command(t_arena *arena)
{
    t_command *cmd;
    
    cmd = arena_alloc(arena, sizeof(t_command));
    if (!cmd)
        return (NULL);
    cmd->argv = NULL;
    cmd->cmd_type = CMD_EXTERNAL;
    cmd->redirections = NULL;
    cmd->pipe_in[0] = -1;
	cmd->pipe_in[1] = -1;
	cmd->pipe_out[0] = -1;
	cmd->pipe_out[1] = -1;
	cmd->pid = -1;
	cmd->next = NULL;
    return (cmd);
}

int is_redir(t_token *token)
{
   if (!token)
       return (0);
   return (token->type == TOKEN_REDIR_IN ||
           token->type == TOKEN_REDIR_OUT ||
           token->type == TOKEN_REDIR_APPEND ||
           token->type == TOKEN_HEREDOC);
}

t_token *handle_redir(t_command *current, t_token *token, t_arena *arena)
{
    t_redir *redir;
	t_token *target;
	t_redir	*tail;
	
	target = token->next;
	redir = arena_alloc(arena, sizeof(t_redir));
	if(!redir)
		return (NULL);
	if(target->type != TOKEN_WORD)
		return (NULL);
	redir = arena_alloc(arena, sizeof(t_redir));
	if(!redir)
		return (NULL);
	redir->type = token_to_redir_type(token->type);
	redir->target = arena_strdup(target->value, arena);
	if(!redir->target)
		return (NULL);
	redir->fd = -1;
	redir->next = NULL;
	if(!current->redirections)
		current->redirections = redir;
	else
	{
		tail  = current->redirections;
		while (tail->next)
			tail = tail->next;
		tail->next = redir;
	}
	return(target->next);
}

t_redir_type token_to_redir_type(t_token_type token_type)
{
    if (token_type == TOKEN_REDIR_IN)
        return (REDIR_INPUT);
    else if (token_type == TOKEN_REDIR_OUT)
        return (REDIR_OUTPUT);
    else if (token_type == TOKEN_REDIR_APPEND)
        return (REDIR_APPEND);
    else if (token_type == TOKEN_HEREDOC)
        return (REDIR_HEREDOC);
    else
        return (REDIR_INPUT);
}
