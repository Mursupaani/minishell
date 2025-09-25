/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:45:53 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/22 12:49:40 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	attach_heredoc_filename_to_command(t_command *cmd, t_arena *arena);

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
	int redir_error;

    head = NULL;
    current = NULL;
    token = tokens;
    while(token)
    {
        if(token->type == TOKEN_PIPE)
        {
			if(!current || !current->argv || !current->argv[0])
				return (NULL);
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
            token = handle_redir(current, token, shell->command_arena, &redir_error);
			if(redir_error)
				return (NULL);
        }
        else
        {
            if(!current)
            {
                current = create_command(shell->command_arena);
                if(!head)
                    head = current;
            }
            add_word_cmd(current, token, shell->command_arena);
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
	cmd->heredoc_filename = NULL;
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

t_token *handle_redir(t_command *current, t_token *token, t_arena *arena, int *error)
{
    t_redir *redir;
    t_token *target;
    // t_redir	*tail;
    
    *error = 0;
    target = token->next;
    if (!target || target->type != TOKEN_WORD)
    {
        *error = 1;
        return (NULL);
    }
    
    redir = arena_alloc(arena, sizeof(t_redir));
    if (!redir)
    {
        *error = 1;
        return (NULL);
    }
    
    redir->type = token_to_redir_type(token->type);
    redir->target = arena_strdup(target->value, arena);
	if (redir->type == REDIR_HEREDOC)
		attach_heredoc_filename_to_command(current, arena);
    if (!redir->target)
    {
        *error = 1;
        return (NULL);
    }
    redir->fd = -1;
    redir->next = NULL;
    attach_redir(current, redir);
    return (target->next);
}

void add_word_cmd(t_command *cmd, t_token *word, t_arena *arena)
{
    int current_count;
    int req_capacity;
    char **new_argv;
    int i;
    
    current_count = 0;
    if(cmd->argv)
    {
        while (cmd->argv[current_count])
            current_count++;
    }
    req_capacity = calculate_new_capacity(current_count + 1);
    if(!cmd->argv || needs_realloc(current_count + 1))
    {
        new_argv = arena_alloc(arena, req_capacity*sizeof(char*));
        if(!new_argv)
            return ;
        i = 0;
        while (i < req_capacity)
        {
            new_argv[i] = NULL;
            i++;
        }
        i = 0;
        if(cmd->argv)
        {
            while (i < current_count)
            {
                new_argv[i] = cmd->argv[i];
                i++;
            }
        }
        cmd->argv = new_argv;
    }
    cmd->argv[current_count] = arena_strdup(word->value, arena);
    cmd->argv[current_count + 1] = NULL;
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


void attach_redir(t_command *cmd, t_redir *redir)
{
    if (!cmd->redirections)
        cmd->redirections = redir;
    else
    {
        t_redir *tail = cmd->redirections;
        while (tail->next)
            tail = tail->next;
        tail->next = redir;
    }
}

int needs_realloc(int current_count)
{
    int capacity;

    if (current_count == 0)
        return (1);
    
    capacity = 8;
    while (capacity <= current_count)
        capacity *= 2;
    
    return (current_count >= capacity / 2);
}

int calculate_new_capacity(int current_count)
{
    int  capacity;

    if (current_count == 0)
        return 8;
    capacity = 8;
    while (capacity <= current_count)
        capacity *= 2;
    return capacity;
}

void classify_commands(t_command *cmd)
{
    t_command *current;
    int is_single;

    current = cmd;
    is_single = (cmd->next == NULL);
    while (current)
    {
        if (!current->argv || !current->argv[0])
        {
            current = current->next;
            continue;
        }
		if (is_builtin_command(current->argv[0]))
        {
            if (is_single)
			// FIXME: No need for the is_parent_only_builtin(current->argv[0]))?
            // if (is_single && is_parent_only_builtin(current->argv[0]))
                current->cmd_type = CMD_BUILTIN_PARENT;
            else
                current->cmd_type = CMD_BUILTIN_CHILD;
        }
		else
			current->cmd_type = CMD_EXTERNAL;
        current = current->next;
    }
}

static int	attach_heredoc_filename_to_command(t_command *cmd, t_arena *arena)
{
	static unsigned int	file_counter;
	const char			*heredoc_name_base = ".heredoc-";
	char				*temp;

	if (cmd->heredoc_filename == NULL)
	{
		temp = ft_itoa(file_counter);
		if (!temp)
			return (1);
		cmd->heredoc_filename = ft_strjoin_arena(heredoc_name_base, temp, arena);
		free(temp);
		file_counter++;
	}
	return (0);
}
