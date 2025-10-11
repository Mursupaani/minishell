/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:54:57 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/11 17:46:18 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <curses.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/resource.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>
# include <../libft/libft.h>

# define COMMAND_ARENA_SIZE 4096
# define SESSION_ARENA_SIZE 8192
# define HASH_TABLE_SIZE 256
# define PWD_BUFFER 4096

extern volatile sig_atomic_t	g_signal_received;
typedef struct s_command		t_command;
// ============================================================================
// HASH TABLE FOR ENVIRONMENT (Simple implementation)
// ============================================================================

typedef struct s_env_entry
{
	char						*key;
	char						*value;
	struct s_env_entry			*next;
}	t_env_entry;

typedef struct s_hash_table
{
	t_env_entry					*buckets[HASH_TABLE_SIZE];
}	t_hash_table;

// ============================================================================
// SIMPLIFIED TOKENIZER (2-pass system for mandatory)
// ============================================================================
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
	int							quoted;
	int							expandable;
	struct s_token				*next;
}	t_token;

// ============================================================================
// COMMAND STRUCTURES (same as before)
// ============================================================================
typedef enum e_cmd_type
{
	CMD_BUILTIN_PARENT,
	CMD_BUILTIN_CHILD,
	CMD_EXTERNAL
}	t_cmd_type;

typedef enum e_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	REDIR_ERROR
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type				type;
	char						*target;
	int							fd;
	struct s_redir				*next;
}	t_redir;

typedef struct s_command
{
	char						**argv;
	bool						*argv_expandable;
	t_cmd_type					cmd_type;
	t_redir						*redirections;
	char						*heredoc_filename;
	struct s_command			*next;
}	t_command;

// ============================================================================
// SIMPLE PARSER STATE (no AST for mandatory)
// ============================================================================
// FIXME: Not using this
// typedef struct s_parser
// {
// 	t_token				*tokens;
// 	t_token				*current;
// 	t_command			*cmd_head;
// 	t_command			*cmd_current;
// 	char				**current_argv;
// 	int				arg_num;
// 	int				argv_capacity;
// }	t_parser;

typedef struct s_shell
{
	// User input
	char						*input;
	char						*prompt;

	// Environment subsystem
	t_hash_table				*env_table;
	char						**env_array;
	char						**path_dirs;

	// Status & mode
	int							last_exit_status;

	// Terminal context
	//FIXME: Not using original_termios. What is it?
	struct termios				original_termios;
	int							stdin_fd;
	int							stdout_fd;
	int							child_pid;
	int							**pipe_array;
	int							*pipe_pids;

	// Heredoc management
	//FIXME: Not using tmp_dir. Thinking about using.
	char						*tmp_dir;
	//FIXME: Not using heredoc counder. Need this to check max heredocs.
	int							heredoc_counter;

	// Memory management
	t_arena						*session_arena;
	t_arena						*command_arena;
}	t_shell;

// ============================================================================
// FUNCTION PROTOTYPES
// ============================================================================

// FIXME: Debug. Can be deleted from final:
// also debug.c
void			print_tokens(t_token *tokens);
void			print_commands(t_command *commands);

// FIXME: Not using these:
int				is_parent_only_builtin(char *cmd_name);
void			print_str_array(char **str_array);

// Signal handling
void			setup_parent_signals(void);
void			setup_child_signals(void);
void			setup_heredoc_signals(void);

// Shell initialization and management
t_shell			*shell_init(char **env);
int				cleanup_after_execution(t_shell *shell, t_command *cmd);
void			update_prompt(t_shell *shell);
void			update_shell_lvl(t_shell *shell);
int				error_exit_and_free_memory(t_shell *shell);
void			free_memory_at_exit(t_shell *shell);

// Environment
t_hash_table	*populate_env_from_envp(char **envp, t_arena *arena);
char			**env_array_from_hashtable(t_shell *shell);
void			update_env_table_and_arr(t_shell *shell);

// Shell modes
int				interactive_shell(t_shell *shell);
int				non_interactve_shell(t_shell *shell, char **argv);

// Execution
void			execute_commands(t_command *cmd, t_shell *shell);
void			choose_execution_type(t_command *cmd, t_shell *shell);
void			execute_builtin_command(t_command *cmd, t_shell *shell);
void			execute_external_command(t_command *cmd, t_shell *shell);
void			find_non_empty_argument(t_command *cmd);

// Pipe
void			execute_pipe(t_command *cmd, t_shell *shell);
int				**arena_alloc_pipe_arr(t_shell *shell, int cmd_count);
int				count_commands(t_command *cmd);
int				close_unused_fds(
					int **pipe_array, int cmd_count, int process_index);

// Redirection
int				check_input_redirection(t_redir *redir, t_shell *shell);
int				check_output_redirection(t_redir *redir, t_shell *shell);
int				check_append_redirection(t_redir *redir, t_shell *shell);
int				execute_redirection(
					t_redir *redir, t_command *cmd, t_shell *shell);
int				handle_heredocs(t_command *cmd, t_shell *shell);
char			*expand_variables_from_input(char *input, t_shell *shell);
char			**split_input(char *input, t_shell *shell);

// Built-in commands
void			change_directory(t_command *cmd, t_shell *shell);
void			print_working_directory(t_shell *shell);
void			env_builtin(t_command *cmd, t_shell *shell);
void			ft_echo(t_command *cmd, t_shell *shell);
void			print_environment_variables(
					char **env, t_shell *shell, bool export);
void			export_environment_variable(t_command *cmd, t_shell *shell);
void			unset_environment_variable(t_command *cmd, t_shell *shell);
char			*get_current_directory(t_shell *shell);
void			exit_builtin(t_command *cmd, t_shell *shell);

// Environment & Variable expansion
t_hash_table	*hash_table_create(t_arena *arena);
char			*hash_table_get(t_hash_table *table, char *key);
void			hash_table_set(t_hash_table *table,
					char *key, char *value, t_arena *arena);
void			prepare_cmd(t_command *cmd, t_shell *shell);
void			expand_cmd(t_command *cmd, t_shell *shell);
char			*expand_var(char *str, t_shell *shell, t_arena *arena);
char			*process_var_expand(char *str, t_shell *shell, t_arena *arena);
void			hash_table_delete(t_hash_table *table, char *key);
char			**copy_env_array(t_shell *shell, t_arena *arena, int *count);

// Parsing
t_command		*parse_pipeline(t_token *tokens, t_shell *shell);
t_command		*create_command(t_arena *arena);
int				is_redir(t_token *token);
t_token			*handle_redir(t_command *current, t_token *token,
					t_arena *arena, int *error);
void			add_word_cmd(t_command *cmd, t_token *word, t_arena *arena);
t_redir_type	token_to_redir_type(t_token_type token_type);
void			attach_redir(t_command *cmd, t_redir *redir);
int				needs_realloc(int current_count);
int				calculate_new_capacity(int current_count);

// Tokenizer functions (token.c)
t_token			*tokenize(char *input, t_arena *arena);

// Utility functions for tokenizer (utils.c)
char			*skip_whitespace(char *pos);
char			*arena_substr(
					char *src, size_t start, size_t len, t_arena *arena);
char			*arena_strdup(const char *s, t_arena *arena);
int				ft_is_special_char(char c);
int				ft_isspace(char c);
char			**ft_split_arena(char const *s, char c, t_arena *arena);

// Utility functions (utils.c)
void			cleanup_shell_partial(t_shell *shell, int level);
char			*arena_strdup(const char *s, t_arena *arena);
void			quick_sort_string_array(char **str_arr, int start, int end);

// Error handling fork wrapper
int				create_fork(t_shell *shell);
int				is_builtin_command(char *cmd_name);
void			classify_commands(t_command *cmd);

#endif
