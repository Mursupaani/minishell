# Minishell

Minishell is a custom, interactive Unix shell built in C. It mimics the core behavior of `bash`, handling everything from parsing command-line inputs to managing processes, pipes, and environment variables. 

**Teamwork:** 
* **My Role:** Execution engine (pipes, redirections, built-ins, HEREDOCs, signal handling and process management).
* **Partner's Role:** Parsing logic (tokenization, quote handling, and syntax validation).

## 📝 About the Project

The goal of this 42 School project is to build a functional shell using a highly restricted set of C standard library functions (e.g., `fork`, `execve`, `pipe`, `dup2`). 

## ✨ Features

* **Command Execution:** Resolves commands via the `PATH` environment variable or relative/absolute paths.
* **Redirections:** 
  * `<` (input)
  * `>` (output)
  * `<<` (heredoc with delimiter)
  * `>>` (append output)
* **Pipes (`|`):** Chains commands, connecting the output of one to the input of the next.
* **Environment Variables:** Expands `$VAR` to its value and `$?` to the last command's exit status.
* **Quotes:** 
  * Single quotes (`'`) block all meta-character interpretation.
  * Double quotes (`"`) block meta-characters except for `$`.
* **Signals:** 
  * `ctrl-C`: New prompt on a new line.
  * `ctrl-D`: Exits the shell.
  * `ctrl-\`: Ignored.
* **History:** Maintains a working command history using `readline`.

### Built-in Commands
* `echo` (with `-n` flag)
* `cd` (relative or absolute path)
* `pwd`
* `export`
* `unset`
* `env`
* `exit`

## 🛠️ Compilation & Usage

**Prerequisites:** A C compiler (`cc`) and the `readline` library.

1. Clone and compile the project:
```bash
git clone <your-repository-url>
cd minishell
make
```

2. Run the shell:
```bash
./minishell
```

3. **Make rules:** `make` (compile), `make clean` (remove objects), `make fclean` (remove objects and executable), `make re` (recompile).
