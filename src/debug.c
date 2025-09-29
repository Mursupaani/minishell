/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 07:55:31 by anpollan          #+#    #+#             */
/*   Updated: 2025/09/29 07:56:03 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void print_tokens(t_token *tokens)
{
    t_token *current = tokens;
    
    while (current)
    {
        printf("Token: type=%d, value='%s', quoted=%d, expandable=%d\n",
               current->type, current->value, current->quoted, current->expandable);
        current = current->next;
    }
    printf("--- End of tokens ---\n");
}

void print_commands(t_command *commands)
{
    t_command *cmd = commands;
    int cmd_num = 1;
    
    while (cmd)
    {
        printf("Command %d:\n", cmd_num++);
        
        // Print argv
        if (cmd->argv)
        {
            printf("  cmd_type: %d\n", cmd->cmd_type);
            printf("  argv: ");
            for (int i = 0; cmd->argv[i]; i++)
                printf("'%s' ", cmd->argv[i]);
            printf("\n");
        }
        
        // Print redirections
        t_redir *redir = cmd->redirections;
        while (redir)
        {
            printf("  redirection: type=%d target='%s'\n", 
                   redir->type, redir->target);
            redir = redir->next;
        }
		if (cmd->heredoc_filename)
            printf("  cmd heredoc filename: '%s'\n", cmd->heredoc_filename); 
		printf("command type: %d\n", cmd->cmd_type);
        
        cmd = cmd->next;
        if (cmd)
            printf("  | (pipe to next command)\n");
    }
    printf("--- End of command chain ---\n");
}
