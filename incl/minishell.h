/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:54:57 by anpollan          #+#    #+#             */
/*   Updated: 2025/08/16 15:55:18 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

// ============================================================================
// ARENA MEMORY MANAGEMENT (from libft)
// ============================================================================
typedef struct s_arena {
    unsigned char   *data;
    size_t          capacity;
    size_t          size;
    struct s_arena  *next;
} t_arena;

// ============================================================================
// HASH TABLE FOR ENVIRONMENT
// ============================================================================
typedef struct s_env_entry {
    char                *key;
    char                *value;
    struct s_env_entry  *next;
} t_env_entry;

typedef struct s_hash_table {
    t_env_entry **buckets;
    size_t      size;
} t_hash_table;

// ============================================================================
// COMMAND CLASSIFICATION
// ============================================================================
typedef enum e_cmd_type {
    CMD_BUILTIN_PARENT,    // cd, export, unset, exit (run in shell process)
    CMD_BUILTIN_CHILD,     // echo, pwd, env (run in child)
    CMD_EXTERNAL           // everything else
} t_cmd_type;

typedef enum e_builtin_type {
    BUILTIN_NONE = -1,
    BUILTIN_ECHO,      // Child process
    BUILTIN_CD,        // Parent only
    BUILTIN_PWD,       // Child process  
    BUILTIN_EXPORT,    // Parent only
    BUILTIN_UNSET,     // Parent only
    BUILTIN_ENV,       // Child process
    BUILTIN_EXIT       // Parent only
} t_builtin_type;

// ============================================================================
// REDIRECTION SYSTEM
// ============================================================================
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

// ============================================================================
// COMMAND STRUCTURE (execution-ready)
// ============================================================================
typedef struct s_command {
    char        **argv;
    t_cmd_type  cmd_type;
    t_redir     *redirections;  // ordered list of redirections
    
    // Pipe management
    int         pipe_in[2];
    int         pipe_out[2];
    pid_t       pid;
    
    struct s_command *next;
} t_command;

// ============================================================================
// PIPELINE STRUCTURE
// ============================================================================
typedef struct s_pipeline {
    t_command           *commands;      // First command in pipeline
    int                 exit_status;    // Pipeline's exit status
    struct s_pipeline   *next;          // For && || operators (bonus)
} t_pipeline;

// ============================================================================
// SHELL SESSION STATE
// ============================================================================
typedef enum e_shell_mode {
    MODE_PROMPT,
    MODE_HEREDOC, 
    MODE_WAIT
} t_shell_mode;

typedef struct s_shell {
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
extern volatile sig_atomic_t g_signal_received;


#endif