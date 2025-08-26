/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:57:16 by magebreh          #+#    #+#             */
/*   Updated: 2025/08/26 17:51:09 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *tokenize(char *input, t_arena *arena)
{
    t_token *head;
    t_token *current;
    t_token *new_token;
    char *pos;

    head = NULL;
    current = NULL;
    pos = input;
    while (*pos)
    {
        pos = skip_whitespace(pos);
        if (!*pos)
            break;
        new_token = create_next_token(&pos, arena);
        if (!new_token)
            return (free_token_list(head), NULL);
        if (!head)
            head = new_token;
        else
            current->next = new_token;
        current = new_token;
    }
    return (head);
}

static t_token *create_next_token(char **pos, t_arena *arena)
{
    if (**pos == '|')
        return (tokenize_pipe(pos, arena));
    else if (**pos == '<')
        return (tokenize_input_redirect(pos, arena));
    else if (**pos == '>')
        return (tokenize_output_redirect(pos, arena));
    else if (**pos == '"')
        return (tokenize_double_quote(pos, arena));
    else if (**pos == '\'')
        return (tokenize_single_quote(pos, arena));
    else
        return (tokenize_word(pos, arena));
}

static t_token *tokenize_pipe(char **pos, t_arena *arena)
{
    t_token *token;
    
    token = arena_alloc(arena, sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = TOKEN_PIPE;
    token->value = arena_strdup("|", arena);
    token->quoted = 0;
    token->expandable = 0;
    token->next = NULL;
    
    (*pos)++; // Move past the '|'
    return (token);
}
