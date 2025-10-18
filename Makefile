# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 11:49:18 by magebreh          #+#    #+#              #
#    Updated: 2025/10/18 18:12:57 by anpollan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INCL_DIR) -Ilibft -g

LIBFT		= ./libft/libft.a

RM = rm -f

OBJ_DIR = obj
SRC_DIR = src
INCL_DIR = incl

HEADER = ./incl/minishell.h
LFT_HEADER = ./libft/libft.h

SRC = main.c \
	  builtins/change_directory.c \
	  builtins/echo.c \
	  builtins/env.c \
	  builtins/exit.c \
	  builtins/export.c \
	  builtins/export_utils.c \
	  builtins/print_working_directory.c \
	  builtins/unset.c \
	  debug/debug.c \
	  debug/print_environment_variables.c \
	  environment/env_array_from_hashtable.c \
	  environment/environment_utils.c \
	  environment/env_expand.c \
	  environment/env_expand_cmd.c \
	  executor/execute_command.c \
	  executor/execute_command_utils.c \
	  executor/execute_external_command.c \
	  executor/execute_pipe.c \
	  executor/pipe_utils.c \
	  heredoc/arena_expand_variables.c \
	  heredoc/expand_variables_from_input.c \
	  heredoc/handle_heredocs.c \
	  heredoc/heredoc_signals.c \
	  heredoc/heredoc_utils.c \
	  heredoc/heredoc_utils2.c \
	  parser/parsing.c \
	  redirections/check_redirections.c \
	  redirections/execute_redirection.c \
	  shell/interactive_shell.c \
	  shell/non_interactive_shell.c \
	  shell/shell.c \
	  shell/update_prompt.c \
	  shell/update_shell_lvl.c \
	  signals/child_signals.c \
	  signals/parent_signals.c \
	  tokenizer/token.c \
	  utils/fork.c \
	  utils/quick_sort_string_array.c \
	  utils/strjoin_arena.c \
	  utils/utils.c \
	  utils/utils2.c


OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline  -o $(NAME)
	@echo "✅ $(NAME) created successfully"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@$(MAKE) -s -C libft

clean:
	@$(RM) -r $(OBJ_DIR)
	@$(MAKE) -C libft clean
	@echo "🧹 Object files cleaned"

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C libft fclean
	@echo "🧹 All files cleaned"

re: fclean all

.PHONY: all bonus clean fclean re info
