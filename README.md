*This project has been created as part of the 42 curriculum by alisseye, llupache.*

# Minishell

## Description
Minishell is a small Unix-like shell written in C. It reimplements core shell behaviors to practice process management, inter-process communication, and parsing. The program reads user input, tokenizes and expands it, builds an abstract syntax tree, and executes commands with support for pipes, redirections, and common built-ins.

## Features
- Execute external binaries with arguments and environment propagation
- Handle pipes `|` and redirections `<`, `>`, `>>`, and heredocs
- Built-ins: `cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`
- Signal handling for interactive use (Ctrl+C, Ctrl+D, Ctrl+\)
- Basic error messaging and cleanup to avoid memory leaks

## Instructions
### Prerequisites
- Linux environment with `make` and `gcc`
- readline library installed

### Build
- Run `make` to build `minishell`
- Run `make clean` to remove objects or `make fclean` to remove the binary

### Run
- Execute `./minishell` from the repository root
- Use standard shell syntax for commands, piping, and redirections

## Project Structure
- `src/` implementation (lexer, parser, executor, built-ins, env, pipes, redirects)
- `include/` public headers
- `libft/` custom libc helpers
- `tests/` lightweight checks and AST printers

## Resources
- Bash Reference Manual
- GNU Readline Library documentation
- AI usage: routine functions were drafted to speed up development; design ideas and data structures were inspired by AI suggestions; AI explanations guided the parser and AST-building algorithm and helped clarify design choices.
