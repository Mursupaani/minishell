# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 11:49:18 by magebreh          #+#    #+#              #
#    Updated: 2025/09/12 14:09:08 by anpollan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INCL_DIR) -g

LIBFT		= ./libft/libft.a

RM = rm -f

# Object directory
OBJ_DIR = obj
SRC_DIR = src
INCL_DIR = incl

# Core libft sources
SRC = main.c print_working_directory.c change_directory.c execute_command.c \
	  interactive_shell.c non_interactive_shell.c parsing.c signals.c shell.c \
	  utils.c echo.c token.c fork.c execute_pipe.c execute_redirection.c \
	  find_file_from_path.c strjoin_arena.c environment_variables_execution.c

# Combined sources
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline  -o $(NAME)
	@echo "✅ $(NAME) created successfully"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
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
