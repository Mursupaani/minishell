# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 11:49:18 by magebreh          #+#    #+#              #
#    Updated: 2025/10/16 17:51:50 by anpollan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INCL_DIR) -Ilibft -g

LIBFT		= ./libft/libft.a

RM = rm -f

# Object directory
OBJ_DIR = obj
SRC_DIR = src
INCL_DIR = incl

# Header
HEADER = ./incl/minishell.h
LFT_HEADER = ./libft/libft.h

# Core libft sources
SRC = main.c print_working_directory.c change_directory.c execute_command.c \
	  interactive_shell.c non_interactive_shell.c parsing.c shell.c \
	  utils.c echo.c token.c fork.c execute_pipe.c execute_redirection.c \
	  strjoin_arena.c environment_variables_execution.c \
	  handle_heredocs.c env_array_from_hashtable.c export.c unset.c \
	  environment_utils.c exit.c execute_external_command.c \
	  execute_command_utils.c check_redirections.c env.c heredoc_utils.c \
	  pipe_utils.c quick_sort_string_array.c expand_variables_from_input.c \
	  update_prompt.c parent_signals.c child_signals.c heredoc_signals.c \
	  update_shell_lvl.c print_environment_variables.c \
	  arena_expand_variables.c export_utils.c debug.c

# Combined sources
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
