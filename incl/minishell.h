/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:54:57 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/21 17:57:07 by anpollan         ###   ########.fr       */
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

#define COMMAND_ARENA_SIZE 1024
#define SESSION_ARENA_SIZE 1024

// ============================================================================
// HASH TABLE FOR ENVIRONMENT (Simple implementation)
// ============================================================================
# define HASH_TABLE_SIZE 256

typedef struct s_env_entry {
    char                *key;
    char                *value;
    struct s_env_entry  *next;
} t_env_entry;

typedef struct s_hash_table {
    t_env_entry *buckets[HASH_TABLE_SIZE];
} t_hash_table;

// ============================================================================
// SIMPLIFIED TOKENIZER (2-pass system for mandatory)
// ============================================================================
typedef enum e_token_type {
    TOKEN_WORD,          // command, argument, filename
    TOKEN_PIPE,          // |
    TOKEN_REDIR_IN,      // 
    TOKEN_REDIR_OUT,     // >
    TOKEN_REDIR_APPEND,  // >>
    TOKEN_HEREDOC,       // 
    TOKEN_EOF
} t_token_type;

typedef struct s_token {
    t_token_type    type;
    char            *value;
    int             quoted;       // 1=single, 2=double, 0=none
    int             expandable;   // Should $VAR expand?
    struct s_token  *next;
} t_token;

// ============================================================================
// COMMAND STRUCTURES (same as before)
// ============================================================================
typedef enum e_cmd_type {
    CMD_BUILTIN_PARENT,    // cd, export, unset, exit (run in shell process)
    CMD_BUILTIN_CHILD,     // echo, pwd, env (run in child)
    CMD_EXTERNAL           // everything else
} t_cmd_type;

typedef enum e_builtin_type {
    BUILTIN_NONE = -1,
    BUILTIN_ECHO,
    BUILTIN_CD,
    BUILTIN_PWD,
    BUILTIN_EXPORT,
    BUILTIN_UNSET,
    BUILTIN_ENV,
    BUILTIN_EXIT
} t_builtin_type;

typedef enum e_redir_type {
    REDIR_INPUT,           // 
    REDIR_OUTPUT,          // >
    REDIR_APPEND,          // >>
    REDIR_HEREDOC,         // 
    REDIR_ERROR            // 2>
} t_redir_type;

typedef struct s_redir {
    t_redir_type    type;
    char            *target;    // filename or heredoc delimiter
    int             fd;         // file descriptor when opened
    struct s_redir  *next;
} t_redir;

typedef struct s_command {
    char		**argv;
	// Do we need this inside t_command?
	char		**envp;
    t_cmd_type	cmd_type;
    t_redir		*redirections;  // ordered list of redirections
    
    // Pipe management
    int			pipe_in[2];
    int			pipe_out[2];
    pid_t		pid;
    
	// Child process status and error
	int			status;
	int			error;
    struct s_command *next;
} t_command;

// ============================================================================
// SIMPLE PARSER STATE (no AST for mandatory)
// ============================================================================
typedef struct s_parser {
    t_token     *tokens;          // Token list head
    t_token     *current;         // Current position
    t_command   *cmd_head;        // First command in pipeline
    t_command   *cmd_current;     // Current command being built
    char        **current_argv;   // Building argv array
    int         arg_num;          // number of arguments
    int         argv_capacity;    // Space allocated for argv
} t_parser;

// ============================================================================
// SHELL SESSION STATE
// ============================================================================
typedef enum e_shell_mode {
    MODE_PROMPT,
    MODE_HEREDOC, 
    MODE_WAIT
} t_shell_mode;

typedef struct s_shell {
	// User input
	char			*input;

    // Environment subsystem
    t_hash_table    *env_table;        // Hash table for environment
    char            **env_array;       // Built from env_table before fork
    char            **path_dirs;       // PATH cache
    int             path_dirty;        // Invalidation flag
    
    // Status & mode
    int             last_exit_status;  // For $?
    int             interactive;       // isatty result
    t_shell_mode    mode;             // Current mode
    
    // Terminal context
    struct termios  original_termios;
    int             stdin_fd;
    int             stdout_fd;
    int             stderr_fd;
    
    // Heredoc management
    char            *tmp_dir;
    int             heredoc_counter;
    
    // Memory management
    t_arena         *session_arena;    // Lives across commands
    t_arena         *command_arena;    // Reset after each command
} t_shell;

// ============================================================================
// GLOBAL VARIABLE (ONLY ONE ALLOWED BY PDF)
// ============================================================================

// FIXME: EXTERN IS NOT ALLOWED
extern volatile sig_atomic_t g_signal_received;

//WARN: No needed anywhere?
void	print_str_array(char **str_array);

// Shell modes
int	interactive_shell(int argc, char **argv, char **envp);
int	non_interactve_shell(int argc, char **argv, char **envp);

// Execution
int	execute_command(t_command *cmd);

// Built-in commands
int	change_directory(const char *path);
int	print_working_directory(void);

// Parsing
t_command	*parse_args(char *input, char **envp, t_arena *arena);

#endif
